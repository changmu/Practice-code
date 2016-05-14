#!/bin/bash

c='gcc -g -Wall -I /home/qiu/github/.lib -L /home/qiu/github/.lib'
${c} cclient.c -o client -la
${c} server.c -o server -la
