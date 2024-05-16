#!/bin/bash
rm result.txt

intra=0


dossier=data/smaller/

for y in 1 10 ; do

T=48
for n in 20 30 ; do
for sym in 2 3 ; do
  for id in {1..20}; do
    for met in $(( -1 * $y )) $(( -4 * $y )) $(( -7 * $y )) $(( -2 * $y )) $(( -3 * $y )) ; do	
      ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
    done
    printf "\n" >> result.txt
  done
  printf "\n" >> result.txt    
done
done
printf "\n" >> result.txt
printf "\n" >> result.txt


T=96
n=20

for sym in 2 3 ; do
  for id in {1..20}; do
    for met in $(( -1 * $y )) $(( -4 * $y )) $(( -7 * $y )) $(( -2 * $y )) $(( -3 * $y ))  ; do	
      ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
    done
    printf "\n" >> result.txt
  done
  printf "\n" >> result.txt    
done
printf "\n" >> result.txt
printf "\n" >> result.txt




dossier=data/Litt_Real/

T=96
n=30
for sym in 2 3 ; do
  for id in {1..20}; do
    for met in $(( -1 * $y )) $(( -4 * $y )) $(( -7 * $y )) $(( -2 * $y )) $(( -3 * $y ))  ; do	
      ./mf $met $dossier $n $T 1 3 $sym 0 $intra $id
    done
    printf "\n" >> result.txt
  done
  printf "\n" >> result.txt    
done

printf "\n" >> result.txt
printf "\n" >> result.txt


T=48
n=60

for T in 48 96 ; do
  for sym in 2 3 4 ; do
    for id in {1..20}; do
      for met in $(( -1 * $y )) $(( -4 * $y )) $(( -7 * $y )) $(( -2 * $y )) $(( -3 * $y )) ; do	
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

