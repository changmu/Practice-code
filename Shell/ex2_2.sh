#!/bin/bash

echo "
time and date: `date`
logged-in users: `who`
uptime: `uptime`
" | tee -a out.logfile
