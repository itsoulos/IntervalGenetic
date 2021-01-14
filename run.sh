#!/bin/bash
DATAPATH=/home/sheridan/SECDATA/
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
GENS=200

./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_1.train --param=testName --value=$DATAPATH/$1_1.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_2.train --param=testName --value=$DATAPATH/$1_2.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_3.train --param=testName --value=$DATAPATH/$1_3.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_4.train --param=testName --value=$DATAPATH/$1_4.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_5.train --param=testName --value=$DATAPATH/$1_5.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_6.train --param=testName --value=$DATAPATH/$1_6.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_7.train --param=testName --value=$DATAPATH/$1_7.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_8.train --param=testName --value=$DATAPATH/$1_8.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_9.train --param=testName --value=$DATAPATH/$1_9.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
./IntervalGenetic --localmethod=genetic --intervalmethod=none  --filename=PROBLEMS/libfunctionalRbf.so --param=trainName --value=$DATAPATH/$1_10.train --param=testName --value=$DATAPATH/$1_10.test --param=nodes --value=$NODES --generations=$GENS >> $1.out
