#!/bin/bash
if [ $# -ne 1 ]; then 
  echo "usage: $0 [country]"
  exit;
fi

country=$1
table="table.dat"
> $table
for file in data/*.csv; do
  python parseCSV.py $file $country | tee -a $table
done

root -l plot.C'("'${country}'")'
