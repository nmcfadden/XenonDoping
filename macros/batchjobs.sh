#! /bin/sh
####
#sample script to submit multiple jobs
#

#set -x
let "indx=1"
RANDOM=22346
while [ $indx -le 5 ]
do
  i=1
  while [ $i -le 15 ]
  do
    let "seed=$RANDOM"
    i=`expr $i + 1`
  done
   let "seed=$RANDOM*$RANDOM"
   echo $seed
   `runbatch.sh Co60.generic.mac  "$seed" `
    indx=`expr $indx + 1`
done

exit 0
