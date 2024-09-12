#!/bin/zsh

dir_path=$1
echo $(ls $dir_path) $(jot -r 1 $(ls $dir_path | wc -l) 1) | awk '{print $($NF)}' -
