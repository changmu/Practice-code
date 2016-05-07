#!/bin/bash

ls . | xargs -i cp ./{} $1
