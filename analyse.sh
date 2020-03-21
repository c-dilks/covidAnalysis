#!/bin/bash
if [ $# -eq 0 ]; then 
  echo "usage: $0 [country] [startDay(optional)]"
  exit;
fi

country=$1
startday=0
if [ $# -gt 1 ]; then startday=$2; fi

table="table.dat"
> $table
for file in data/*.csv; do
  python parseCSV.py $file $country | tee -a $table
done

root -l plot.C'("'${country}'",'${startday}')'
