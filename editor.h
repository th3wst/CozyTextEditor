#ifndef EDITOR_H
#define EDITOR_H

#include <stddef.h>

//represents a single row of text
typedef struct {
    int size;
    char *chars;
    unsigned char *hl; //array for syntax highlighting colors
} erow;

//global editor state
typedef struct {
    int cx, cy;         //cursor x and y position within the file
    int rowoff;         //row scroll offset
    int coloff;         //column scroll offset
    int screenrows;     //terminal rows (excluding status bar)
    int screencols;     //terminal columns
    int numrows;        //total rows in document
    erow *row;          //arrray of rows
    int dirty;          //modified flag
    char *filename;     //current file path
} editorConfig;

extern editorConfig E;

void initEditor(void);
void editorInsertRow(int at, const char *s, size_t len);
void editorFreeRow(erow *row);
void editorDelRow(int at);
void editorRowInsertChar(erow *row, int at, int c);
void editorRowDelChar(erow *row, int at);
void editorInsertChar(int c);
void editorInsertNewline(void);
void editorDelChar(void);

// Constants
#define CTRL_KEY(k) ((k) & 0x1f)
#define HL_NORMAL 0
#define HL_NUMBER 1
#define HL_STRING 2
#define HL_KEYWORD 3
#define HL_COMMENT 4

#endif