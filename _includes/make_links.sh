#!/bin/bash
set -e

dirname="$(for i in $(seq 1 255); do echo -n "d"; done)"
for i in $(seq 1 17); do
  mkdir $dirname
  ln -s $dirname d
  cd $dirname
done
