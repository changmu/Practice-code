#!/bin/bash

Number_of_expected_args=2
E_WRONG_ARGS=85
script_parameters="-a -h -m -z"

if [ $# -ne $Number_of_expected_args ]
then
    echo "Usage: `basename $0` $script_parameters"
    exit $E_WRONG_ARGS
fi
