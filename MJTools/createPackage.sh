#!/bin/sh
#########
#script to create a new package in cvs for Majorana
# experiment
# usage:   ./createPackage  pkgName
#
#Akbar Mokhtarani, LBNL Jun 2, 2004
#
if [ -z $1  ]; then
    echo "please provide package name to create"
    exit 0
fi

PKG_NAME=$1
tempDir="/tmp/MJTmp"
#set +x
if [ ! -d $tempDir ]; then
      `mkdir $tempDir`
fi

#echo $tempDir
cd $tempDir

cvs co MJ/MJTools/pkgTemplate

cd $tempDir/MJ/MJTools/pkgTemplate
cvs import -m"intital package creation" MJ/"$PKG_NAME" V00-00-00 start
 
echo "Created package $PKG_NAME "
#exit 0
if [  -d $tempDir ]; then
     `rm -rf $tempDir `
fi

exit 0

