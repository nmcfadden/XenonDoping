#!/bin/csh
# Arguments:
# 1: Energy (MeV)
# 2: Number of events to simulate.
#
# Author: R. Henning
# 3/10/2005
# 4/25/2005 -- Converted to SGE, R. Henning
# 7/19/2005 -- Converted to neutron study, R. Henning
#

module load SGE

if($#argv != 2) then
    echo "Wrong number of arguments. 2 required."
    exit
else

echo "Submitting Job..."
set SAVEPATH=/auto/majorana2/users/rhenning/neutrons/
set FILENAMEBASE=MJ57Neutrons${1}
set FILE=${SAVEPATH}/${FILENAMEBASE}

set FILESED="\/auto\/majorana2\/users\/rhenning\/neutrons\/MJ57Neutrons${1}.root"

echo "Base filename: " ${FILE}
rm -f ${FILE}.mac
sed "s/@OUTFILE@/${FILESED}/g" macros/MJ57BangerNeutronTemplate.mac |\
    sed "s/@ENERGY@/${1}/" |\
    sed "s/@NEVENT@/${2}/" > ${FILE}.mac

rm -f ${FILE}.out ${FILE}.err
qsub <<EOF
#!/bin/csh -v 
#\$ -N ${FILENAMEBASE}
#\$ -e ${FILE}.err 
#\$ -o ${FILE}.out  
#\$ -P majorana
#\$ -cwd
#\$ -hard
#\$ -l h_cpu=23:59:00 
printenv
/auto/u/rhenning/Majorana/code/MaGe/bin/MaGe ${FILE}.mac
EOF

