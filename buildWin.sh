#!/bin/bash
target_files=


for f in ./*.c; do
    file_name="${f/.\//}"
    gcc -c $file_name
    target_files="${target_files} ${file_name/.c/.o}"

done
#echo $target_files
gcc -o game.exe $target_files -lraylib -lgdi32 -lwinmm
rm *.o
./game.exe
#rm *.exe
