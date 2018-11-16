#!/bin/sh
#########
#  script to create a new release for user based
#  on an existing realease
#  usage:   getRel  releaseNo
#  where releaseNo is an existing release no
#
#  e.g.  getRel 1.0
#   this creates  an MJ_1.0 directory and installs the 
#   MJ directory under it. It also creates a file  .baseRel
#   in MJ_1.0  that contains the release number. DONOT DELETE THIS FILE
#
#Akbar Mokhtarani, LBNL Jun 11, 2004
#
#+-----------------------------------------------------------
if [ -z $1  ]; then
    echo "The following releases are available:"
#    exit 0
    for dir in $MJHOME/builds/*; do
      if [ -d $dir && $dir != "$MJHOME/builds/templates" ]; then
	      cat > tmpf <<EOF
	      $dir
EOF

              relName=`awk -F/ '{print $NF}' tmpf`
	      if [ "$relName" = "prod" ]; then
		  echo "prod -->" `ls -l $dir | awk '{print $NF}'`
	      else
		  echo  $relName
	      fi
         rm tmpf
      fi
     done
    echo "Please choose one of these to run this script"
    echo "Example: getRelease MaGe-01.00.01"
    if [ -f tmpf ]; then rm tmpf; fi
    exit 0
fi

RELEASENO=$1
##userRelDir="MJ_$RELEASENO"   #local dir to create new release

#echo "user release dir is: $userRelDir"

relDir="$MJHOME/builds/$1"

if [ ! -d $relDir ]; then
      echo "Release $1 doesn't exist"
      exit 0
fi

#copy the release to here

echo "Copying release $RELEASENO to here..."

`cp -r $relDir .`

#setup env variables for this release

echo "setting up the environment variables"

envFile=$RELEASENO/buildTools/Rel_spec_pdsf.sh

if [ -f $envFile ]; then
    `source $envFile`
else
    echo "File $envFile does not exist"
fi
#create a file to hold the release number

#cat > ".baseRel" <<EOF
#$RELEASENO
#EOF

echo "Done..."

exit 0

