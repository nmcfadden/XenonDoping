#!bin/csh -f
#script to setup environment variables for Majorana experiment
#
setenv MJHOME "/auto/common/majorana"
setenv PATH {$PATH}:/auto/common/majorana/bin

#start the ssh-agent
##test -z "$SSH_AUTH_SOCK" && eval `ssh-agent -s`

#setup cvs
#We are now using the cvs server in Munich, AM, 12/6/04 
##setenv CVSROOT cvsserver:/cvs/majorana
setenv CVS_RSH ssh

#this is used in the main makefile to setup the env. var fro pdsf
setenv MAGEENV PDSF

#setup geant
if ( ${?USE_G7} )  then
#echo "using geant4 version 7.0"
source $MJHOME/scripts/geant4Setup_4_7.csh
else if ( ${?USE_G7_1} )  then
source $MJHOME/scripts/geant4Setup_4_7_1.csh
else if ( ${?USE_G7_1_p01} )  then
source $MJHOME/scripts/geant4Setup_4_7_1_p01.csh
else if ( ${?USE_G8_0} )  then
source $MJHOME/scripts/geant4Setup_4_8_0.csh
else
#echo "Using geant4 version 6.1"
source $MJHOME/bin/geant4Setup.csh
endif

module load gcc/gcc-3.2-rh73

#load the SGE module
module load SGE2

#set the shared libraries
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:$CLHEP_LIB_DIR
 
#
# Set up ROOT
#

if ( ${?USE_ROOT_5_06_00} ) then
source $MJHOME/scripts/root5_06_00.csh
else if ( ${?USE_ROOT_4_04_02g} ) then
source $MJHOME/scripts/root4_04_02g.csh
else
source $MJHOME/scripts/root.csh
endif

setenv MANPATH :${MANPATH}

alias getRelease source $MJHOME/scripts/fetchRelease.csh
