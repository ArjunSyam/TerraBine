#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

//Input line values
typedef struct {
  char * username;
  int line;
} InputLine;


const char *builtin_func_list[][2] = {
  {"ls","./shell_cmds/ls.sh"},
  {"cd",". ./shell_cmds/cd.sh"},
  {"mv","./shell_cmds/mv.sh"},
  {"pwd","./shell_cmds/pwd.sh"},
  {"delete","./shell_cmds/delete.sh"},
  {"cat","./shell_cmds/cat.sh"},
};

//Macros
#define check_end(msg) (strcmp(msg, "end") == 0)
#define check_clear(msg) (strcmp(msg, "cls") == 0)
#define CMD_ARG_BUFSIZE 64;
#define CMD_BUFSIZE 30; //not used yet
#define display(msg) mvprintw(input->line++,1,"%s",msg);
#define TOK_DELIM " "

//functions

//input line function
//retuens the line inputed by user
char *write_command(InputLine *input) {
  int bufsize = CMD_BUFSIZE; 
  char *line = (char *)malloc(bufsize*sizeof(char)); //figure out dynamic alloction
  if (line == NULL) {
    return NULL;
  };

  mvprintw(input->line++,1,"%s",input->username);
  getstr(line);
  
  return line;
}

//split line into arguments function
char **split_line(char *cmd,InputLine * input) {
  int i = 0;
  int bufsize = CMD_ARG_BUFSIZE;
  char **tokens = (char **)malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    display("ERROR: alloction error in split_line: tokens");
    exit(EXIT_FAILURE);
  }
  
  token = strtok(cmd,TOK_DELIM);
  while (token != NULL ) {
    //handling comments
    if (token[0] == '#')
      break;

    tokens[i++] = token;

    if (i >= bufsize) {
      bufsize++;
      tokens = realloc(tokens, bufsize * sizeof(char *));

      if (!tokens) {
        display("ERROR: alloction error in split_line: tokens");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, TOK_DELIM);
  }
  tokens[i] = NULL;
  return (tokens);
}

//command argument execution
void execute_args(char **cmd_args,InputLine * input) {

  int check_presence = 0;
  size_t no_builtin_func = sizeof(builtin_func_list)/sizeof(builtin_func_list[0]);

  for(size_t cmd_val = 0; cmd_val<no_builtin_func; cmd_val++) {
    if (strcmp(cmd_args[0],builtin_func_list[cmd_val][0]) == 0) {
      check_presence = 1;
      cmd_args[0] = (char *) builtin_func_list[cmd_val][1];

      //setting script as executable
      if(chmod(cmd_args[0], S_IRWXU) == -1) {
        perror("chmod failed");
        return;
      }

      int Pipe_PtoC [2];

      if (pipe(Pipe_PtoC) == -1) {
        perror("pipe failed");
      };

      pid_t pid = fork();

      if (pid == -1) {
        perror("fork failed");
        return;
      } 

      else if(pid == 0) {
        //child process

        close(Pipe_PtoC[0]); //closes the reading end only needs to write
        dup2(Pipe_PtoC[1],STDOUT_FILENO); //writes the values coming out of sh file into STDOUT_FILENO
        close(Pipe_PtoC[1]);

        execvp(cmd_args[0],cmd_args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
      }

      else {
        //parent process

        close(Pipe_PtoC[1]); // only needs to read the out of the childprocess
        
        char buffer[1024];
        ssize_t nbytes;

        while((nbytes = read(Pipe_PtoC[0],buffer,sizeof(buffer) - 1)) > 0) {
          buffer[nbytes] = '\0'; // adds end of line to each line of buffer
          display(buffer) ;
        }
        
        close(Pipe_PtoC[0]);
        waitpid(pid, NULL, 0);
      }
    }

  }

  if(!check_presence) {
    mvprintw(input->line++,1,"Command not found: %s",cmd_args[0]);
  }

}

int main(int argc, char **argv) {
  //init screen
  initscr();
  start_color();
  
  //intializing input line
  InputLine *input = (InputLine *)malloc(sizeof(InputLine));
  if (input == NULL) {
    endwin();
    return -1;
  }

  input->username = "Skibbidi@SigmaLaptop:~$ ";
  input->line = 0;

  if(can_change_color()) {
    init_color(COLOR_BLUE,0,0,300);
  }

  //define color parts
  init_pair(1, COLOR_WHITE, COLOR_BLUE);

  //set up full screen with the color
  bkgd(COLOR_PAIR(1));

  //Hello display method
  attron(COLOR_PAIR(1)); 
  mvprintw(input->line++,1,"Hello World, Welcome to TerraBine");
  attroff(COLOR_PAIR(1));

  //main messaging area
  while(1) {
    attron(COLOR_PAIR(1)); //turn on the color init_pair
    char * cmd = write_command(input);

    if(cmd == NULL) {
      break;
    }
    
    //for end cmd
    if(check_end(cmd)){
      free(cmd);
      break;
    }
    
    //for clr cmd
    else if(check_clear(cmd)) {
      clear();
      input->line = 0;
    }
    
    //command execution call
    else if(strcmp(cmd,"") != 0) {
      //spliting the input line into command and its arguments
      char **cmd_args;
      //char *result;

      cmd_args = split_line(cmd,input);
      execute_args(cmd_args,input);
      //mvprintw(input->line++,1,"%s",msg);
    }
    
    free(cmd);
    attroff(COLOR_PAIR(1));
    refresh();
    //pause the screen output
    //getch();
  }
  
  free(input);
  endwin();
  return 0;
}
