#!/bin/bash

: ${1? "Usage: `basename $0` command"}
type $1 &> /dev/null
if [ $? == 0 ]
then
    echo "exists"
else
    echo "not exists"
fi
