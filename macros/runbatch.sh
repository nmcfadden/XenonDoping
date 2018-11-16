#! /bin/sh
#***********************************************
#  runbatch.sh 
#  script to submit Majorana batch jobs on
#  pdsf. The script requires a geant4 macro
#  as intput to MJApp and a seed for the random
#  number generator. It creates some local files
#  that are used for the job. These are put in the
#  tmp directory. These can be deleted when the job 
#  is finished.
#
#  Akbar Mokhtarani, July 27, 2004
#
#***********************************************

#check for the input macro file
if [ -z $1 ]; then
   echo "Please give a macro file name"
   exit 0
fi


macfile=$1
rndseed=$2

cd ..
workdir="`pwd`"

cd macros/

if [ -z $2 ] ; then
 rndseed=1234567452
fi

if [ ! -d tmp ]; then mkdir tmp; fi

#these are temporary files used for this job
tmpfile="tmp/MJmac.$$"
lsfFile="tmp/MJSubmit.$$"

if [ -f $tmpfile ]; then rm $tmpfile; fi
if [ -f $lsfFile ]; then rm $lsfFile; fi

#cp $macfile $tmpfile

sed "s/@SEED@/$rndseed/" $macfile > $tmpfile 
#  |  sed "s/@OUTFILE@/
#pass the seed to the application
#cat >> $tmpfile <<EOF 
#/MJ/management/heprandomseed $rndseed
#EOF

#create the file for batch que
cat > $lsfFile <<EOF 
#! /bin/tcsh 
source /auto/common/majorana/bin/setupMajorana.csh
#printenv
$workdir/bin/MJApp $tmpfile
exit
EOF

chmod +x $lsfFile

#output file
outfile="tmp/batch_output.$$"

#submit the job
bsub -q short -o $outfile $lsfFile 

exit 0
