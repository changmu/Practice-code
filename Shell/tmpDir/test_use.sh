#!/bin/bash

read num
declare -i sum=0
for ((i=1; i<=$num; i++))
do
    sum=$(($sum+$i))
done
echo $sum
