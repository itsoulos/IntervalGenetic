#!/bin/bash
DATAPATH=file:///home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200
echo ./IntervalGenetic  --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/GenClass/libGenClass.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --generations=$GENS  --threads=8
./IntervalGenetic  --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/GenClass/libGenClass.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test  --generations=$GENS  --threads=8
