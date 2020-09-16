#!/bin/bash

function failTest() {
  echo "***********************************"
  echo -e "   $f  \tFAILED!"
  echo "***********************************"
  exit 1
}

chmod +x bin/*
i=1
for f in bin/*
do
  "$f" || failTest "$f"
done
