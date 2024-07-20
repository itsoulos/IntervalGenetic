#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200
MARGIN=256
./IntervalGenetic  --localmethod=none --intervalmethod=grammar --filename=PROBLEMS/Feature/libFeature.so --param=features --value=2 --param=rightMargin --value=$MARGIN --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS --seed=2 --threads=1 
