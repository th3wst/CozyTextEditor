#include "editor.h"
#include "fileio.h"
#include "render.h"
#include "input.h"
#include <ncurses.h>

void init_curses(void) {
    initscr();
    raw();                  // Disable line buffering and interrupt signals (Ctrl+C, Ctrl+Z)
    noecho();               // Do not echo typed keys
    keypad(stdscr, TRUE);   // Enable arrow keys and F-keys
    start_color();
    use_default_colors();
    
    // Setup syntax highlight colors
    init_pair(1, COLOR_RED, -1);     // Numbers
    init_pair(2, COLOR_GREEN, -1);   // Strings
    init_pair(3, COLOR_MAGENTA, -1); // Keywords
    init_pair(5, COLOR_YELLOW, -1);  // Line Numbers

    getmaxyx(stdscr, E.screenrows, E.screencols);
    E.screenrows -= 1; // Room for status bar
    E.screencols -= 5; // Room for line numbers
}

int main(int argc, char *argv[]) {
    initEditor();
    init_curses();

    if (argc >= 2) {
        editorOpen(argv[1]);
    }

    while (1) {
        editorRefreshScreen();
        if (!editorProcessKeypress()) {
            break;
        }
    }

    endwin(); // Restore terminal settings
    return 0;
}