# DMC (diagram MaxSAT Solver)
DMC computes the answer to a generalized MaxSAT problem using algebraic decision diagrams

<!-- ####################################################################### -->

## Installation (Linux)

### Prerequisites
- automake 1.16
- cmake 3.16
- g++ 9.3
- gmp 6.2
- make 4.2
- already included as git submodules:
  - [cudd 3.0](https://github.com/ivmai/cudd)
  - [cxxopts 2.2](https://github.com/jarro2783/cxxopts)
  - [sylvan 1.5](https://github.com/trolando/sylvan)

### Command
```bash
make dmc
```

<!-- ####################################################################### -->

## Examples

### Showing command-line options
#### Command
```bash
./dmc
```
#### Output
```
Diagram Model Counter (reads join tree from stdin)
Usage:
  dmc [OPTION...]

      --cf arg  cnf file path; string (REQUIRED)
      --wc arg  weighted counting: 0, 1; int (default: 0)
      --pc arg  projected counting: 0, 1; int (default: 0)
      --pw arg  planner wait duration (in seconds), or 0 for first join tree only; float (default: 0)
      --dp arg  diagram package: c/CUDD, s/SYLVAN; string (default: c)
      --tc arg  thread count, or 0 for hardware_concurrency value; int (default: 1)
      --ts arg  thread slice count [with dp_arg = c]; int (default: 1)
      --rs arg  random seed; int (default: 0)
      --dv arg  diagram var order: 0/RANDOM, 1/DECLARED, 2/MOST_CLAUSES, 3/MINFILL, 4/MCS, 5/LEXP,
                6/LEXM (negative for inverse order); int (default: 4)
      --sv arg  slice var order [with dp_arg = c]: 0/RANDOM, 1/DECLARED, 2/MOST_CLAUSES, 3/MINFILL,
                4/MCS, 5/LEXP, 6/LEXM, 7/BIGGEST_NODE, 8/HIGHEST_NODE (negative for inverse order); int
                (default: 7)
      --ms arg  mem sensitivity (in MB) for reporting usage [with dp_arg = c]; float (default: 1e3)
      --mm arg  max mem (in MB) for unique table and cache table combined; float (default: 4e3)
      --tr arg  table ratio [with dp_arg = s]: log2(unique_size/cache_size); int (default: 1)
      --ir arg  init ratio for tables [with dp_arg = s]: log2(max_size/init_size); int (default: 10)
      --mp arg  multiple precision [with dp_arg = s]: 0, 1; int (default: 0)
      --lc arg  log counting [with dp_arg = c]: 0, 1; int (default: 0)
      --jp arg  join priority: a/ARBITRARY_PAIR, b/BIGGEST_PAIR, s/SMALLEST_PAIR; string (default: s)
      --vc arg  verbose cnf: 0, 1, 2; int (default: 0)
      --vj arg  verbose join tree: 0, 1, 2; int (default: 0)
      --vp arg  verbose profiling: 0, 1, 2; int (default: 0)
      --vs arg  verbose solving: 0, 1, 2; int (default: 1)
      --mx arg  MaxSAT solving: 0, 1; always set to one when solving MaxSAT problems. int (default: 0)
      --mb arg  Upper Bound of cost for ADD pruning: int (default: inf)
```

### Computing weighted projected model count given cnf file and join tree from jt file
#### Command
```bash
./dmc --cf=../examples/pbtest.wbo --mx=1 <../examples/phi.jt
```
#### Output
```
c processing command-line options...
c cnfFile                     ../benchmarks/examples/pbtest.wbo
c weightedCounting            0
c projectedCounting           0
c planningStrategy            FIRST_JOIN_TREE
c diagramPackage              CUDD
c threadCount                 1
c threadSliceCount            1
c randomSeed                  0
c diagramVarOrder             MCS
c sliceVarOrder               BIGGEST_NODE
c memSensitivityMegabytes     1000
c maxMemMegabytes             4000
c logCounting                 0
c joinPriority                SMALLEST_PAIR

c processing cnf formula...
c trivial bound: 300

c procressing join tree...
c getting join tree from stdin (end input with 'enter' then 'ctrl d')
c number of variables: 7
c number of constraints: 7
c processed join tree ending on line 29
c joinTreeWidth               3
c plannerSeconds              -inf
c getting join tree from stdin: done

c computing output...
c diagramVarSeconds           0
c thread slice counts: { 1 }
c sliceWidth                  3
c threadMaxMemMegabytes       4000
c thread    1/1 | assignment    1/1: {  }
c thread    1/1 | assignment    1/1 | seconds 0.03       | mx 0
c maxsat LB under partial assignment 0
c numNodes 1
o 0
c ------------------------------------------------------------------
c s type maxsat
c s exact double prec-sci 0
c ------------------------------------------------------------------
c seconds                     0.031

```
The optimal cost is zero indicates that this instance is actually satisfiable.

### Computing weighted projected model count given cnf file and join tree from planner
#### Command
```bash
cnfFile="../examples/pbtest.wbo" && ../lg/build/lg "../lg/solvers/flow-cutter-pace17/flow_cutter_pace17 -p 100" <$cnfFile | ./dmc --cf=$cnfFile --mx=1
```
