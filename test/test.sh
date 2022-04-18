#!/bin/sh

CMD=$1
shift
for arg do
    $CMD $arg | sort | diff -u --from-file ${arg}.eta - || exit 1
done
