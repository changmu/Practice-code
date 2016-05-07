#!/bin/bash

fileName=$1;

if [ -x "$fileName" ]
then
    echo "File exists.";cp $fileName $fileName.bak
else
    echo "File $fileName not found."
fi

echo "File test complete."
