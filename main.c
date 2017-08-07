#include <stdio.h>
#include <stdlib.h>
#include "forked.h"

char buf[1000][1000];
int bufsize;
FILE *in;

/* flags */
int display_code;
int display_buffer;

int main(int argc, char **argv)
{
    int i;
    char *fname = 0;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-')
            option(argv[i]);
        else
            fname = argv[i];
    }

    in = fname ? fopen(fname,"r") : stdin;

    int x = 0, y = 0, c;
    while ((c = getc(in)) != EOF)
    {
        if (c == '\n') {
            y++;
            x = 0;
            bufsize++;
        } else {
            buf[y][x] = c;
            x++;
        }
    }

    if (display_buffer)
    {
        for (y = 0; y < bufsize; y++) {
            for (x = 0; buf[y][x]; x++)
                putchar(buf[y][x]);
            puts("");
        }
    }

    int retval = interp();
    fflush(stdout);

    if (retval == 2) {
        fprintf(stderr,"Error 2: went off the edge of the playing field\n");
        return 2;
    }
    else if (retval == 1) {
        fprintf(stderr,"Error 1: invalid direction\n");
        return 1;
    }
    return 0;
}

void option(char *s)
{
    if (!strcmp(s,"--display-code") || !strcmp(s,"-d"))
        display_code = 1;
    else if (!strcmp(s,"--display-buffer") || !strcmp(s,"-b"))
        display_buffer = 1;
}
