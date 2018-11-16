#!/usr/bin/env python

import fileinput
import re
import subprocess
import os

def createMacros(material,runNumber,numEventsPerJob):
  macFileName = '%s/CERN_%d.mac' % (material,runNumber)
  macFile = open(macFileName,'w')
  inFile = open("CERN.mac",'r')
  for line in inFile:
    beamDumpLine = '/MG/geometry/CERN_NA55/setBeamDumpType %s' % material
    seedLine = '/MG/manager/useInternalSeed %d' % (100+i)
    rootFileLine = '/MG/eventaction/rootfilename %s/%s/cern%02d.root' % (os.environ['PWD'],material,runNumber)
    nMuLine = '/run/beamOn %d' % numEventsPerJob
    line = re.sub('/MG/geometry/CERN_NA55/setBeamDumpType .*',beamDumpLine,line)
    line = re.sub('/MG/manager/useInternalSeed .*',seedLine,line)
    line = re.sub('/MG/eventaction/rootfilename .*',rootFileLine,line)
    line = re.sub('/run/beamOn .*',nMuLine,line)
    macFile.write(line)

def makeQsubScript(material,runNumber):
  qsubFileName = '%s/runmage%d' % (material,runNumber)
  qsubFile = open(qsubFileName,'w')
  qsubFile.write('#!/bin/csh\n')  
  jobNameLine = '#$ -N MaGe%s%d\n\n' % (material,runNumber)
  qsubFile.write(jobNameLine)

  magePath = str(os.path.expandvars('$MAGEDIR'))
  pathToMage = '%s/bin/Linux-g++/MaGe' % (magePath)
  pathToMacro = '%s/%s/CERN_%d.mac' % (os.environ['PWD'],material,runNumber)
  execLine = '%s -p Shielding %s\n' % (pathToMage,pathToMacro)
  qsubFile.write(execLine)
  qsubFile.close()
  
def makeRootScript(material,jobList):
  runrootPath = './%s/runroot' % material
  runrootFile = open(runrootPath,'w')
  runrootFile.write('#!/bin/csh\n')
  jobListLine = '#$ -hold_jid %s\n\n' % ','.join(jobList)
  runrootFile.write(jobListLine)
  execLine = '%s/CERNanalysis %s\n' % (os.environ['PWD'],material)
  runrootFile.write(execLine)
  runrootFile.close()

def makeLatexScript(jobID):
  scriptPath = './genReport'
  scriptFile = open(scriptPath,'w')
  scriptFile.write('#!/bin/csh\n')
  holdLine = '#$ -hold_jid %s\n\n' % jobID
  execLine = '../RGen ../globalparameter.txt CERNtemplate.tex CERNreport.tex\n'
  scriptFile.write(execLine)
  scriptFile.close()

def submitJob(pathToScript):
  qsubCommandList = ['qsub',pathToScript]
  qsubJob = subprocess.Popen(qsubCommandList,stdout=subprocess.PIPE)
  qsubOutput = qsubJob.stdout.read()
  jobid = re.search('[0-9]{7}',qsubOutput).group(0)
  return jobid
    
if __name__ == '__main__':
  numJobs = 10
  numEvents = 1e6
  for material in ['Copper','Lead','Graphite']:
    jobList = []
    materialDir = './%s' % material
    if not os.path.isdir(materialDir):
      print 'making directory %s'% material
      subprocess.call(['mkdir',material])
    for i in range(0,numJobs):
      createMacros(material,i,numEvents)
      makeQsubScript(material,i)
      pathToScript = './%s/runmage%d' % (material,i)
      jobList.append(submitJob(pathToScript))
    
    makeRootScript(material,jobList)
    pathToScript = './%s/runroot' % material
    jobID = submitJob(pathToScript)
    makeLatexScript(jobID)
    submitJob('./genReport')
    
