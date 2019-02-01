#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "gl_signal.h"

/* define states variable */
#define DEAD_CELL 0
#define ALIVE_CELL 1

/**
 * Declare the gl_quitgame extern variable (from src/gl_signal.c)
 * and define as false.
 */
bool gl_quitgame = false;

/**
 * Organism structure holds the organism cells, lenght
 * and the amount of alive and dead cells.
 */
struct organism_t
{
    uint8_t **cells;       /* the cells of this organism */
    uint32_t length;       /* the length of the array */
    uint32_t dead_cells;   /* count the dead cells */
    uint32_t alive_cells;  /* count the alive cells */
};


/**
 * Given a bidimension grid, print the grid representing
 * the alive and dead cells. 
 */
void
organism_print(struct organism_t *organism)
{
    fprintf(stdout, "Game of life\n");
    fprintf(stdout, "  This organism can die of 'natural order' or");
    fprintf(stdout, "  you can kill it using CTRL^C\n\n");

    for (int i = 0; i < organism->length; i++) {
        fprintf(stdout, "\t"); /* Do a tab for each line */

        for (int j = 0; j < organism->length; j++) {
            if (organism->cells[i][j] == DEAD_CELL)
                fprintf(stdout, " - ");
            else
                fprintf(stdout, " X ");
        }

        fprintf(stdout, "\n"); /* new line */
    }

    fprintf(stdout, "\n\n"); /* double new line */
}


/**
 * Initialize the organism cells states randomly.
 */
void
organism_init(struct organism_t *organism, uint32_t length)
{
    srand(time(NULL));

    /* Initialize the organism values */
    organism->length = length;
    organism->cells  = (uint8_t **) malloc(length * sizeof(uint8_t *));

    for (int i = 0; i < length; i++) {
        organism->cells[i] = (uint8_t *) malloc(length * sizeof(uint8_t));

        /* initialize the cells with zero */
        for (int j = 0; j < length; j++) {
            if (rand() % 5 == 0)
                organism->cells[i][j] = ALIVE_CELL;
            else
                organism->cells[i][j] = DEAD_CELL;
        }
    }

    return;  
}


/**
 * Free an organism structure from memory and avoid memory leak.
 */
void
organism_free(struct organism_t *organism)
{
    for (int i = 0; i < organism->length; i++) {
        if (organism->cells[i] != NULL)
            free(organism->cells[i]);
    }

    return;
}

/**
 * Update the dead and alive cells on the organism
 */
void
organism_update_count(struct organism_t *organism)
{
    int dead_cells = 0, alive_cells = 0;

    for (int i = 0; i < organism->length; i++) {
        for (int j = 0; j < organism->length; j++) {
            if (organism->cells[i][j] == DEAD_CELL)
                dead_cells += 1;
            else
                alive_cells += 1;
        }
    }

    /* update current counters */
    organism->dead_cells  = dead_cells;
    organism->alive_cells = alive_cells;

    return;
}

/**
 * Compute one lifecycle of the organism. The return value is 
 * encoded as follows:
 *
 *   (n < 0) => Negeative number defines an error occur
 *   (n = 0) => Non-cells was changed
 *   (n > 0) => 'N' cells was whether killed or born.
 */
int
organism_lifecycle(struct organism_t *organism)
{
    /* init vars */
    int is_skip;
    int rcode = 0;
    uint16_t dead_cells, live_cells;
    uint16_t row_prev, row_next, col_prev, col_next;

    /* iterate over each cell space */
    for (int i = 0; i < organism->length; i++) {
        for (int j = 0; j < organism->length; j++) {

            /* initialize */
            dead_cells = 0, live_cells = 0;

            /* count the previous and next neighbors position */
            row_prev = (i - 1), row_next = (i + 1);
            col_prev = (j - 1), col_next = (j + 1);

            /* counter the current cells neighbors */
            for (int in = row_prev; in <= row_next; in++) {
                for (int jn = col_prev; jn <= col_next; jn++) {

                    /*
                     * Skip the counting when the cell itself, and the
                     * there's no neighbors
                     */
                    is_skip = (in == i && jn == j)
                        || (in < 0 || in >= organism->length)
                        || (jn < 0 || jn >= organism->length);

                    if (is_skip)
                        continue;
                    else {
                        if (organism->cells[in][jn] == DEAD_CELL)
                            dead_cells += 1;
                        else
                            live_cells += 1;
                    }
                }
            }

            /*
             * Rule 1: Any live cell with fewer than two live neighbors dies, 
             *         as if by population.
             *
             * Rule 2: Any live cell with two or three live neighbors lives on
             *         the next generation.
             *
             * Rule 3: Any live cell with more then three live neighbors dies,
             *         as if by overpopulation.
             *
             * Rule 4: Any dead cell with exactly three live nighbors becomes,
             *         a live cell, as if by reproduction.
             */
            if (organism->cells[i][j] == DEAD_CELL) {
                if (live_cells == 3) {
                    rcode += 1;
                    organism->cells[i][j] = ALIVE_CELL; /* cells is alive due to rule 4 */
                }
            }
            else {
                if (live_cells < 2) {
                    rcode += 1;
                    organism->cells[i][j] = DEAD_CELL; /* cells is dead due to rule 1 */
                }
                else if (live_cells > 3) {
                    rcode += 1;
                    organism->cells[i][j] = DEAD_CELL; /* cells is dead due to rule 3 */ 
                }

                /* when no previous rule is applied, cells continues to live rule 2 */
            }
        }
    }

    /* update the organism counter */
    organism_update_count(organism);

    return rcode;
}

/**
 * ----------------------------------------------------------------------------
 *  Main script starts here
 * ----------------------------------------------------------------------------
 */
int
main(const int argc, char* argv[])
{
    int organism_length = 20; /* the square length of the organism */
    int changed; /* how many cells changed, returned from organis_lifecycle fun */

    if (argc == 2) {
        organism_length = atoi(argv[1]);
    }

    /* define CTRL^C handler */
    signal(SIGINT, gl_quit_handler);

    /* initialize a organism structure */
    struct organism_t organism;
    organism_init(&organism, organism_length);

    /* Iterate a few Organism's lifecycle to see their interaction */
    for (long long i = 0; gl_quitgame == false; i++) {

        printf("\e[1;1H\e[2J"); /* clear screen first */

        organism_print(&organism);
        if ((changed = organism_lifecycle(&organism)) == 0) {
            fprintf(stdout, "No more changed on the cell, quitting the game\n");
            break;
        }
        else if (changed < 0) {
            fprintf(stderr, "Something went wrong during the lifecycle\n");
            break;
        }
        else {
            fprintf(stdout, "Summary:\n");
            fprintf(stdout, "  %lld Cycles (year) of this organims\n", i);
            fprintf(stdout, "  %d Alive cells\n", organism.alive_cells);
            fprintf(stdout, "  %d Dead cells\n", organism.dead_cells);
            fprintf(stdout, "  %d Items changed\n", changed);
        }

        usleep(100000); /* wait 500 ms to the next round, because we humans are not so fast */
    }

    /* free the memory object to prevent memory-leak */
    organism_free(&organism);

    /* that's all folks */
    return 0;
}
