#!/bin/csh
# Arguments:
# 1: Region, ie. Tubes
# 2: Location (physical volume), ie. cavityPhysical
# 3: Name of source, ie. Ge68
# 4: N for isotope
# 5: Z for isotope
# 6: Number of events to simulate.
# 7: (optional) Save only sensitive volume hits ('true' or 'false' (default))
# 8: (optional) Kill alpha tracks outside sensitive volume (default: false)
# 9: (optional) Kill beta tracks outside sensitive volume (default: false)
# 10: (optional) Stop nuclei after one step outside sensitive volume (default: false)
#
# Author: R. Henning
# 3/10/2005
# 4/25/2005 -- Converted to SGE, R. Henning
# 1/2/2006 -- Added random seed.
# 1/5/2006 -- Removed random seed in lieu of /dev/random implenentation. R.Henning
# 8/21/2006 -- Added options 8-10. Made 7 optional. R. Henning
#

module load SGE

if($#argv < 7 || $#argv > 10) then
    echo "Wrong number of arguments. 7 to 10 required."
    exit
else

set SENSWRITE=false
set KILLALPHAS=false
set KILLBETA=false
set STOPNUCLEI=false
if($#argv > 6) then
  set SENSWRITE=${7}
    if($#argv > 7) then
      set KILLALPHAS=${8}
	if($#argv > 8) then
	  set KILLBETAS=${9}
	    if($#argv > 9) then
	      set STOPNUCLEI=${10}
	    endif
	endif
    endif
endif

echo "Submitting Job..."
set SAVEPATH=/auto/majorana1/MJ/data/ProposedGeometries/ReferenceDesign/${1}
set FILENAMEBASE=${2}${3}
set FILE=${SAVEPATH}/${FILENAMEBASE}
    
set FILEROOT="\/auto\/majorana1\/MJ\/data\/ProposedGeometries\/ReferenceDesign\/${1}\/${2}${3}.root"
set DIRROOT="\/auto\/majorana1\/MJ\/data\/ProposedGeometries\/ReferenceDesign\/${1}"
set FILESED="\/scratch\/rhenning\/${2}${3}.root"
    
echo "Base filename: " ${FILE}
rm -f ${FILE}.mac
sed "s/@OUTFILE@/${FILESED}/g" macros/RDTemplate.mac |\
	sed "s/@VOLUME@/${2}/" |\
	sed "s/@IONN@/${4}/" |\
	sed "s/@IONZ@/${5}/" |\
	sed "s/@IOND@/0/" |\
	sed "s/@SENSWRITE@/${7}/" |\
	sed "s/@NEVENT@/${6}/"  |\
        sed "s/@KILLALPHAS@/${KILLALPHAS}/"  |\
        sed "s/@KILLBETAS@/${KILLBETAS}/"  |\
        sed "s/@STOPNUCLEI@/${STOPNUCLEI}/" > ${FILE}.mac
	
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
#\$ -l scratchfree=3000M
printenv
mkdir /scratch/rhenning
/home/rhenning/Majorana/code/MaGe/bin/MaGe ${FILE}.mac abort
rm /home/rhenning/Majorana/code/MaGe/core.*
mv /scratch/rhenning/*.root ${DIRROOT}
EOF
    
endif
