#!/bin/bash
cc gen.c matrix.c -o gen -O3
./gen 12 12 5 0 > A.txt

