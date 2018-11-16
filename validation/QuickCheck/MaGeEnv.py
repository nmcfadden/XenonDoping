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
    MaGeOutput = readFile('MaGeEnv.txt')
    GlobalParameter = readFile('quickcheckparameter.txt')
    
    keyword = ['G4ABLADATA','G4INSTALL','G4LEDATA','G4LEVELGAMMADATA','G4NEUTRONHPDATA','G4NEUTRONXSDATA','G4PIIDATA','G4RADIOACTIVEDATA','G4REALSURFACEDATA','G4SYSTEM','G4WORKDIR','G4LIB_BUILD_GDML','MGDODIR','MAGEDIR']
    token = ['XX:G4ABLADATA:XX','XX:G4INSTALL:XX','XX:G4LEDATA:XX','XX:G4LEVELGAMMADATA:XX','XX:G4NEUTRONHPDATA:XX','XX:G4NEUTRONXSDATA:XX','XX:G4PIIDATA:XX','XX:G4RADIOACTIVEDATA:XX','XX:G4REALSURFACEDATA:XX','XX:G4SYSTEM:XX','XX:G4WORKDIR:XX','XX:G4LIB_BUILD_GDML:XX','XX:MGDODIR:XX','XX:MAGEDIR:XX']
    value = ['','','','','','','','','','','','','','']
    gdml = 0
    
    for i in range(len(MaGeOutput)):
        s = MaGeOutput[i]
        substring = s.split('=')
        for j in range(len(keyword)):
            if substring[0] == keyword[j]:
                value[j] = substring[1]
                if j == 11:
                    continue
                GlobalParameter.append(token[j] + ' ' + value[j])
    
    
    # create error messages    
    errors = ''
    for i in range(len(GlobalParameter)):
        s = GlobalParameter[i]
        substring = s.split(' ')
        if substring[0] == 'XX:Errors:XX':
            errors = s
    
    if len(value[1]) == 0:
        errors = errors + '{\\color{Red}Unable to detect {\\sc Geant4} installation. Please check your {\\sc Geant4} installation and set the environment variables.\\\\}'
    if len(value[11]) == 0:
        errors = errors + '{\\color{Red}GDML support for {\\sc Geant4} disabled. Please recompile {\\sc Geant4} and MaGe with GDML support.\\\\}'
        gdml = 1      
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
    
    if len(value[13]) == 0:
        warnings = warnings + '{\\color{BurntOrange}No MaGe environment variable detected. Please define MAGEDIR.\\\\}'
    if len(value[12]) == 0:
        warnings = warnings + '{\\color{BurntOrange}No MGDO environment variable detected. Please define MGDODIR.\\\\}'
    if len(value[10]) == 0:
        warnings = warnings + '{\\color{BurntOrange}{\\sc Geant4} working directory not detected. Please define G4WORKDIR.\\\\}'
    if len(value[0]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for nuclear shell effects are missing.\\\\}'
    if len(value[2]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for low energy electromagnetic processes are missing.\\\\}'
    if len(value[3]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for photon evaporation are missing.\\\\}'
    if len(value[4]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for neutron interactions are missing.\\\\}'
    if len(value[5]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for evaluated neutron cross sections are missing.\\\\}'
    if len(value[6]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for precise impact ionisation are missing.\\\\}'
    if len(value[7]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for radioactive decays are missing.\\\\}'
    if len(value[8]) == 0:
        warnings = warnings + '{\\color{BurntOrange}Data files for measured optical surface reflectance are missing.\\\\}'
                
    for i in range(len(GlobalParameter)):
        s = GlobalParameter[i]
        substring = s.split(' ')
        if substring[0] == 'XX:Warnings:XX':
            warnings = warnings.replace('\n', '') + '\n'
            GlobalParameter[i] = warnings 
    
    for i in range(len(value)):
        s = token[i]
        if i == 11:
            if value[i] == '1\n':
                value[i] = '\\colorbox{OliveGreen}{\\color{White}yes}'
            else:
                value[i] = '\\colorbox{Red}{\\color{White}no}'
            GlobalParameter.append(token[i] + '\t' + value[i] + '\n')
            continue
        
        if len(value[i]) != 0:
            GlobalParameter.append(s[0:-3] + '_I:XX\t\\colorbox{OliveGreen}{\\color{White}yes}\n')
        else:
            GlobalParameter.append(s[0:-3] + '_I:XX\t\\colorbox{Red}{\\color{White}no}\n')
    
    writeFile('quickcheckparameter.txt',GlobalParameter)
    if gdml != 0:
        sys.exit(1)

main()
