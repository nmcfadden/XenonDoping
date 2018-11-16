###!bin/sh -f 
#script to setup cvs repository for Majorana user
#This assumes that the user name on cvs.nersc.gov machine is the
#same as the machine this script is running from

fileName="$HOME/.ssh/id_mj_cvs.rsa"
pubFile=$fileName.pub
configFile="$HOME/.ssh/config"
myName=`whoami`
##exit(0)
passP=

`ssh-keygen -t rsa  -f "$fileName" -P "$passP"`
`scp "$pubFile" cvs.nersc.gov:.ssh/authorized_keys2`

#create the config file
cat > $configFile <<EOF
Host 		cvsserver
HostName 	cvs.nersc.gov
User 		$myName
IdentityFile 	~/.ssh/id_mj_cvs
Protocol	2,1
EOF

#set cvs environment variables
CVSROOT=cvsserver:/cvs/majorana
CVS_RSH=ssh
export CVSROOT
export CVS_RSH
exit 0
