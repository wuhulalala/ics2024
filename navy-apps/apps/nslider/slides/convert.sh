#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_file.pdf>"
    exit 1
fi

input_file="$1"  # 

convert "$input_file" \
  -sharpen "0x1.0" \
  -type truecolor -resize 400x300\! slides.bmp

mkdir -p $NAVY_HOME/fsimg/share/slides/
rm $NAVY_HOME/fsimg/share/slides/*
mv *.bmp $NAVY_HOME/fsimg/share/slides/
