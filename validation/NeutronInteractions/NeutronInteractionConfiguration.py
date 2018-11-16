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
  token = ['NI_NumberOfSimulatedInteractions','NI_RunSimulation','NI_RelativeComparison','RC_Version', 'RC_newVersion']
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

# setting the number of interactions
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
