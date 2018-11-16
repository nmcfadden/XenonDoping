#!/usr/bin/python

import sys
import os
import glob

def main():
    os.system('DecayEngine macros/Co60.mac dat/Co60.root > /dev/null')
    os.system('DecayEngine macros/Rn219.mac dat/Rn219.root > /dev/null')
    
    os.system('../RadioactiveDecays/DecayAnalysis dat/Co60.dat dat/Co60.root > /dev/null')
    os.system('../RadioactiveDecays/DecayAnalysis dat/Rn219.dat dat/Rn219.root > /dev/null')
    
    os.system('./MergeParameters.py Co60parameter.txt')
    os.system('./MergeParameters.py Rn219parameter.txt')
        
    os.system('rm -f dat/Co60.root')
    os.system('rm -f Co60parameter.txt')
    os.system('rm -f dat/Rn219.root')
    os.system('rm -f Rn219parameter.txt')    
    
main()
