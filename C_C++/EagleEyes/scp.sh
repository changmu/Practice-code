#!/bin/bash

scp -P 2121 -i ~qiu/.ssh/id_rsa $1 root@183.60.189.19:/root/qiu
scp -P 2121 -i ~qiu/.ssh/id_rsa $1 root@183.60.189.20:/root/qiu
scp -P 2121 -i ~qiu/.ssh/id_rsa $1 root@183.60.189.21:/root/qiu

# bj04
# scp -P 2121 $1  test@106.38.222.165:/home/test
# cd02phy
# scp -P 2121 $1 qiu@222.211.90.67:/home/qiu