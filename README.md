# DPMS (Dynamic Programming for Generalized MaxSAT)

DPMS handles generalized MaxSAT problems in an extended DIMACS format (described below)
- The DPMS framework runs in two phases:
  - Planning phase: [LG](./lg) constructs a (graded) project-join tree of a generalized MaxSAT formula
  - Execution phase: [DMC](./dmc) computes the answer to a generalized MaxSAT formula using the (graded) project-join tree

## Installation (Linux)
- automake 1.16
- cmake 3.16
- g++ 9.3
- gmp 6.2
- make 4.2
- already included as git submodules:
  - [cudd 3.0](https://github.com/ivmai/cudd) (a slightly modified version for DPMS is inlcuded. Needs to be compiled manually, see below)
  - [cxxopts 2.2](https://github.com/jarro2783/cxxopts) (included)
  - [sylvan 1.5](https://github.com/trolando/sylvan)(included)

### Compile CUDD (ADD supporter)
In addmc/libraries/cudd, run

	./INSTALL.sh

### Compile LG (Tree Builder)
In lg/, run

	make

For more information, see [here](lg/README.md)

### Compile DMC (Executor)
In dmc/, run 

	make dmc

For more information, see [here](dmc/README.md)

## Usage Example (Command Line)
	cnfFile="examples/hybrid.hwcnf" && lg/build/lg "lg/solvers/flow-cutter-pace17/flow_cutter_pace17 -p 100" < $cnfFile | dmc/dmc --cf=$cnfFile --mx=1

Make sure to use "--mx=1" to enable maxSAT.

Use the option "--mb=BOUND" to give an upper bound (int) of optimal cost (e.g., the result of o-line of a MaxSAT solver) for ADD pruning. For example,
	
	cnfFile="examples/hybrid.hwcnf" && lg/build/lg "lg/solvers/flow-cutter-pace17/flow_cutter_pace17 -p 100" < $cnfFile | dmc/dmc --cf=$cnfFile --mx=1 --mb=60000

For a WBO or partial MaxSAT instance, --mb is set to be the trivial bound which can be read from the instance, unless the user gives a better bound.

## Benchmarks for evaluations of IJCAI-22 submission

Please see the directory benchmarks\_results

## Problem format of Generalized MaxSAT
Some examples of each type of problem can be found in examples/

### (generalized) MaxSAT and weighted MaxSAT
The Max-CNF-SAT problems (.cnf) should use the DIMACS format: https://www.ieee.org/conferences/publishing/templates.html

For XOR constraints, use 'x' at the beginning of a line 

	x1 xor x2 xor \neg x3 => x 1 2 -3 0.

For weighted MaxSAT (.cnf), use "p wcnf nvars nclauses total-Soft-Weight" instead of "p cnf nvars nclauses" in header. For each clause line, put the weight at the beginning of a line, then the first literal.

DPMS also accepts the hybrid weighted MaxSAT format (.hwcnf), take exapmles/hybrid.hwcnf for an example:

	p hwcnf 7 8 100
	[3] +1 x1 +1 x2 >= 1 ;
	[2] -1 x1 -1 x2 >= -1 ;
	[10] -1 x3 +1 x2 >= 0 ;
	[9] -1 x3 +1 x4 >= 0 ;
	[12] +1 x3 -1 x2 -1 x4 >= -1 ;
	[34] -1 x5 +1 x6 >= 0 ;
	[15] -1 x5 +1 x7 >= 0 ;
	[7] x 1 2 3 4 0

In a .hwcnf file, weights are always in front of each constraint, wrapped by '[]'. Each constraint after the weight can be a CNF clause, XOR or a pseudo-Boolean constraint.

### Pseudo-Boolean optimization (WBO)
For PB constraints (.wbo), here is an example 

	+1 x1 +1 x2 >= 1 ;
	[90] -1 x1 -1 x2 >= -1 ;

The first constraint is a hard constraint. The second constraint is soft with weight 90.

### Min-MaxSAT
A Min-MaxSAT problem file is same with a MaxSAT file except that there is a 'vm' line indicating the min variables. Variables that do not appear in the vm line are all max variables.
