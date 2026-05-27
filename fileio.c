// fileio.c
#define _GNU_SOURCE
#include "fileio.h"
#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void editorOpen(const char *filename) {
    free(E.filename);
    E.filename = strdup(filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) return; // File doesn't exist yet, that's fine

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
            linelen--;
        editorInsertRow(E.numrows, line, linelen);
    }
    free(line);
    fclose(fp);
    E.dirty = 0;
}

void editorSave(void) {
    if (E.filename == NULL) return;
    FILE *fp = fopen(E.filename, "w");
    if (!fp) return;

    for (int i = 0; i < E.numrows; i++) {
        fprintf(fp, "%s\n", E.row[i].chars);
    }
    fclose(fp);
    E.dirty = 0;
}