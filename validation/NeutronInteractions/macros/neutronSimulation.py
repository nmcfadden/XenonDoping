#!/usr/bin/python

import sys
import os
import glob

def readFile(filename):
# opens a file ("filename") and returns the content as a list of strings ("list")
    list = []
    f = open(filename, 'r')
    for line in f:
        list.append(line)
    f.close()
    return list

def writeFile(filename,list):
# creates a file ("filename") from a list of strings ("list")    
    f = open(filename, 'w')
    for i in range(len(list)):
        f.write(list[i])
    f.close()
    

def main():
    energy = readFile('dat/neutronEnergies.txt')
    
    if len(sys.argv)>1:
        element = sys.argv[1]
    else:
        print 'WARNING: no element specified, script terminated'
        sys.exit()
    
    file = 'macros/' + element + '.mac'
    
    datnames = []
    
    for i in range(len(energy)-1): # why -1? This just omits the last energy of 50MeV
        print 'configuring script ' + file + ' for ' + energy[i][:-1] + ' MeV' 
        macro = readFile(file)
        
        # determine number of sub steps
	# n substeps is set in validation.config though it is labeled to the effect of "NI_NumberOfSimulatedInteractions"
        for j in range(len(macro)):
            s = macro[j]
            if s[0] == '#':
                continue
            substring = s.split(' ')
            if substring[0] == '/run/beamOn':
                substeps = float(substring[1]) # substeps
        
	# increment energy through n substeps        
        for k in range(int(substeps)):           
            energySet = float(energy[i]) + k * (float(energy[i+1]) - float(energy[i]))/substeps
            for j in range(len(macro)):
                s = macro[j]
                if s[0] == '#':
                    continue
                substring = s.split(' ')
            	if substring[0] == '/MG/geometry/GDML/sourceFile':
		    magePath = str(os.path.expandvars('$MAGEDIR'))
		    macro[j] = substring[0] + ' ' + magePath + '/validation/NeutronInteractions/macros/' + element + '.gdml\n'		
                if substring[0] == '/MG/eventaction/rootfilename':
		    magePath = str(os.path.expandvars('$MAGEDIR'))
                    macro[j] = substring[0] + ' ' + magePath + '/validation/NeutronInteractions/dat/' + element + '_' + str(energySet) + 'MeV.root\n'
                    datnames.append(str(energySet) + '\tdat/' + element + '_' + str(energySet) + 'MeV.root\n')
                if substring[0] == '/gps/energy':
                    macro[j] = substring[0] + ' ' + str(energySet) + ' MeV\n'
                if substring[0] == '/run/beamOn':
                    macro[j] = substring[0] + ' 10\n' # hard coded: 10 events each substep
                writeFile(file,macro)
            
            #print ' -> executing MaGe simulation {:.1f} MeV'.format(energySet)
            print ' -> executing MaGe simulation (' + str(energySet) + 'MeV) ...'
            os.system('MaGe ' + file + ' > /dev/null')
        
        # restore number of sub steps
        for j in range(len(macro)):
            s = macro[j]
            if s[0] == '#':
                continue
            substring = s.split(' ')
            if substring[0] == '/run/beamOn':
                macro[j] = substring[0] + ' ' + str(int(substeps)) + '\n'
            writeFile(file,macro)

    writeFile('dat/' + element + '_datnames.txt',datnames)

main()
