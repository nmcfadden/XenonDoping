#! /bin/csh -f  
#script to setup cvs repository for Majorana user
#This assumes that the user name on cvs.nersc.gov machine is the
#same as the machine this script is running from
#
set fileName="$HOME/.ssh/id_mj_cvs.rsa"
#delete the files if they exits
if ( -f "$fileName" )  then
    rm "$fileName"
endif
set pubFile=$fileName.pub
if ( -f $pubFile )  then
    rm $pubFile
endif
set configFile="$HOME/.ssh/config"
if ( -f $configFile )  then
    rm $configFile
endif
set myName=`whoami`
set passP
#
`ssh-keygen -t rsa  -f "$fileName" -P "$passP"    >&  /dev/null`
#echo $status
#if (  "$status" != "0" ) then
#    echo Could not generate the keys
#    exit(0)
#endif
echo Copying public key to cvs.ners.gov
`scp "$pubFile" $myName@cvs.nersc.gov:.ssh/authorized_keys2 `
#
#create the config file
cat > $configFile <<EOF
Host 		cvsserver
HostName 	cvs.nersc.gov
User 		$myName
IdentityFile 	~/.ssh/id_mj_cvs
Protocol	2,1
EOF
#
#set cvs environment variables
setenv CVSROOT cvsserver:/cvs/majorana
setenv CVS_RSH ssh
#
exit(0)
