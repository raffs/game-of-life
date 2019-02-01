# Game of Life - implemented in C Language

This a simple implementation of the Game of Life (also known as Conway's Game of Life).

## Why ?

I thought it will be a good idea to share this beginner and easy implementation of the Game Of Life in C.

## Compilation

To compile the software you can run the `compile` script located on the `bin` directory 

```sh
$ bin/compile
```
OBS: The `bin/compile` script use the GCC compiler

## Execution

By default the Game of Life uses a `20x20` grid, but it can be changed through the command-line
arguments. On the following example the program is executed with 30x30 grid.

```sh
$ ./game-of-life      # Use the 20x20 organism size

$ ./game-of-life 30   # Use a 30x30 organism size
```

## Rules

The universe of the Game of Life is an infinite, two-dimensional orthogonal grid
of square cells, each of which is in one of two possible states, alive or dead, 
(or populated and unpopulated, respectively). Every cell interacts with its eight 
neighbours, which are the cells that are horizontally, vertically, or diagonally 
adjacent. At each step in time, the following transitions occur:

1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

The initial pattern constitutes the seed of the system. The first generation is created by applying 
the above rules simultaneously to every cell in the seed; births and deaths occur simultaneously, 
and the discrete moment at which this happens is sometimes called a tick. Each generation is a 
pure function of the preceding one. The rules continue to be applied repeatedly to create 
further generations.

Source from wikipedia: [Wikipedia: Conway's Game Of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).
