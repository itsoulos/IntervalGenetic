#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=500

./IntervalGenetic --chromosomes=500  --localmethod=genetic --intervalmethod=none --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS   2>dd


