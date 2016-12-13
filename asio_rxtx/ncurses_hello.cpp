/*
 *  Communications Node
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <ncurses.h>

int main(int argc, char** argv)
{

  initscr();                // start curses mode
  printw("Bonjour Monde!"); // print hello world
  refresh();                // print it to real screen
  getch();                  // wait for user input
  endwin();                 // end curses mode

  return 0;
}
