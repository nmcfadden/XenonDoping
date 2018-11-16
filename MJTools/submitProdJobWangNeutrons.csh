#!/bin/csh
# Arguments:
# 1: Depth
# 2: Number of events to simulate.
# 3: Random number seed.
# 4: Run number
#
# Author: R. Henning
# 9-13-2005: Created from submitProdJobNeutrons.csh
#

module load SGE

if($#argv != 4) then
    echo "Wrong number of arguments. 4 required."
    exit
else

echo "Submitting Job..."
set SAVEPATH=/auto/majorana1/users/rhenning/neutrons
set FILENAMEBASE=MJ57WangNeutrons${1}_${4}
set FILE=${SAVEPATH}/${FILENAMEBASE}
set FILESED="\/auto\/majorana1\/users\/rhenning\/neutrons\/${FILENAMEBASE}"

echo "Base filename: " ${FILE}
rm -f ${FILE}.mac
sed "s/@OUTFILE@/${FILESED}.root/g" macros/WangNeutronsTemplate.mac |\
    sed "s/@DEPTH@/${1}/" |\
    sed "s/@NEVENT@/${2}/" |\
    sed "s/@SEED@/${3}/" |\
    sed "s/@DIAGNOSTICSFILE@/${FILESED}_diag.root/"> ${FILE}.mac

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

