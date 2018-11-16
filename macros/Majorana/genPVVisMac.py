#!/usr/bin/python
             
import os, sys, string, fnmatch
from optparse import OptionParser

pyVersion = str(sys.version_info[0]) + "." + str(sys.version_info[1])
versionNumber = "1.0"
version = "\nGenerate Component Group Visualization Macro v" + versionNumber + """ released December 7, 2011.
Tested on python v2.5 (you are using """ + pyVersion + """).
Send feedback to khangdnguyen@lbl.gov or jadetwiler@lbl.gov.

Version history:
v1.0, 2011 December 7 - Initial release
"""

usage = "Usage: python %s [options] file1.txt file2.txt ... fileN.txt" % sys.argv[0]

# ----------------------------------------------------------------------#
#                          USAGE DOCUMENTATION                          #
# ----------------------------------------------------------------------#
usageDocument = r"""

          	        Welcome to GenPVVisMac!
              Generate Physical Volume Visualization Macro

This script generates visualization macros from text files given by the
user. The text files are expected to only contain names of physical volumes
from a specific detector geometry, assigned to "MJDemonstator" by default. 
The output macro files serve as templates which can be individually modified 
to the user's liking, such as adjusting the lighting vector and zoom factor. 
Here are some basic usage examples:

       Example:   python genPVVisMac.py file1.txt               
      Produces:   file1.mac 

       Example:   python genPVVisMac.py file1.txt file2.txt ... fileN.txt              
      Produces:   file1.mac file2.mac ... fileN.mac 

-------------------------------------------------------------------------
			      BULK MODE
-------------------------------------------------------------------------
Bulk Mode allows the user to create macros from ALL .txt files found in the
current directory. This feature is offered as a convenience for bulk jobs
so the user does not have to re-run the script multiple times or type
each and every .txt file they wish to create a visualization macro for.

To enter Bulk Mode, do the following command:

                        python genPVVisMac.py -b

-------------------------------------------------------------------------
		       SPECIFYING WHICH GEOMETRY
-------------------------------------------------------------------------
You can specify which detector geometry you would like to write the macro
for. Simply use the -g option along with a valid detector name, like so:

       Example:   python genPVVisMac.py -g Bruno file1.txt               
      Produces:   file1.mac

Instead of using the default detector MJDemonstrator, file1.mac is written
for the Bruno detector.

-------------------------------------------------------------------------
		        CREATING AN EMPTY MACRO
-------------------------------------------------------------------------
If you do not have a .txt file containing a list of all the physical
volumes you would like to draw, you can still use this script to create
an empty macro to help you set up basic settings for your visualization.
A file called "emptyMacro.mac" will be created which you can edit to suit
your needs. Some examples of using the -e option follows:

       Example:   python genPVVisMac.py -e
      Produces:   emptyMacro.mac

If you give a list of .txt files or use bulk mode in addition to using the 
-e option, macros for those text files will be created in addition to the 
empty macro: 

       Example:   python genPVVisMac.py -e file1.txt
      Produces:   emptyMacro.mac file1.mac

       Example:   python genPVVisMac.py -b -e 
      Produces:   emptyMacro.mac file1.mac file2.mac ... fileN.mac
"""

# ----------------------------------------------------------------------#
#                            PROGRAM OPTIONS                            #
# ----------------------------------------------------------------------#
parser = OptionParser(usage + usageDocument)

# ------------------------> General Options < ---------------------------
parser.add_option("-b", "--bulk", action="store_true", dest="bulkMode", default=False,
                  help="Run script for all .txt files found in current directory and subdirectories")
parser.add_option("-e", "--empty", action="store_true", dest="empty", default=False,
                  help="Generate an empty macro for user to add individual physical volumes")
parser.add_option("-g", "--geometry", action="store", type="string", dest="geometry", default="MJDemonstrator",
                  help="Specify which detector geometry to use")
parser.add_option("-v", "--version", action="store_true", dest="printVersion", default=False,
                  help="Print version info and exit")

(options, args) = parser.parse_args()


# ----------------------------------------------------------------------#
#                            PRINTING HELP                               #
# ----------------------------------------------------------------------#
if len(args) == 0 and options.bulkMode == False and options.empty == False:
    print usage
    print 'Use option -h to show full help documentation.'
    sys.exit(0)



# ----------------------------------------------------------------------#
#                           PRINT VERSION                               #
# ----------------------------------------------------------------------#
if(options.printVersion):
    print version
    sys.exit(0)


# ----------------------------------------------------------------------#
#                CREATE LIST OF TEXT FILES TO BE CREATED                #
# ----------------------------------------------------------------------#
listOfTextFiles = [] 
for k in range(0, len(args)):
    positionalArgs = str(args[k])
    if positionalArgs[-4:] == '.txt':
	textFilename = args[k]
	if not listOfTextFiles:
	    listOfTextFiles.append(textFilename)
	if textFilename not in listOfTextFiles:
	    listOfTextFiles.append(textFilename)
    else:
	print '\aERROR: Sorry, only .txt files are acceptable!' 
	sys.exit(0)


