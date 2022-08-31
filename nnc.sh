#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200
COUNT=500
./IntervalGenetic  --chromosomes=$COUNT --localmethod=none --intervalmethod=double --filename=PROBLEMS/NNC/libNNC.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=8 --debug=yes --seed=$i 



