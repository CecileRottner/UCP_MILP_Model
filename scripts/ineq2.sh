#!/bin/bash
rm result.txt

intra=0

for y in 1 ; do

dossier=data/Litt_Real/



T=48
n=60

for T in 96 ; do
  for sym in 4 ; do
    for id in {1..20}; do
      for met in $(( -4 * $y )) $(( -5 * $y )) ; do	
        ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
      done
      printf "\n" >> result.txt
    done
    printf "\n" >> result.txt    
  done
done


printf "\n" >> result.txt
printf "\n" >> result.txt

done

