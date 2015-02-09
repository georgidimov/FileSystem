#!/bin/bash
make clean
qmake
make
./tests
