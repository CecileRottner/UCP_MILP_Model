#!/bin/bash
rm result.txt

intra=0

dossier=data/GenData/

T=48
n=60

for n in 60 ; do
for T in 48 ; do
  for sym in 3 ; do
    for id in {1..20}; do
      for met in 0 1 ; do	
        ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
      done
      printf "\n" >> result.txt
    done
    printf "\n" >> result.txt    
  done
done
done

printf "\n" >> result.txt
printf "\n" >> result.txt

