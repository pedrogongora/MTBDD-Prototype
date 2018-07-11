# MTBDD-Prototype

Prototype C++ implementation of multi-terminal BDD node-table \[2\] and symbolic Bellman-Ford for graph games \[1\].

## About

A *graph game* is a generalization of a [game tree](https://en.wikipedia.org/wiki/Extensive-form_game) for finite, non-cooperative, complete and perfect information games. In short, a graph game is a directed and weighted graph, where each node corresponds to a players turn, and weights are vectors representing the utilities for all players. The number of players and nodes are always finite.

In this setting, a [shortest path](https://en.wikipedia.org/wiki/Shortest_path_problem) from one node to another is also a generalization of a [Nash equilibrium](https://en.wikipedia.org/wiki/Nash_equilibrium), and thus, any shortest path algorithm, such as Bellman-Ford, can compute a game Nash equilibrium.

## What's implemented

The code in [src/arrayimpl](src/arrayimpl) implements the MTBDD node table. Only the basic BDD operations are implemented (see the file [src/arrayimpl/ApplyOps.h](src/arrayimpl/ApplyOps.h)).

The code in [src/booleanmodel](src/booleanmodel) builds game graph BDDs from state-transition boolean equations.

Take for exmple, the following two equations:
```
x1 := x1 & ~x2 : [1.0, 2.0];
x2 := x1 | x2  : [5.0, 4.0];
```
The first equation can be read as: a transition from states where `x1` is `true` and `x2` is `false` to states where `x1` is `true`, has utility `1.0` for player 1, and `2.0` for player 2.
The second equation can be read as: a transition from states where either `x1` or `x2` is `true` to states where `x2` is `true`, has utility `5.0` for player 1, and `4.0` for player 2.

Each equation contributes additively, in this example, the two equations build the BDD of the following graph:
<img src='https://g.gravizo.com/svg?%20digraph%20G%20%7B%0A%20%2000%20%5Bpos%3D%220%2C0!%22%5D%3B%0A%20%2001%20%5Bpos%3D%221%2C0!%22%5D%3B%0A%20%2010%20%5Bpos%3D%220%2C1!%22%5D%3B%0A%20%2011%20%5Bpos%3D%221%2C1!%22%5D%3B%0A%20%2001%20-%3E%2011%20%5Blabel%3D%225%2C4%22%5D%3B%0A%20%2010%20-%3E%2001%20%5Blabel%3D%225%2C4%22%5D%3B%0A%20%2010%20-%3E%2011%20%5Blabel%3D%226%2C6%22%5D%3B%0A%20%2011%20-%3E%2001%20%5Blabel%3D%225%2C4%22%5D%3B%0A%20%7D%0A%20%20%20%20%20%20%20%20%20%20%20%20'>

Players turns are assigned in order (of appereance) of each variable to each state. In the example, states`00` and `10` are player 1 turn, and states`01` and `11` are player 2 turn.

## Compiling
For compiling you need a a C++ compiler and [cmake](https://cmake.org/).

 Clone/download and unpack the source code to you computer and run the following:
```
cd MTBDD-Prototype/build
cmake ..
make 
```

The executable `MTBDD-Proto` takes as an argument the path of a text file containing boolean equations, parses the file, build a BDD, computes the shortest path from *all* states to the last state (i.e., the state where all variables are `true`), and prints the results.

## License

GNU General Public License v. 2

----
1. Góngora PA and Rosenblueth DA. [A symbolic shortest path algorithm for computing subgame-perfect Nash equilibria](http://www.degruyter.com/view/j/amcs.2015.25.issue-3/amcs-2015-0043/amcs-2015-0043.xml). International Journal of Applied Mathematics and Computer Science. 25:3. 2015.
2. Meinel, C. and Theobald, T. (1998). Algorithms and Data Structures in VSLI Design: OBDD-Foundations and Applications, Springer-Verlag, Berlin.
