#!/bin/bash
DATAPATH=file:///home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
GENS=200
MARGIN=4096
ORIGINAL=0
CHROMOSOMES=500
./IntervalGenetic  --localmethod=none --intervalmethod=double --filename=PROBLEMS/GenClass/libGenClass.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test  --generations=$GENS  --threads=1 --seed=1 --chromosomes=$CHROMOSOMES --param=isOriginal --value=$ORIGINAL --param=rightmargin --value=$MARGIN
