#!/bin/sh

#script to build classes

if [ -z $1  ]; then
    echo "please provide class name"
    exit 0
fi
CLASS_NAME=$1
AUTHOR=$2

#echo "$1   $2"
if [ -z $AUTHOR  ]; then 
    AUTHOR=`whoami`
fi
CONTACT_NAME=$3
DATE="`date`"

#pakcage name is where we source this script
PKGNAME="`pwd | awk -F/ '{print $NF}'`"

#file names
HeaderFile=$CLASS_NAME.hh
FileName=$CLASS_NAME.cc
#
tempDir="../MJTools/templates"
tempCCfile=$tempDir/MJcodeTemplate.cc
tempHHfile=$tempDir/MJheaderTemplate.hh

thisDir="`pwd`"
incDir="$thisDir/include"
pkgDir="$thisDir/$PKGNAME"
srcDir="$thisDir/src"
if [ ! -d $incDir ]; then
    echo "Creating include directory"
    `mkdir include`
fi
if [ ! -d $srcDir ]; then
    echo "Creating src directory"
    `mkdir src`
fi
if [ ! -d $pkgDir ]; then
    echo "Creating $PKGNAME directory"
    `mkdir $PKGNAME`
fi
#change name to upper case for #ifndef directive
ONCE_NAME=`echo $CLASS_NAME|tr a-z A-Z`
#echo "$ONCE_NAME"


#header (.hh) file
sed "s/@CLASS_NAME@/$CLASS_NAME/g" $tempHHfile |
       sed "s/@ONCE_NAME@/$ONCE_NAME/g" |
       sed "s/@CONTACT@/$CONTACT_NAME/g" |
       sed "s/@AUTHOR@/$AUTHOR/g" | 
       sed "s/@START_DATE@/$DATE/g" > $pkgDir/$HeaderFile

#implementation (.cc) file
sed "s/@CLASS_NAME@/$CLASS_NAME/g" $tempCCfile |
       sed "s/@CONTACT@/$CONTACT_NAME/g" |
       sed "s/@PACKAGE_NAME@/$PKGNAME/g" |
       sed "s/@AUTHOR@/$AUTHOR/g" | 
       sed "s/@START_DATE@/$DATE/g" > $srcDir/$FileName


echo "Created $pkgDir/$HeaderFile and $srcDir/$FileName "

exit 0

