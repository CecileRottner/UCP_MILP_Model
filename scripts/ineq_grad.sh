#!/bin/bash
rm result.txt

intra=0

dossier=data/Litt_Real/

n=60
T=96
y=10

  for sym in 3 ; do
    for id in {15..20}; do
      for met in $(( -1 * $y )) $(( -4 * $y )) $(( -7 * $y )) $(( -2 * $y )) $(( -3 * $y )) ; do	
        ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
      done
      printf "\n" >> result.txt
    done
    printf "\n" >> result.txt    
  done



  for sym in 4 ; do
    for id in {1..20}; do
      for met in $(( -1 * $y )) $(( -4 * $y )) $(( -7 * $y )) $(( -2 * $y )) $(( -3 * $y )) ; do	
        ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
      done
      printf "\n" >> result.txt
    done
    printf "\n" >> result.txt    
  done


printf "\n" >> result.txt
printf "\n" >> result.txt

done

