#!/usr/bin/python

import sys
import os

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
    
    if len(sys.argv)>1:
        filename = sys.argv[1]
    else:
        print 'WARNING: no file specified, script terminated'
        sys.exit()
    
    LocalParameter = readFile(filename)
    GlobalParameter = readFile('quickcheckparameter.txt')
    
    for i in range(len(LocalParameter)):
        GlobalParameter.append(LocalParameter[i])
    
    writeFile('quickcheckparameter.txt',GlobalParameter)

main()
