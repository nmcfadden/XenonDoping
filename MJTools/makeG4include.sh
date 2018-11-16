#!/bin/ksh
#script to copy geatn 4 header files to $MJHOME/include 
#directory. 
#Akbar Mokhtarani, LBNL, March-25-2004

G4INSTALL=/usr/local/pkg/Geant4/6.0.p01/geant4.6.0.p01
G4_INCLUDE=/auto/common/majorana/include

rm $G4_INCLUDE/*.hh
rm $G4_INCLUDE/*.icc

#move to source directory
cd $G4INSTALL/source


#this function recursively moves through directories
#under the geant4 source directory and copy include files 
#to the above directory

############## do_it ############
function do_it {
#set -x
    for file in *
    do

        if [ -d "$file"  ]          #if file is a directory  
        then   
 	 if [ "$file" = "include" ]
	 then
	 echo "copying from $file to $G4_INCLUDE "
#	 cp  $file/* $G4_INCLUDE/
	 cp  $file/*.hh $G4_INCLUDE/
	 cp  $file/*.h $G4_INCLUDE/
	 cp  $file/*.inc $G4_INCLUDE/
	 cp  $file/*.icc $G4_INCLUDE/
	 else
          cd $file                  #cd into it 
	  do_it           #and call this function again
          fi           
        fi
    done                            #done with this directory
          cd ..                     #and move one directory up
}

do_it

exit
