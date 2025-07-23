#!/bin/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$1/applib
cd $1
exec "./""$2"

echo $1
echo $2