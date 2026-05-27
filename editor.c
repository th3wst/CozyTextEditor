#include "editor.h"
#include <stdlib.h>
#include <string.h>

editorConfig E;

void initEditor(void) {
    E.cx = 0; E.cy = 0;
    E.rowoff = 0; E.coloff = 0;
    E.numrows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.filename = NULL;
}

void editorInsertRow(int at, const char *s, size_t len) {
    if (at < 0 || at > E.numrows) return;
    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
    memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));

    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';
    E.row[at].hl = malloc(len);
    memset(E.row[at].hl, HL_NORMAL, len);

    E.numrows++;
    E.dirty++;
}

void editorFreeRow(erow *row) {
    free(row->chars);
    free(row->hl);
}

void editorDelRow(int at) {
    if (at < 0 || at >= E.numrows) return;
    editorFreeRow(&E.row[at]);
    memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
    E.numrows--;
    E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c) {
    if (at < 0 || at > row->size) at = row->size;
    row->chars = realloc(row->chars, row->size + 2);
    row->hl = realloc(row->hl, row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->chars[at] = c;
    row->size++;
    E.dirty++;
}

void editorInsertChar(int c) {
    if (E.cy == E.numrows) {
        editorInsertRow(E.numrows, "", 0);
    }
    editorRowInsertChar(&E.row[E.cy], E.cx, c);
    E.cx++;
}

void editorRowDelChar(erow *row, int at) {
    if (at < 0 || at >= row->size) return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    E.dirty++;
}

void editorDelChar(void) {
    if (E.cy == E.numrows || (E.cx == 0 && E.cy == 0)) return;
    erow *row = &E.row[E.cy];
    if (E.cx > 0) {
        editorRowDelChar(row, E.cx - 1);
        E.cx--;
    } else {
        E.cx = E.row[E.cy - 1].size;
        //append current row to previous row
        erow *prev_row = &E.row[E.cy - 1];
        int new_size = prev_row->size + row->size;
        prev_row->chars = realloc(prev_row->chars, new_size + 1);
        memcpy(&prev_row->chars[prev_row->size], row->chars, row->size);
        prev_row->chars[new_size] = '\0';
        prev_row->size = new_size;
        editorDelRow(E.cy);
        E.cy--;
    }
}

void editorInsertNewline(void) {
    if (E.cx == 0) {
        editorInsertRow(E.cy, "", 0);
    } else {
        erow *row = &E.row[E.cy];
        editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
        row = &E.row[E.cy]; //reassign in case realloc moved memory
        row->size = E.cx;
        row->chars[row->size] = '\0';
    }
    E.cy++;
    E.cx = 0;
}