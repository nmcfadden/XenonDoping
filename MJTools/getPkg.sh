#!/bin/sh
#########
# script to check out packages from cvs 
# in an existing user workspace
# usage:   getPkg  pkgName tagNo
# where pkgName is the name of existing package in cvs
# and tagNo is the package tag you want to check out
# if tagNo not provided, the tag in the release is used
#
# getPkg database            checks out the package database from
#                            the release   
# getPkg database V00-01-00  checks out V00-01-00 version of
#                            database from cvs
#  
# getPkg management HEAD    checks out the head version
# 
# getPkg all                checks out all packages from the release
#
#  getPkg -f tag.list       checks out packages listed in a file
#                           each line is "packageName  tag"
#
# Akbar Mokhtarani, LBNL Jun 11, 2004
#
#----------------------------------------------------
if [ -z $1  ]; then
    echo "please provide package name"
    exit 0
fi
infile=
readAll="false"

#set -x
#----------------------------------------------------
#get command line options
while getopts f:  c
do
       case $c in
	   f)  infile=$OPTARG
	       continue
	       ;;
       esac
done

#----------------------------------------------------
function Read_file {
    echo "Reading tags from file $infile"
     cd ../
    `cat "$infile" |
         while read pkg  ver   #read package name and tag from infile
         do
           cvs co -r $ver MJ/$pkg 
         done`
    exit
}
#-----------------------------------------------------
#if a file containing list of tags is given read it

if [ -n "$infile" ]; then
    Read_file
fi
#-----------------------------------------------------
workDir=`pwd`
base_="../.baseRel"
baseRel=`cat $base_`
relDir="$MJHOME/builds/$baseRel"
tag=

pkgName=$1

if [ ! -f $base_ ]; then
      echo "This workspace is not based on any release"
      exit 0
fi
#----------------------------------------------------
#### get the tag from the release #####
#set -x
EXCLUDE_DIR="bin lib obj CVS"  #packages to exclude from search
function getTag {
    for x in $relDir/MJ/* ; do
	excl="false"
	echo "$x" > /tmp/pkg.temp  #need to get the package name
	pk=`cat /tmp/pkg.temp |awk -F/ '{print $NF}'`  #this gives package name
	for xf in $EXCLUDE_DIR  ; do
	    if [ "$xf" = "$pk" ]; then excl="true"; fi
	done
          if [ "$excl" = "true" ]; then continue; fi
	if [ "$readAll" = "true" ] && [ -d "$x" ]; then
	    echo "$pk" `cat "$x"/CVS/Tag | cut -c2-` >> $infile
	fi

	if [ $pkgName = "$pk" ] && [ "$readAll" = "false" ] ; then 
	    tag=`cat "$x"/CVS/Tag | cut -c2-`   #get the tag 
	fi
        rm /tmp/pkg.temp
    done
}

#----------------------------------------------------

#check to see if the release exists
if [ ! -d $relDir ]; then
    echo "Release $baseRel does not exits"
    exit 0
fi
#----------------------------------------------------
#if they need all packages in this release 
#create a temporary file to hold the packagename and tags
if [ "$1" = "all" ]; then
    echo "Will checkout all packages from release ..."
    if [ -f /tmp/tag.list ] ; then rm /tmp/tag.list ;fi
    infile=/tmp/tag.list
    readAll="true"
    getTag
    Read_file
#    rm /tmp/tag.list
    exit
fi
#----------------------------------------------------

if [ "$2" != "all" ]; then
if [ -z $2 ]; then    #no tag is given, get it from the release
    getTag
    if [ -z $tag  ] ; then
	echo "Package $pkgName is not in the release"
        exit
    else
        echo "Will checkout $pkgName $tag from release ..." 
        tagNo="$tag"
    fi
else
    echo "Will checkout $pkgName $2 ..."
    tagNo="$2"
fi
fi

cd ../

#checkout the package
cvs co -r $tagNo MJ/$pkgName  

exit 0

