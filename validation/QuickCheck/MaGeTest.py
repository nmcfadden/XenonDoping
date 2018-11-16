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
    MaGeOutput = readFile('MaGeInfo.txt')
    GlobalParameter = readFile('quickcheckparameter.txt')
    
    GlobalParameter.append('XX:Errors:XX \n')
    GlobalParameter.append('XX:Warnings:XX \n')
    
    keyword = ['MaGe Revision','MaGe SVN Tag','MaGe Last Updated','MGDO Revision','MGDO SVN Tag','MGDO Last Updated','CLHEP Version',' Geant4 version Name']
    token = ['XX:MaGeRevision:XX','XX:MaGeSVNTag:XX','XX:MaGeLastUpdated:XX','XX:MGDORevision:XX','XX:MGDOSVNTag:XX','XX:MGDOLastUpdated:XX','XX:CLHEPVersion:XX','XX:Geant4VersionName:XX']
    value = ['','','','','','','','']
    
    for i in range(len(MaGeOutput)):
        s = MaGeOutput[i]
        substring = s.split(':')
        for j in range(len(keyword)):
            if substring[0] == keyword[j]:
                value[j] = substring[1]
                GlobalParameter.append(token[j] + ' ' + value[j])
    
    # create error messages    
    errors = ''
    for i in range(len(GlobalParameter)):
        s = GlobalParameter[i]
        substring = s.split(' ')
        if substring[0] == 'XX:Errors:XX':
            errors = s
    
    if len(MaGeOutput) < 1:
        errors = errors + '{\\color{Red}Unable to execute MaGe. Please check the MaGe and {\\sc Geant4} installation.\\\\}'
    if len(value[6]) == 0:
        errors = errors + '{\\color{Red}Unable to determine CLHEP version. Please check CLHEP installation.\\\\}'
                
    for i in range(len(GlobalParameter)):
        s = GlobalParameter[i]
        substring = s.split(' ')
        if substring[0] == 'XX:Errors:XX':
            errors = errors.replace('\n', '') + '\n'
            GlobalParameter[i] = errors 

    # create warning messages    
    warnings = ''
    for i in range(len(GlobalParameter)):
        s = GlobalParameter[i]
        substring = s.split(' ')
        if substring[0] == 'XX:Warnings:XX':
            warnings = s
    
    if value[0] != value[3]:
        warnings = warnings + '{\\color{BurntOrange}Different revisions of MaGe (' + value[0] + ') and MGDO (' + value[3] + ') detected. Please update to the same revision.}\\\\'
        
    for i in range(len(GlobalParameter)):
        s = GlobalParameter[i]
        substring = s.split(' ')
        if substring[0] == 'XX:Warnings:XX':
            warnings = warnings.replace('\n', '') + '\n'
            GlobalParameter[i] = warnings 
        
    writeFile('quickcheckparameter.txt',GlobalParameter)

main()
