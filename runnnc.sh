#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200
COUNT=500
./IntervalGenetic  --chromosomes=$COUNT --localmethod=none --intervalmethod=integer --filename=PROBLEMS/NNC/libNNC.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=1 --debug=yes --seed=$i --param=rightmargin --value=256 --param=chromosomesize --value=100



