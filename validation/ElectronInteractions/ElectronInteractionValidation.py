#!/usr/bin/env python

from multiprocessing import Pool
import multiprocessing
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

def runSimulation(t):
    
    dictionary = {}
    token = ['EI_NumberOfSimulatedInteractions','EI_RunSimulation','EI_RelativeComparison','RC_Version', 'RC_newVersion']
    content = readFile('../validation.config')
    
    # creating dictionary of MaGe validation config file  
    for i in range(len(content)):
        s = content[i]
        if s[0] == '#':
            continue
        substring = s.split(' ')
        for j in range(len(token)):
            if substring[0] == token[j]:
                dictionary[token[j]] = substring[1]   
    
	element = t[7:].replace('.mac','')
    
    if dictionary[token[1]][0] == '1':
        print '+++ executing \'' + element + ' Decay\' simulation +++'
        os.system('macros/electronSimulation.py ' + element)
        if os.path.exists('dat/' + element + '.dat') == True:
            print '+++ analysing \'' + element + ' Decay\' data +++'
            os.system('./ElectronInteractionAnalysis ' + element + ' ' + dictionary[token[3]][:-1] + ' ' + dictionary[token[4]][:-1] + ' > /dev/null')
            print '+++ generating report file for ' + element + '+++'
            os.system('../RGen ' + element + 'parameter.txt ' + element + 'template.tex ' + element + 'report.tex' + '>/dev/null')
            # delete temporary ROOT files
            os.system('rm -f dat/' + element + '*MeV.root')
        else:
            print '*** file ' + 'dat/' + element + '.dat not available! ***'

if __name__ == '__main__':
    pool = Pool(processes=multiprocessing.cpu_count())
    pool.map(runSimulation, glob.glob('macros/*.mac'))
