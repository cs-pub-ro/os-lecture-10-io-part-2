#!/bin/sh

if test $# -ne 1; then
    echo "Usage: $0 <num_clients>" 1>&2
    exit 1
fi

num_clients="$1"

for i in $(seq -f "%04g" 0 "$num_clients"); do
    ./client &
done

wait
