#!/bin/bash
for f in $(ls)
do
  if [[ "${f:0:1}" == 'c' ]]
  then
    cp "$f" "copy_$f"
  fi
done
