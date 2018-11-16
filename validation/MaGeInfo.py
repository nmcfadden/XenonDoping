#!/usr/bin/env python

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
    MaGeOutput = readFile('MaGeInfo.txt')
    GlobalParameter = readFile('globalparameter.txt')
    
    keyword = ['MaGe Revision','MaGe SVN Tag','MaGe Last Updated','MGDO Revision','MGDO SVN Tag','MGDO Last Updated','CLHEP Version',' Geant4 version Name']
    token = ['XX:MaGeRevision:XX','XX:MaGeSVNTag:XX','XX:MaGeLastUpdated:XX','XX:MGDORevision:XX','XX:MGDOSVNTag:XX','XX:MGDOLastUpdated:XX','XX:CLHEPVersion:XX','XX:Geant4VersionName:XX']
    
    for i in range(len(MaGeOutput)):
        s = MaGeOutput[i]
        substring = s.split(':')
        for j in range(len(keyword)):
            if substring[0] == keyword[j]:
                GlobalParameter.append(token[j] + ' ' + substring[1])
    
    writeFile('globalparameter.txt',GlobalParameter)

main()
