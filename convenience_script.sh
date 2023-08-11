#!/bin/bash

while(true)
do
 echo 'Tip: exit picocom with ctrl + a + x'
 echo 'b - build ; f - flash ; p - picocom ; q - quit'
 echo '>'
 read -r cmd
 case $cmd in
  b)
   make
  ;;
  f)
   make flash
  ;;
  p)
   picocom -b 2000000 /dev/ttyUSB0
  ;;
  q)
   exit
  ;;
 esac
done
