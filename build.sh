#!/bin/bash
rm a.out
cc game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./a.out
