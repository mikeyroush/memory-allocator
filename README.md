# memory-allocator

This project implements a buddy-system memory manager that allocates memory in blocks with sizes that are power-of-two multiples of a basic block size. you can specify the basic block size and total block size with `-b ` and `-s` flags respectively.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for testing purposes.

### Prerequisites

A working command line

### Installing and Running

Download the zip then link the files in the command line via the makefile and run the executable

Navigate to the correct directory and utilize the makefile

```console
foo@bar:~$ cd /path/to/directory/here
foo@bar:~$ make all
```

This will have created a few .o files and an executable. To run it, do as follows...

```console
foo@bar:~$ ./memtest -b 128 -s 2048
```

Demo

```console
128:
0
256:
0
512:
0
1024:
0
2048: 	2048
1

allocating block a
wanted: 212, new: 256
128:
0
256: 	256
1
512: 	512
1
1024: 	1024
1
2048:
0

allocating block b
wanted: 300, new: 512
128:
0
256: 	256
1
512:
0
1024: 	1024
1
2048:
0

freeing block a
128:
0
256:
0
512: 	512
1
1024: 	1024
1
2048:
0

freeing block b
128:
0
256:
0
512:
0
1024:
0
2048: 	2048
1
```

## Authors

* **Michael Roush** - *Project completion*

## License

Copyright © 2017 Michael Roush. All rights reserved.

