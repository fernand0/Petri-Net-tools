#!/bin/sh


for i in 2 3 5 10 15 20 50 100 ;
do
	echo $i 
	./model_generator $i 3 1 RAS-C-$i.pnml
	./model_generator 2 $i 1 RAS-R-$i.pnml
done

