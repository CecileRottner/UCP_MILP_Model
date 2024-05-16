#!/bin/bash
rm result.txt

dossier=data/smaller/

n=20
T=24


for sym in 3 ; do
  for id in {1..20}; do
      ./mf 1 $dossier $n $T 1 3 $sym 0 0 $id
  done
done
printf "\n" >> result.txt
printf "\n" >> result.txt

n=20
T=48

for sym in 3 ; do
  for id in {1..20}; do
      ./mf 1 $dossier $n $T 1 3 $sym 0 0 $id
  done
done
printf "\n" >> result.txt
printf "\n" >> result.txt

n=20
T=96

for sym in 3 ; do
  for id in {1..20}; do
      ./mf 1 $dossier $n $T 1 3 $sym 0 0 $id
  done
done
printf "\n" >> result.txt
printf "\n" >> result.txt



dossier=data/Litt_Real/

n=30
T=96

for sym in 3 ; do
  for id in {1..20}; do
      ./mf 1 $dossier $n $T 1 3 $sym 0 0 $id
  done
done
printf "\n" >> result.txt
printf "\n" >> result.txt

n=60
T=48

for sym in 4 3 ; do
  for id in {1..$nbinst}; do
      ./mf 1 $dossier $n $T 1 3 $sym 0 0 $id
  done
done
printf "\n" >> result.txt
printf "\n" >> result.txt

n=60
T=96

for sym in 4 3 ; do
  for id in {1..$nbinst}; do
      ./mf 1 $dossier $n $T 1 3 $sym 0 0 $id
  done
done
printf "\n" >> result.txt
printf "\n" >> result.txt

