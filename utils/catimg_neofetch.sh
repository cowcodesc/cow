#!/bin/zsh

catimg -t -w 90 -r 2 $1/$(zsh ~/cow/utils/random_filepicker.sh $1) && neofetch --off
