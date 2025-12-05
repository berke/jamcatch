#!/bin/sh

exec picocom --baud 38400 /dev/ttyS1 $@
