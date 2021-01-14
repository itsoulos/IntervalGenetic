#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/constructed/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/SECDATA/
NODES=10
GENS=200

  ./IntervalGenetic  --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/functionalRbf.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=32


