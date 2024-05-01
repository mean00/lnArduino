#!/bin/bash
#set -x
#
if [ "$#" -ne 3 ]; then
    echo "rustgen.sh xxx.h xxx.rs"
    exit 1
fi
ME=$0
export ME=`realpath $ME`
export ME=`dirname $ME`
bash $ME/rustgen_lang.sh $1 $2 c $3
