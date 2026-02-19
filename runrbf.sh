#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=500
## Available values: mse, class, average, squared
FITNESS_METHOD=average

./IntervalGenetic --chromosomes=500  --localmethod=genetic --intervalmethod=genetic --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --fitness_option=$FITNESS_METHOD 2>dd


