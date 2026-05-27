// render.c
#include "render.h"
#include "editor.h"
#include <ncurses.h>
#include <string.h>
#include <ctype.h>

void editorUpdateSyntax(erow *row) {
    int in_string = 0;
    for (int i = 0; i < row->size; i++) {
        char c = row->chars[i];
        row->hl[i] = HL_NORMAL;

        if (in_string) {
            row->hl[i] = HL_STRING;
            if (c == '"' && (i == 0 || row->chars[i-1] != '\\')) in_string = 0;
            continue;
        }

        if (c == '"') {
            in_string = 1;
            row->hl[i] = HL_STRING;
            continue;
        }

        if (isdigit(c)) {
            row->hl[i] = HL_NUMBER;
            continue;
        }
        
        if (i <= row->size - 3 && strncmp(&row->chars[i], "int", 3) == 0 && (i==0 || isspace(row->chars[i-1])) && (i+3==row->size || isspace(row->chars[i+3]))) {
            row->hl[i] = row->hl[i+1] = row->hl[i+2] = HL_KEYWORD;
            i += 2;
        }
    }
}

static void editorScroll(void) {
    if (E.cy < E.rowoff) E.rowoff = E.cy;
    if (E.cy >= E.rowoff + E.screenrows) E.rowoff = E.cy - E.screenrows + 1;
    if (E.cx < E.coloff) E.coloff = E.cx;
    if (E.cx >= E.coloff + E.screencols) E.coloff = E.cx - E.screencols + 1;
}

void editorRefreshScreen(void) {
    editorScroll();
    erase();

    for (int y = 0; y < E.screenrows; y++) {
        int filerow = y + E.rowoff;
        if (filerow < E.numrows) {
            // Draw Line Number
            attron(COLOR_PAIR(5));
            mvprintw(y, 0, "%4d ", filerow + 1);
            attroff(COLOR_PAIR(5));

            erow *row = &E.row[filerow];
            editorUpdateSyntax(row);
            int len = row->size - E.coloff;
            if (len > 0) {
                if (len > E.screencols - 5) len = E.screencols - 5;
                for(int i = 0; i < len; i++) {
                    int c_idx = E.coloff + i;
                    int color = row->hl[c_idx];
                    
                    if (color == HL_NUMBER) attron(COLOR_PAIR(1));
                    else if (color == HL_STRING) attron(COLOR_PAIR(2));
                    else if (color == HL_KEYWORD) attron(COLOR_PAIR(3));
                    
                    mvaddch(y, 5 + i, row->chars[c_idx]);
                    
                    if (color == HL_NUMBER) attroff(COLOR_PAIR(1));
                    else if (color == HL_STRING) attroff(COLOR_PAIR(2));
                    else if (color == HL_KEYWORD) attroff(COLOR_PAIR(3));
                }
            }
        } else {
            mvaddch(y, 0, '~'); 
        }
    }

    attron(A_REVERSE);
    char status[80], rstatus[80];
    int len = snprintf(status, sizeof(status), " %.20s - %d lines %s",
        E.filename ? E.filename : "[No Name]", E.numrows, E.dirty ? "(modified)" : "");
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d ", E.cy + 1, E.cx + 1);
    
    mvprintw(E.screenrows, 0, "%s", status);
    for (int i = len; i < E.screencols + 5; i++) {
        if (i == E.screencols + 5 - rlen) {
            printw("%s", rstatus);
            break;
        } else {
            addch(' ');
        }
    }
    attroff(A_REVERSE);

    move(E.cy - E.rowoff, (E.cx - E.coloff) + 5);
    refresh();
}