# ----------------------------------------------------------------------#
#                    CREATE MACROS FOR GIVEN TEXT FILE                  #
# ----------------------------------------------------------------------#
def CreateEmptyMacro():    
    """Create an empty macro."""
    empty = open("emptyMacro.mac", "w")
    empty.write("# Basic Settings\n")
    empty.write("/MG/manager/mglog routine\n")
    empty.write("/MG/geometry/detector [GEOMETRY]\n")
    empty.write("/MG/processes/useNoHadPhysics true\n")
    empty.write("/run/initialize\n")
    empty.write("\n")
    empty.write("/vis/open RayTracerX\n")
    empty.write("\n")
    empty.write("# Camera Settings\n")
    empty.write("/vis/viewer/set/viewpointThetaPhi 45 30\n")
    empty.write("/vis/viewer/set/background white\n")
    empty.write("/vis/viewer/set/lightsThetaPhi 30 30\n")
    empty.write("/vis/viewer/zoom 0.25\n")
    empty.write("/vis/viewer/pan 0 0\n")
    empty.write("\n")
    empty.write("/vis/scene/create\n")
    empty.write("\n")
    empty.write("# Visibility Settings\n")
    empty.write("/vis/geometry/set/visibility all -1 false\n")
    empty.write("/MG/geometry/SetVisForPhysVolsMatching [PHYSICAL VOLUME NAME]\n")
    empty.write("\n")
    empty.write("/vis/drawVolume\n")
    empty.write("/vis/viewer/flush")
    empty.close()
    # Return name of macro created
    return "emptyMacro.mac"

def CreateTemplateMacro(filename):    
    """Create a macro from the given filename. Return the name of the
    macro created."""
    extension = filename.find(".txt")
    macroname = filename[:extension] + ".mac"
    template = open(macroname, "w")
    template.write("# Basic Settings\n")
    template.write("/MG/manager/mglog routine\n")
    template.write("/MG/geometry/detector " + options.geometry + "\n")
    template.write("/MG/processes/useNoHadPhysics true\n")
    template.write("/run/initialize\n")
    template.write("\n")
    template.write("/vis/open RayTracerX\n")
    template.write("\n")
    template.write("# Camera Settings\n")
    template.write("/vis/viewer/set/viewpointThetaPhi 45 30\n")
    template.write("/vis/viewer/set/background white\n")
    template.write("/vis/viewer/set/lightsThetaPhi 30 30\n")
    template.write("/vis/viewer/zoom 0.25\n")
    template.write("/vis/viewer/pan 0 0\n")
    template.write("\n")
    template.write("/vis/scene/create\n")
    template.write("\n")
    template.write("# Visibility Settings\n")
    template.write("/vis/geometry/set/visibility all -1 false\n")
    # Read in physical volume names from text file
    inputFile = open(filename, "r")
    lineList = inputFile.readlines()
    inputFile.close()
    for line in lineList:
        template.write("/MG/geometry/SetVisForPhysVolsMatching " + line)
    template.write("\n")
    template.write("/vis/drawVolume\n")
    template.write("/vis/viewer/flush")
    template.close()
    # Return name of macro created
    return macroname


# ----------------------------------------------------------------------#
#                  LOCATE FILES WITH SPECIFIED PATTERN                  #
# ----------------------------------------------------------------------#
def LocateFiles(pattern, root=os.curdir):
    """Locate files that match the given pattern. Starts search in current
    directory by default."""
    matches = []
    for path, dirs, files in os.walk(os.path.abspath(root)):
	for filename in fnmatch.filter(files, pattern):
	    matches.append(os.path.join(path, filename))
    return matches



# ----------------------------------------------------------------------#
#                    CREATE MACRO(S) FROM TEXT FILE(S)                  #
# ----------------------------------------------------------------------#
listOfMacrosCreated = []
if(options.empty):
    listOfMacrosCreated.append(CreateEmptyMacro())

if(options.bulkMode):
    for fileFound in LocateFiles("*.txt"):
	listOfMacrosCreated.append(CreateTemplateMacro(fileFound))
else:
    for file in listOfTextFiles:
	listOfMacrosCreated.append(CreateTemplateMacro(file))


# ----------------------------------------------------------------------#
#                    PRINT SUMMARY OF MACROS CREATED                    #
# ----------------------------------------------------------------------#
print '\nThe following macros have been successfully created...'
print ('=' * 70)
for macro in listOfMacrosCreated:
    print macro


del listOfTextFiles
del listOfMacrosCreated

sys.exit(0)

