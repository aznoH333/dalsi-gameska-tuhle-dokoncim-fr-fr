#!/bin/bash
target_files=""

for f in ./*.c; do
    file_name="${f/.\//}"
    target_files+=" ${file_name/.c/.o}"
    gcc -c $file_name
done

gcc $target_files -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
rm *.o
gdb ./a.out
rm a.out