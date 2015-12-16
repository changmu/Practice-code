#!/bin/bash
F() {
	echo $1
	sleep 1
	F $1
}
F hello
