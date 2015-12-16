#!/bin/bash
start=$(date +%s)
find /usr/include -type f -name *.c -print
end=$(date +%s)
echo Used $((end-start)) seconds.
