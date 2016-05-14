#!/bin/bash

c='gcc -g -Wall -I /home/qiu/github/.lib -L /home/qiu/github/.lib'
${c} client.c -o client -la
${c} server.c -o server -la
