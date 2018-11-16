#!/bin/csh
# Arguments:
# 1: Region, ie. Tubes
# 2: Location (physical volume), ie. cavityPhysical
# 3: Name of source, ie. Ge68
# 4: N for isotope
# 5: Z for isotope
# 6: Depth for isotope (3rd parameter)
# 7: Number of events to simulate.
#
# Author: R. Henning
# 3/10/2005
# 4/25/2005 -- Converted to SGE, R. Henning
#

module load SGE

if($#argv != 7) then
    echo "Wrong number of arguments. 7 required."
    exit
else

echo "Submitting Job..."
set SAVEPATH=/auto/majorana1/MJ/data/ProposedGeometries/MJ57Banger/${1}
set FILENAMEBASE=${2}${3}
set FILE=${SAVEPATH}/${FILENAMEBASE}

set FILESED="\/auto\/majorana1\/MJ\/data\/ProposedGeometries\/MJ57Banger\/${1}\/${2}${3}.root"

echo "Base filename: " ${FILE}
rm -f ${FILE}.mac
sed "s/@OUTFILE@/${FILESED}/g" macros/MJ57BangerTemplate.mac |\
    sed "s/@VOLUME@/${2}/" |\
    sed "s/@IONN@/${4}/" |\
    sed "s/@IONZ@/${5}/" |\
    sed "s/@IOND@/${6}/" |\
    sed "s/@NEVENT@/${7}/" > ${FILE}.mac

rm -f ${FILE}.out ${FILE}.err
qsub <<EOF
#!/bin/csh -v 
#\$ -N ${FILENAMEBASE}
#\$ -e ${FILE}.err 
#\$ -o ${FILE}.out  
#\$ -P majorana
#\$ -cwd
#\$ -hard
#\$ -l h_cpu=12:00:00 
printenv


/auto/u/rhenning/Majorana/code/MaGe/bin/MaGe ${FILE}.mac
EOF

