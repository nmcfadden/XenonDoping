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
    energy = readFile('dat/gammaEnergies.txt')
    
    if len(sys.argv)>1:
        element = sys.argv[1]
    else:
        print 'WARNING: no element specified, script terminated'
        sys.exit()
    
    file = 'macros/' + element + '.mac'
    
    for i in range(len(energy)):
        print 'configuring script ' + file + ' for ' + energy[i][:-1] + ' MeV' 
        macro = readFile(file)
        for j in range(len(macro)):
            s = macro[j]
            if s[0] == '#':
                continue
            substring = s.split(' ')
            if substring[0] == '/MG/geometry/GDML/sourceFile':
                magePath = str(os.path.expandvars('$MAGEDIR'))
                macro[j] = substring[0] + ' ' + magePath + '/validation/GammaInteractions/macros/' + element + '.gdml\n'
            if substring[0] == '/MG/eventaction/rootfilename':
		magePath = str(os.path.expandvars('$MAGEDIR'))
		macro[j] = substring[0] + ' ' + magePath + '/validation/GammaInteractions/dat/' + element + '_' + energy[i][:-1] + 'MeV.root\n'
                #macro[j] = substring[0] + ' dat/' + element + '_' + energy[i][:-1] + 'MeV.root\n'
            if substring[0] == '/gps/energy':
                macro[j] = substring[0] + ' ' + energy[i][:-1] + ' MeV\n'
        writeFile(file,macro)
        
        print 'executing MaGe simulation ...'
        os.system('MaGe ' + file + ' > /dev/null')
    
main()
