#!/bin/sh
#stwm status.sh example file

TIMING=10

statustext()
{
    # Syntax : status <bar name> <data>
    # possible sequences as data: \s[] \R[] \i[]
    stwm -c status "default `date`"
}

while true;
do
    statustext
    sleep $TIMING
done
