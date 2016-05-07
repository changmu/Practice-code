#!/bin/bash

LOG_DIR=tmpDir
cd $LOG_DIR
cat /dev/null > messages
cat /dev/null > wtmp

echo "Logs cleaned up."

exit
