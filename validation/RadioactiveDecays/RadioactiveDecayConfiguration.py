#!/usr/bin/env python

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
  
  dictionary = {}
  token = ['RD_NumberOfSimulatedDecays','RD_RunSimulation','RD_RelativeComparison','RC_Version', 'RC_newVersion']
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

# setting the number of decays    
  files = glob.glob("macros/*.mac")
  for i in range(len(files)):
      print 'configuring file ' + files[i]
      macro = readFile(files[i])
      for j in range(len(macro)):
          s = macro[j]
          if s[0] == '#':
              continue
          substring = s.split(' ')
          if substring[0] == '/run/beamOn':
              macro[j] = substring[0] + ' ' + dictionary[token[0]]
      writeFile(files[i], macro)
 
 # turning simulation of radioactive decays on/off
#  print 'configuring radioactive decay validation script'  
#  script = readFile('RadioactiveDecayValidation.py')
#  for i in range(len(script)):
#      s = script[i]
#      substring = s.split(' ')
#      if dictionary[token[1]][0] == '1':
#          if substring[0] == '#\tos.system(\'DecayEngine':
#              substring[0] = substring[0].replace('#','')
#              for j in range(len(substring)):
#                  if j > 0:
#                      ns = ns + ' ' + substring[j]
#                  else:
#                      ns = substring[j]  
#              script[i] = ns
#      else:
#          if substring[0] == '\tos.system(\'DecayEngine':
#              script[i] = '#' + s
#  writeFile('RadioactiveDecayValidation.py', script)

 # adding relative comparison parameters 
#  script = readFile('RadioactiveDecayValidation.py')
#  for i in range(len(script)):
#      s = script[i]
#      substring = s.split(' ')
#      #os.system('./DecayAnalysis dat/' + element + '.dat dat/' + element + '.root' + '>/dev/null')
#      if substring[0] == '\t\tos.system(\'./DecayAnalysis':
#          script[i] = substring[0] + ' dat/\' + element + \'.dat dat/\' + element + \'.root \' + \'' + dictionary[token[3]][:-1] + ' ' + dictionary[token[4]][:-1] + '\' + \' > /dev/null\')\n'
#  writeFile('RadioactiveDecayValidation.py', script)


# adjusting LaTeX templates for relative comparison
  print 'adjusting LaTeX template files'
  files = glob.glob("./*template.tex")
  for i in range(len(files)):
      print files[i]
      latex = readFile(files[i])
      rcactive = 0
      for j in range(len(latex)):
          s = latex[j]
          if s == '%RC*START\n':
              rcactive = 1
          if s == '%RC*END\n':
              rcactive = 0
          
          if rcactive == 1 and s != '%RC*START\n':
              if dictionary[token[2]][0] == '0':
                  if s[0] != '%':
                      latex[j] = '%' + s
                      
              if dictionary[token[2]][0] == '1':
                  if s[0] == '%':
                      latex[j] = s[1:]        
      writeFile(files[i], latex)       

main()
