#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
DATAPATH=/home/sheridan/Desktop/ERGASIES/NFT/bin/
NODES=10
GENS=20
echo ./IntervalGenetic  --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/Feature/libFeature.so --param=features --value=2 --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=16
./IntervalGenetic  --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/Feature/libFeature.so --param=features --value=2 --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=8



