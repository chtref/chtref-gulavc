#!/bin/bash


starttime=$(date +%s)
for run in {1..50}
do
   $ wget http://192.168.6.101/nom=Chtref
done
endtime=$(date +%s)

elapse=$(($endtime - $starttime))
elapse=$($elapse/50)
echo $elapse
read -p "pls"