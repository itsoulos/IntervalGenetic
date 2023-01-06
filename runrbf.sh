#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/constructed/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/NEWBETTER/
NODES=10
GENS=100

./IntervalGenetic --chromosomes=200  --localmethod=genetic --intervalmethod=pso --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=8


