#!/bin/bash
DATAPATH=file:///home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=500
MARGIN=512
ORIGINAL=1
CHROMOSOMES=500
./IntervalGenetic  --localmethod=none --intervalmethod=none --filename=PROBLEMS/GenClass/libGenClass.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test  --generations=$GENS  --threads=1 --seed=1 --chromosomes=$CHROMOSOMES --param=isOriginal --value=$ORIGINAL --param=rightmargin --value=$MARGIN
