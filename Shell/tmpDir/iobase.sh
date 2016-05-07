#!/bin/bash
#convert base

no=100
echo "obase=2; $no" | bc
no=1000
echo "obase=10; ibase=2; $no" | bc
