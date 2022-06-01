#!/bin/bash
gcc -g -Wall main.c mp4.c -o stamps
gcc -g -Wall data.c -o rand
while true; do
    ./rand > input.in
    ./gold `cat input.in` > std.out
    ./stamps `cat input.in` > me.out
    cat input.in
    cat me.out
    cat std.out
    diff std.out me.out
    if [ $? -ne 0 ] ; then 
        echo WA; break ;
    fi ; 
        echo AC;
    echo -e \\n
    sleep 1s
done
