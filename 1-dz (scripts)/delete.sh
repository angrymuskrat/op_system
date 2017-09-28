#!/bin/bash
for f in $(ls)
do
  if [[ "${f:0:1}" == 'a' ]]
  then
    rm "$f"
  fi
done
