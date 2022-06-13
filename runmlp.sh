#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/COVID/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/NEWBETTER/
NODES=10
GENS=200

./IntervalGenetic  --localmethod=adam --intervalmethod=none --filename=PROBLEMS/libnntest.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=1


