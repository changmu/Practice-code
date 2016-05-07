#!/bin/bash

ROOT_UID=0
E_NOTROOT=87
if [ "$UID" -ne "$ROOT_UID" ]
then
    echo "You are not root."
    exit $E_NOTROOT
fi
