# LG

## Description

A tool for using tree decompositions to construct join trees.

## Prerequisites

* make
* g++
* boost (graph and system), which can be installed by:
```bash
sudo apt-get install libboost-all-dev
```

## Installation

LG can then be built with the following command:
```bash
make
```

FlowCutter, a tree decomposer, can be built with the following command:
```bash
make -C solvers/flow-cutter-pace17
```

## Running

Once LG and FlowCutter have been built, LG can be run with the following command:
```bash
build/lg "./solvers/flow-cutter-pace17/flow_cutter_pace17 -s 1234567 -p 100" <../examples/pbtest.wbo
```

On this command, output is:
```
c pid 13915
c num clauses = 7
c number of variables 7
c min degree heuristic
c outputing bagsize 3
p jt 7 7 12
8 7 e 7
9 3 5 4 e 3 4
10 9 1 2 e 1 2
11 6 e 6
12 8 10 11 e 5
c joinTreeWidth 3
c seconds 0.0129699
=
c status 3 1631418184753
c min shortcut heuristic
c run with 0.0/0.1/0.2 min balance and node_min_expansion in endless loop with varying seed
^C
```

Note that LG is an anytime algorithm, so it prints multiple join trees to STDOUT, separated by '='.
The process ID of the tree-decomposition solver is given in the first comment line (`c pid`),
which can be used to kill the tree-decomposition solver.
