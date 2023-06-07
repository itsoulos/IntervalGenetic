#!/bin/bash
DATAPATH=file:///home/itsoulos/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200
INSTANCES=30
OUTFILE=$1.out
rm -f $OUTFILE
for i in $(seq 1 $INSTANCES)
do
./IntervalGenetic  --localmethod=genetic --intervalmethod=integer --filename=PROBLEMS/GenClass/libGenClass.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test  --generations=$GENS  --threads=8 --seed=$i >>$OUTFILE
done
