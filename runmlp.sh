#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=500
## Available values: mse, class, average, squared
FITNESS_METHOD=squared

./IntervalGenetic --chromosomes=500 --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/libnntest.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=10 --fitness_option=$FITNESS_METHOD


