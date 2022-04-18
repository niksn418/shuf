#!/bin/sh

CMD=$1
shift
for arg do
    STR=`echo $arg | sed -e 's/.*nrange-//' | sed -e 's/.txt.eta//'`
    RANGE=`echo $STR | sed -e 's/[0-9]\{1,\}-//'`
    N=`echo $STR | sed -e 's/-.*//'`
    echo "$CMD --head-count=$N --input-range=$RANGE"
    $CMD --head-count=$N --input-range=$RANGE | sort | diff -u --from-file $arg - || exit 1
done
