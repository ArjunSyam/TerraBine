#include <ncurses.h>

int main() {
    initscr();            // Start curses mode
    start_color();        // Enable color functionality 
    init_pair(2, COLOR_WHITE, COLOR_BLUE);   // White text, blue background

    // Create a window
    WINDOW *win = newwin(10, 40, 5, 5);
    wbkgd(win, COLOR_PAIR(2));                // Set window background color
    wattron(win, COLOR_PAIR(2));              // Set text color to red

    wprintw(win, "Hello, ncurses with colors!");
    wrefresh(win);                            // Show changes on screen

    getch();    // Wait for user input to exit

    endwin();   // End curses mode
    return 0;
}
