#!/bin/csh  -f
#########
#  script to create a new release for user based
#  on an existing realease
#  usage:   getRelease  releaseNo
#  where releaseNo is an existing release no
#
#Akbar Mokhtarani, LBNL Nov 28, 2005
#
#+-----------------------------------------------------------

if ( $#argv == 0  ) then
    echo "The following releases are available:"
#    exit 0
    foreach dir  ( $MJHOME/builds/* ) 
      if ( -d $dir && $dir != "$MJHOME/builds/templates" ) then
	      cat > tmpf <<EOF
	      $dir
EOF

              set relName=`awk -F/ '{print $NF}' tmpf`
	      if ( $relName == "prod" ) then
		  echo "prod -->" `ls -l $dir | awk '{print $NF}'`
	      else
		  echo  $relName
	      endif
         rm tmpf
      endif
     end
    echo "Please choose one of these to run this script"
    echo "Example: getRelease MaGe-01.00.01"
    if ( -f tmpf ) then 
	rm tmpf
    endif
    exit 0
endif

set RELEASENO = $1
##userRelDir="MJ_$RELEASENO"   #local dir to create new release

#echo "user release dir is: $userRelDir"

set relDir = "$MJHOME/builds/$1"

if ( ! -d $relDir ) then
      echo "Release $1 doesn't exist"
      echo "Run 'getRelease' command with no arguments to get a list of available releases"
      exit 0
endif

#copy the release to here

echo "Copying release $RELEASENO to `pwd`/$RELEASENO"

cp -r $relDir .

#setup env variables for this release

echo "setting up the environment variables"

set envfile = $RELEASENO/buildTools/Rel_spec_pdsf.csh

if ( -f $envfile ) then
    source ./$envfile
else
    echo "File $envfile does not exist"
endif
#create a file to hold the release number

#cat > ".baseRel" <<EOF
#$RELEASENO
#EOF

echo "Done..."

exit 0

