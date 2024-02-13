#!/bin/bash
DATAPATH=file:///home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=500
INSTANCES=30
OUTFILE=$1.out
./IntervalGenetic  --localmethod=genetic --intervalmethod=none --filename=PROBLEMS/GenClass/libGenClass.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test  --generations=$GENS  --threads=8 --seed=1 --chromosomes=500
