#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/COVID/
NODES=10
GENS=200

./IntervalGenetic  --localmethod=genetic --intervalmethod=none --filename=PROBLEMS/Feature/libFeature.so --param=features --value=2 --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=32



