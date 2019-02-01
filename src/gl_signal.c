#include <stdbool.h>
#include <stdio.h>

#include "signal.h"

/* define an extern variable to control the game runtime */
extern bool gl_quitgame;

/**
 * gl_quit_handler is the function responsible
 * to handle a CTRL^C on the Game Of Life.
 */
void
gl_quit_handler(int signal)
{
    fprintf(stdout, "\n\nThanks for 'playing' with me. Bye!\n");
    gl_quitgame = true;
}
