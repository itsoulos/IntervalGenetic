#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=100

./IntervalGenetic --chromosomes=200  --localmethod=genetic --intervalmethod=grammar --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=8


