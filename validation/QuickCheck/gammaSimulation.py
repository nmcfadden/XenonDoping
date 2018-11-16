#!/usr/bin/python

import sys
import os
import glob

def main():
    os.system('MaGe macros/GeGamma.mac > /dev/null')
    os.system('../GammaInteractions/GammaInteractionAnalysis Ge > /dev/null')
    os.system('./MergeParameters.py Geparameter.txt')

    os.system('rm -f dat/Ge_2MeV.root')
    os.system('rm -f Geparameter.txt')
    
main()
