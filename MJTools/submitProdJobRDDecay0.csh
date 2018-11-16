#!/bin/csh
# Arguments:
# 1: Region, ie. Tubes
# 2: Location (physical volume), ie. cavityPhysical
# 3: Name of source, ie. Ge68
# 4: Filename with Decay0 vertices
# 5: Save only sensitive volume hits ('true' or 'false')
# 6: Number of events (should be < # in arg 4)
#
# Author: R. Henning
#
# 6-22-2006, Created from submitProdJobRD.csh
#

module load SGE

if($#argv != 6) then
    echo "Wrong number of arguments. 6 required."
    exit
else

echo "Submitting Job..."
set SAVEPATH=/auto/majorana1/MJ/data/ProposedGeometries/ReferenceDesign/${1}
set FILENAMEBASE=${2}${3}
set FILE=${SAVEPATH}/${FILENAMEBASE}

set FILEROOT="\/auto\/majorana1\/MJ\/data\/ProposedGeometries\/ReferenceDesign\/${1}\/${2}${3}.root"
set DIRROOT="\/auto\/majorana1\/MJ\/data\/ProposedGeometries\/ReferenceDesign\/${1}"
set FILESED="\/scratch\/rhenning\/${2}${3}.root"

echo "Base filename: " ${FILE}
rm -f ${FILE}.mac
sed "s/@OUTFILE@/${FILESED}/g" macros/RDDecay0Template.mac |\
    sed "s/@VOLUME@/${2}/" |\
#    sed "s/@DECAYFNAME@/${4}/" |\
    sed "s/@SENSWRITE@/${5}/" |\
    sed "s/@NEVENT@/${6}/" > ${FILE}.mac

rm -f ${FILE}.out ${FILE}.err
qsub <<EOF
#!/bin/csh -v 
#\$ -N ${FILENAMEBASE}
#\$ -e ${FILE}.err 
#\$ -o ${FILE}.out  
#\$ -P majorana
#\$ -cwd
#\$ -hard
#\$ -l h_cpu=24:00:00 
#\$ -l scratchfree=4000M
printenv
mkdir /scratch/rhenning
rm /scratch/rhenning/*
cp ${4} /scratch/rhenning/decay0.dat
/home/rhenning/Majorana/code/MaGe/bin/MaGe ${FILE}.mac abort
rm /home/rhenning/Majorana/code/MaGe/core.*
mv /scratch/rhenning/*.root ${DIRROOT}
rm -rf /scratch/rhenning
EOF

