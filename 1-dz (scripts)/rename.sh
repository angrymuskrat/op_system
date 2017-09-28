#!/bin/bash
for f in $(ls)
do
  if [[ "${f:0:1}" == 'b' ]]
  then
    mv "$f" "renamed_$f"
  fi
done
