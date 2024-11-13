#include <ncurses.h>
#include <string.h>

//Input line values
typedef struct {
  char * username;
  int line;
} InputLine;

//Macros
#define check_end strcmp(msg,"end") == 0

//input line method
char * write_message(InputLine *input) {
  char * line;
  mvprintw(input->line++,1,"%s",input->username);
  getstr(line);
  
  return line;
}

int main(int argc, char **argv) {
  //init screen
  initscr();
  start_color();
  
  //intializing input line
  InputLine *input;
  input->username = "Skibbidi@SigmaLaptop:~$ ";
  input->line = 0;

  if(can_change_color()) {
    init_color(COLOR_BLUE,0,0,300);
  }

  //define color parts
  init_pair(1, COLOR_WHITE, COLOR_BLUE);

  //set up full screen with the color
  bkgd(COLOR_PAIR(1));

  //main messaging area
  while(1) {
    attron(COLOR_PAIR(1)); //turn on the color init_pair
    char * msg = write_message(input);

    if(check_end) {
      break;
    }
    else {
      mvprintw(input->line++,1,"%s",msg);
    }

    attroff(COLOR_PAIR(1));
    refresh();
    //pause the screen output
    getch();
  }

  endwin();
  return 0;
}
