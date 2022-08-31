#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200
COUNT=500
rm -f $1.out
for i in {1..10}
do
./IntervalGenetic  --chromosomes=$COUNT --localmethod=genetic --intervalmethod=double --filename=PROBLEMS/NNC/libNNC.so --param=trainName --value=$DATAPATH/$1.train --param=testName --value=$DATAPATH/$1.test --param=nodes --value=$NODES --generations=$GENS  --threads=8 --debug=no --seed=$i >> $1.out
echo "finish $1 iter $i "
done

echo -n "TEST ERROR: "
cat $1.out | grep GEN  | awk '{ sum += $5 } END { if (NR > 0) print sum / NR }'

echo -n "CLASS ERROR: "
cat $1.out |grep GEN | awk '{if (NR!=1) {print substr($6, 1, length($6)-1)}}' | awk '{ sum += $1 } END { if (NR > 0) print sum / NR }'


