#!bin/sh -f
#script to setup environment variables for Majorana experiment
#
MJHOME="/auto/common/majorana"
export MJHOME

#set PATH={$PATH}:$MJHOME/bin
PATH={$PATH}:/auto/common/majorana/bin

#start the ssh-agent
##test -z "$SSH_AUTH_SOCK" && eval `ssh-agent -s`

#setup cvs 
#We are now using the cvs server in Munich, AM, 12/6/04 
##CVSROOT=cvsserver:/cvs/majorana
CVS_RSH=ssh
##export CVSROOT
export CVS_RSH

#this is used in the main makefile to setup the env. var for pdsf
export MAGEENV=PDSF

#setup geant
source $MJHOME/bin/geant4Setup.sh

#setup Redhat 7.3
##/usr/local/bin/rh73

#####module load gcc/gcc-3.2-rh73
module load SGE2

#set the shared libraries
##LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/pkg/CLHEP/1.8.1.0/lib/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/auto/common/majorana/simu/CLHEP/lib/

#
# Set up ROOT
#
##ROOTSYS=/auto/atlas/sw/root/v3.05.04/rh73_gcc32/root
#ROOTSYS=/usr/local/pkg/ROOT/v3.05.04/rh73_gcc32/root

ROOTSYS=/auto/common/majorana/simu/root

PATH=$PATH:$ROOTSYS/bin
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

export LD_LIBRARY_PATH
export PATH
export ROOTSYS
 
alias getRelease='source $MJHOME/scripts/fetchRelease.sh'
