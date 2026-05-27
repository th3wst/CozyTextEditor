#include "input.h"
#include "editor.h"
#include "fileio.h"
#include <ncurses.h>

int editorProcessKeypress(void) {
    int c = getch();

    switch (c) {
        case CTRL_KEY('q'):
        case CTRL_KEY('x'):
        case 27:            //ESC key
            return 0; //sig to quit

        case CTRL_KEY('s'):
            editorSave();
            break;

        case KEY_UP:
            if (E.cy != 0) E.cy--;
            break;
        case KEY_DOWN:
            if (E.cy < E.numrows) E.cy++;
            break;
        case KEY_LEFT:
            if (E.cx != 0) E.cx--;
            else if (E.cy > 0) { E.cy--; E.cx = E.row[E.cy].size; }
            break;
        case KEY_RIGHT:
            if (E.cy < E.numrows) {
                if (E.cx < E.row[E.cy].size) E.cx++;
                else if (E.cx == E.row[E.cy].size) { E.cy++; E.cx = 0; }
            }
            break;

        case KEY_BACKSPACE:
        case 127: 
        case '\b':
            editorDelChar();
            break;

        case '\n':
        case KEY_ENTER:
            editorInsertNewline();
            break;

        case '\t':
            for(int i=0; i<4; i++) editorInsertChar(' ');
            break;

        case KEY_RESIZE:
            getmaxyx(stdscr, E.screenrows, E.screencols);
            E.screenrows -= 1; 
            E.screencols -= 5; 
            break;

        default:
            //insert printable characters
            if (c >= 32 && c < 127) {
                editorInsertChar(c);
            }
            break;
    }

    if (E.cy < E.numrows && E.cx > E.row[E.cy].size) {
        E.cx = E.row[E.cy].size;
    }

    return 1;
}