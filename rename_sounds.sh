#!/bin/bash

for file in ./assets/sounds/* ; do
  mv "$file" "${file:0:16}0${file:16}"
done
