#!/bin/bash

if [ "$1" ]; then
  DIRECTORY="$1"
else
  echo "cd: missing operand"
  exit 1
fi

if [ -d "$DIRECTORY" ]; then
  cd "$DIRECTORY"
else
  echo "cd: $DIRECTORY: No such directory"
  exit 1
fi
