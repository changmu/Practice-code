#!/bin/bash

scp -P 2121 -i ~qiu/.ssh/id_rsa_2048 checkHardware root@183.60.189.19:/root/qiu
scp -P 2121 -i ~qiu/.ssh/id_rsa_2048 checkHardware root@183.60.189.20:/root/qiu
scp -P 2121 -i ~qiu/.ssh/id_rsa_2048 checkHardware root@183.60.189.21:/root/qiu
