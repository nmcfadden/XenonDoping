#!/usr/bin/python
             
import os, sys, string, fnmatch
import math, shutil, subprocess, time
from cStringIO import StringIO
from optparse import OptionParser, OptionGroup

from ROOT import gROOT
gROOT.SetBatch(True)
from ROOT import gRandom
from ROOT import gStyle
from ROOT import TFile
from ROOT import TTree
from ROOT import TObject
from ROOT import TChain
from ROOT import TCanvas
from ROOT import TF1
from ROOT import TH1D
from ROOT import TLegend

pyVersion = str(sys.version_info[0]) + "." + str(sys.version_info[1])
# fonts from (http://patorjk.com/software/taag/): Alligator2, BigMoney-nw
# ~ ~ ~ ~ ~ ~ ~ Courtesy of http://patorjk.com/software/taag/ ~ ~ ~ ~ ~ ~ ~
scriptDescription = r"""
                                         ***WELCOME***

This Python script was written to manage Majorana ton scale simulation tasks. All it needs are macro
files written specifically to run on MaGe. Other macro files will not work properly! With that said,
let's get started shall we?  :)
"""
logo = r"""
*****************************************************************************************************
*                                                                                                   *
*              ::::    ::::      :::     ::::    :::     :::      ::::::::  ::::::::::              *
*              +:+:+: :+:+:+   :+: :+:   :+:+:   :+:   :+: :+:   :+:    :+: :+:                     *
*              +:+ +:+:+ +:+  +:+   +:+  :+:+:+  +:+  +:+   +:+  +:+        +:+                     *
*              +#+  +:+  +#+ +#++:++#++: +#+ +:+ +#+ +#++:++#++: :#:        +#++:++#                *
*              +#+       +#+ +#+     +#+ +#+  +#+#+# +#+     +#+ +#+   +#+# +#+                     *
*              #+#       #+# #+#     #+# #+#   #+#+# #+#     #+# #+#    #+# #+#                     *
*              ###       ### ###     ### ###    #### ###     ###  ########  ##########              *
*                                                                                                   *
*  ::::::::::: ::::::::  ::::    :::       ::::::::   ::::::::      :::     :::        ::::::::::   *
*      :+:    :+:    :+: :+:+:   :+:      :+:    :+: :+:    :+:   :+: :+:   :+:        :+:          *
*      +:+    +:+    +:+ :+:+:+  +:+      +:+        +:+         +:+   +:+  +:+        +:+          *
*      +#+    +#+    +:+ +#+ +:+ +#+      +#++:++#++ +#+        +#++:++#++: +#+        +#++:++#     *
*      +#+    +#+    +#+ +#+  +#+#+#             +#+ +#+        +#+     +#+ +#+        +#+          *
*      #+#    #+#    #+# #+#   #+#+#      #+#    #+# #+#    #+# #+#     #+# #+#        #+#          *
*      ###     ########  ###    ####       ########   ########  ###     ### ########## ##########   *
*                                                                                                   *
*    :::::::: ::::::::::: ::::    ::::       ::::::::::: :::      ::::::::  :::    ::: ::::::::     *
*   :+:    :+:    :+:     +:+:+: :+:+:+          :+:   :+: :+:   :+:    :+: :+:   :+: :+:    :+:    *
*   +:+           +:+     +:+ +:+:+ +:+          +:+  +:+   +:+  +:+        +:+  +:+  +:+           *
*   +#++:++#++    +#+     +#+  +:+  +#+          +#+ +#++:++#++: +#++:++#++ +#++:++   +#++:++#++    *
*          +#+    +#+     +#+       +#+          +#+ +#+     +#+        +#+ +#+  +#+         +#+    *
*   #+#    #+#    #+#     #+#       #+#          #+# #+#     #+# #+#    #+# #+#   #+# #+#    #+#    *
*    ######## ########### ###       ###          ### ###     ###  ########  ###    ### ########     *
*                                                                                                   *
*                                          $$\       $$$$$$\                                        *
*                                         $$$ |     $$$ __$$\                                       *
*                              \$$\   $$\\_$$ |     $$\$$\$$ |                                      *
*                               \$$ $$ /   $$ |     $$ |\$$$ |                                      *
*                                \$$$ /  $$$$$$\ $$\\$$$$$$  /                                      *
*                                 \_ /  \______|\__|\______/                                        *
*                                                                                                   *
*                             By: Khang Nguyen, LBNL                                                *
*                             Last Update: Monday, July 30, 2012                                    *
*                                                                                                   *
*****************************************************************************************************
"""
versionNumber = "1.0"
version = "\nManage Ton Scale Simulation Tasks v" + versionNumber + """ released July 30, 2012.
Tested on python v2.7 (you are using """ + pyVersion + """).
Send feedback to photonfactor@gmail.com, jadetwiler@lbl.gov, or rdmartin@lbl.gov.

Version history:
v1.0, 2012 July 30  - Initial release
"""

usage = "Usage: python %s [options] /macros/directory1 ... /macros/directoryN" % sys.argv[0]

# ----------------------------------------------------------------------#
#                          USAGE DOCUMENTATION                          #
# ----------------------------------------------------------------------#
# REMINDER: Do :set textwith=90 (or :set tw=90) in vi for formatting of usage document
usageDocument = r"""

                          Welcome to ManageTonScaleSimulationTasks!
                              Manage Ton Scale Simulation Tasks

This script was written to assist users in managing the following aspects of doing ton
scale simulations for Majorana:

1. Creation of .csh files from macros;
2. Submission of jobs which includes the running of MaGe;
3. Removal of .csh files for jobs that ran successfully;
4. Collection of MaGe outputs after jobs are completed;
5. Running of GAT on those MaGe outputs; and
6. Analysis of the GAT outputs.

In order to run the script, the user needs to provide the absolute path to a directory
containing the macro files which he/she wishes to analyze. The user can list up to any
number of directories he/she likes. In each correctly specified directory where macro
files are found, the following files and folders are created:

0. /BigMacros folder:          If created, this folder stores all the big macros that were
                               split up by the script because the macros specify a number
                               of runs that is too large.
1. /Macros folder:             This folder stores all the macros you asked the script to
                               analyze.
2. /MaGeOutputs folder:        This folder stores all the MaGe outputs from your
                               simulations after they have completed.
3. /GATOutputs folder:         This folder stores all the GAT outputs after you run GAT on
                               the outputs in the MaGeOutputs folder.
4. /Graphs folder:             This folder stores all the graphs produced from the
                               analysis of all outputs in the GATOutputs folder. It also
                               contains a file called "SummaryOfAnalyses.txt" which
                               contains a summary of all the results from the analysis on
                               your simulations. 
5. /Graphs/ErrorPlots folder:  This folder stores all the error plots used to calculate
                               the errors on the efficiency cuts. 

In addition to these files and folders, in the directory where the script is ran, two
other files are created:

6. JobsSummaryStatus.txt:    This document records all the jobs which succeeded and failed
                             to submit.
7. .stored_directories.log:  A hidden file which stores all the macro directories you
                             specified the last time you ran the script. If it exists,
			     this file is read in by the script when you use the special
			     character "@". Please read the "General Options" section
			     below for further details.

Here are some basic usage examples:

       Example:   python manageTonScaleSimulationTasks.py /Majorana/TSSimulations/macros
      Produces:   /Majorana/TSSimulations/macros/JobsSummaryStatus.txt
		  /Majorana/TSSimulations/macros/.stored_directories.log
		  [/Majorana/TSSimulations/macros/BigMacros/*]      <-- if needed
		  /Majorana/TSSimulations/macros/Macros/*
                  /Majorana/TSSimulations/macros/MaGeOutputs/*
		  /Majorana/TSSimulations/macros/GATOutputs/*
		  /Majorana/TSSimulations/macros/Graphs/ErrorPlots/*
		  /Majorana/TSSimulations/macros/Graphs/*

       Example:   python manageTonScaleSimulationTasks.py /directory1/macros
                         /directory2/macros ... directoryN/macros
      Produces:   [/directory1/macros/BigMacros/*]                  <-- if needed
                  /directory1/macros/Macros/*
                  /directory1/macros/MaGeOutputs/*
		  /directory1/macros/GATOutputs/*
		  /directory1/macros/Graphs/ErrorPlots/*
		  /directory1/macros/Graphs/*

		  [/directory2/macros/BigMacros/*]                  <-- if needed
		  /directory2/macros/Macros/*
                  /directory2/macros/MaGeOutputs/*
		  /directory2/macros/GATOutputs/*
		  /directory2/macros/Graphs/ErrorPlots/*
		  /directory2/macros/Graphs/*
                                   ...
		  [/directoryN/macros/BigMacros/*]                  <-- if needed
		  /directoryN/macros/Macros/*
                  /directoryN/macros/MaGeOutputs/*
		  /directoryN/macros/GATOutputs/*
		  /directoryN/macros/Graphs/ErrorPlots/*
		  /directoryN/macros/Graphs/*

There are three categories of options available to users: General options, specific task
options, and progressive task options. Each of these categories is explained in further
detail below. If no options are specified, the script automatically creates .csh files of
your macros files, submits your jobs, and cleans up those .csh files for jobs that ran
successfully. After these actions, the script stops execution while your simulation jobs
are running and prompts you to type the following command to continue where the script
left off after your jobs have completed:

                  python manageTonScaleSimulationTasks.py --collectEnd @ 

The at sign "@" is a special character used in this script. Please read the notes at the
end of the "General Options" section for more details about how it is used.

For macro files that simulate events more than a certain number (set to 10 million events
when this script was originally implemented), the script automatically splits those macro
files up into a number of parts. These files have a "_Part" suffix attached to the end of
their basename. The original macro files that specified a run number too great are stored
in a folder called "BigMacros" in the directory where this script was ran.

                                       ***WARNING***
 
At the moment, this script can only analyze simulations of decays of Thallium 208. The
script is not general enough to handle the decay of other isotopes. Future implementation
will be necessary to expand the capabilities of this script.

=============================================================================================
                                      GENERAL OPTIONS
=============================================================================================
---------------------------------------> Bulk Mode <-----------------------------------------
As mentioned above, the standard way to run this script is to provide the absolute paths
to each and every single one directory that contains the macro simulation files you want
to run and analyze. What if you have so many directories with macros that you want to
simulate and analyze? Say there are over a hundred directories. Do you have to enter each
and every single one? The answer is no. An option called "Bulk Mode" was implemented in
this script to allow the user to specify just one base path. The script will start
searching in this base path and continue searching in all its subdirectories for all macro
files. 

To use Bulk Mode, do the following command: 
    
                  python manageTonScaleSimulationTasks.py -b /base/pathname

Here is an example to illustrate the power of bulk mode:

       Example:   python manageTonScaleSimulationTasks.py -b /Majorana/TSSimulations/
      Produces:   [/Majorana/TSSimulations/FoundMacros1/BigMacros/*]       <-- if needed
                  /Majorana/TSSimulations/FoundMacros1/Macros
                  /Majorana/TSSimulations/FoundMacros1/MaGeOutputs/*
		  /Majorana/TSSimulations/FoundMacros1/GATOutputs/*
		  /Majorana/TSSimulations/FoundMacros1/Graphs/ErrorPlots/*
		  /Majorana/TSSimulations/FoundMacros1/Graphs/*
                 
                  [/Majorana/TSSimulations/FoundMacros2/BigMacros/*]       <-- if needed
		  /Majorana/TSSimulations/FoundMacros2/Macros
                  /Majorana/TSSimulations/FoundMacros2/MaGeOutputs/*
		  /Majorana/TSSimulations/FoundMacros2/GATOutputs/*
		  /Majorana/TSSimulations/FoundMacros2/Graphs/ErrorPlots/*
		  /Majorana/TSSimulations/FoundMacros2/Graphs/*
                                           ...
                  [/Majorana/TSSimulations/FoundMacrosN/BigMacros/*]       <-- if needed
		  /Majorana/TSSimulations/FoundMacrosN/Macros
                  /Majorana/TSSimulations/FoundMacrosN/MaGeOutputs/*
		  /Majorana/TSSimulations/FoundMacrosN/GATOutputs/*
		  /Majorana/TSSimulations/FoundMacrosN/Graphs/ErrorPlots/*
		  /Majorana/TSSimulations/FoundMacrosN/Graphs/*

Because bulk mode is so powerful, the number of base path names the user can specify is
restricted to just ONE. The main use for bulk mode is to do bulk jobs. If multiple base
path names are given, it would be extremely difficult for the user to locate the sources
of error if something should go wrong. While the script was carefully implemented to
perform many checks, the task of book keeping thousands of jobs, say, can be daunting!
Bulk mode has restrictions for the practical purpose of easier user management.

-----------------------------------> Interactive Mode <--------------------------------------
This user documentation can be quite tedious to read to understand all of the script's
capabilities. For users who just want to dive right in to using the script, an option was
included to make life very simple. This option is called "Interactive Mode". Interactive
Mode guides the user through every step of the analysis process. Questions appear in
succession on the screen and input is retrieved from the user. Interactive Mode is
extremely user-friendly. If you give an invalid answer anywhere, the script will ask you
whether you would like to try again and the process continues from where you left off; you
don't have to re-run Interactive Mode and answer all the questions again. You can quit
Interactive Mode any time, anywhere by simplying entering "q" at the prompt. To enter
Interactive Mode, do the following command:

                  python manageTonScaleSimulationTasks.py -i

Interactive Mode terminates after your jobs have submitted and the script exits. The
reason for this is because it takes time for MaGe to simulate your jobs. The script won't
be able to do anything until those jobs are completed. After your jobs are completed, to
continue where the script left off, type the following command:

                  python manageTonScaleSimulationTasks.py --collectEnd @

This command starts the script by collecting all the MaGe outputs (more about "progressive
options" below) and then continuing to the end. The "@" character is a special character
in this script used to read in a file called ".stored_directories.log". This file is a
hidden file located in the directory where the script is ran. It stores the time and date
when the file was created and all the existing macro directories the user specified the
last time the script was ran. By typing the above command after exiting Interactive Mode
and your jobs have completed, the script reads in ".stored_directories.log" and continues
the analysis without you having to specify all the macro directory paths again (which you
might've forgotten after waiting so long for those simulations to complete).

NOTES ABOUT SPECIAL CHARACTER "@": The usefulness of the special character "@" goes beyond
after Interactive Mode exits. Suppose the user made changes to the macro files he/she
analyzed before. Instead of specifying all the macro directory paths again, the user can
re-run the script with the character "@" as the positional argument without typing
everything over again. This is especially useful if you entered in a lot of directories
before and don't remember!
 
-------------------------------------> Fresh Option <----------------------------------------
The "-f" or "--fresh" option allows the user to remove files and folders the script
created for a fresh re-run of the script. Two types of removal methods are available to
users for practical matters: Soft removal and hard removal.

1. Soft removal: - KEEP contents in /Macros folder and /MaGeOutputs folder but REMOVE all
                   contents in /GATOutputs and /Graphs folder.
                 - PURPOSE: So user doesn't have to re-write macros and wait for MaGe to 
                   complete simulation jobs, which probably took a long time to complete 
                   in the first place if many jobs were involved. Also, if changes were to 
                   be made to this script in the future, the user could just re-run GAT and 
                   analyze those GAT outputs on the same old simulation results.  
2. Hard removal: - REMOVE all files including contents in /Macros folder, /MaGeOutputs 
                   folder, /GATOutputs/ folder, and /Graphs folder. Any macro files in
                   /BigMacros and /Macros will be restored.
                 - PURPOSE: So user can do a complete re-run of the script. 

------------------------------------> Geometry Option <--------------------------------------
The "-g" or "--geometry" option creates a macro file called "TSGeometryViewer.mac" which
can be used to view the geometry of various detector components. The user can modify this
file to view changes he/she makes to the detector geometries, for instance, to check that
his/her code is working properly. This file can also be used to produce images of
detector components for inclusion in documents, reports, etc.

-------------------------------------> Macro Option <----------------------------------------
The "-m" or "--macro" option creates a macro file called "TSBasicMacro.mac" which gives
the user a starting point for all simulation work. The user can modify this file to fit
the need and specifications of whatever simulation job needs to be performed. Simply
re-name this file to whatever name you please, make any necessary changes to suit the
simulation, and copy it over and over again for other related simulations (modifying them
accordingly of course). Please do take care to read the macro file over before you blindly
accept all the default parameters. As mentioned previously, "TSBasicMacro.mac" serves as a
STARTING POINT to help you get started with writing your own simulation macro. It does NOT
replace the thinking for you. You would need to change the ROOT output filename in the
basic macro, for example.

-------------------------------------> Path Option <-----------------------------------------
Normally, for every macro directory the user specifies, the script stores all of its
outputs in that directory. You can change this behavior with the "-p" or "--path" option.
This option allows the user to specify exactly where he/she wishes all of the scripts's
outputs to be stored. This can be useful when the user wants all of the graphs to be
stored in one location as opposed to 20 different locations corresponding to each macro
directory, for instance.  

Two methods were implemented for the "-p" option: 

1. Copy option: - Copy all contents to the specified path. All contents in the macro
                  directories given on the command line will stay intact. This is the
                  default action.
2. Move option: - Move all contents to the specified path. All contents in the macro
                  directories given on the command line will no longer be there.

This "-p" option can only be used in combination with the following options: 

                          1. --collect           4. --collectEnd
                          2. --run               5. --runEnd           
                          3. --analyze"

The reason for this is because creating .csh files (--create option), submitting jobs
(--submit option), and cleaning up .csh files for jobs that ran successfully (--clean
option) are actions that do not involve manipulating data, analyzing data, and creating
permanent outputs, so there is little use in relocating their results to one common
location. The way the "-p" option works is a little tricky, so I will try to explain its
use with each of the allowable options above.

1. In combination with the "--collect" option: This will cause only outputs in the
MaGeOutputs folder for each specified macro directory to be copied or move.

2. In combination with the "--run" option: This will cause only outputs in the GATOutputs
folder for each specified macro directory to be copied or moved.

3. In combination with the "--analyze" option: This will cause only outputs in the Graphs
folder and Graphs/ErrorPlots subfolder for each specified macro directory to be copied or
moved.

4. In combination with the "--collectEnd" option: This will cause outputs in the
MaGeOutputs folder and the following files to be copied or moved if they exist:
 * JobsSummaryStatus.txt file:  A new file with the same name "JobsSummaryStatus.txt" is
                                created in the path specified which combines all the 
                                "JobsSummaryStatus.txt" files in all the macro directories
				into one.
 * TSBasicMacro.mac file:       A new file with the same name "TSBasicMacro.mac" is
                                created in the path specified which combines all the 
                                "TSBasicMacro.mac" files in each macro directory into one.
 * TSGeometryViewer.mac file:   A new file with the same name "TSGeometryViewer.mac" is
                                created in the path specified which combines all the
                                "TSGeometryViewer.mac" files in each macro directory into
                                one.
 * Files in BigMacros folder:   If this folder exists, all files will be copied or moved
                                to the path specified.
 * Files in Macros folder:      If this folder exists, all files will be copied or moved
                                to the path specified.
 * .stored_directories.log:     A new file with the same name ".stored_directories.log" is
                                created in the path specified which combines all the
				".stored_directories.log" files in all the macro
				directories into one.

5. In combination with the "--runEnd" option: This will cause outputs in the
GATOutputs folder and the following files to be copied or moved if they exist:
 * JobsSummaryStatus.txt file:  A new file with the same name "JobsSummaryStatus.txt" is
                                created in the path specified which combines all the 
                                "JobsSummaryStatus.txt" files in all the macro directories
				into one.
 * TSBasicMacro.mac file:       A new file with the same name "TSBasicMacro.mac" is
                                created in the path specified which combines all the 
                                "TSBasicMacro.mac" files in each macro directory into one.
 * TSGeometryViewer.mac file:   A new file with the same name "TSGeometryViewer.mac" is
                                created in the path specified which combines all the
                                "TSGeometryViewer.mac" files in each macro directory into
                                one.
 * Files in BigMacros folder:   If this folder exists, all files will be copied or moved
                                to the path specified.
 * Files in Macros folder:      If this folder exists, all files will be copied or moved
                                to the path specified.
 * .stored_directories.log:     A new file with the same name ".stored_directories.log" is
                                created in the path specified which combines all the
				".stored_directories.log" files in all the macro
				directories into one.

Because the hidden file .stored_directories.log is consolidated into one if you use the
"-p" option, the special character "@" will no longer work the way it is supposed to, so
beware!

It is important to note that copying or moving action only includes files and folders this
script creates. As a reminder, I have listed the script's possible outputs here:

1. JobsSummaryStatus.txt file
2. TSBasicMacro.mac file
3. TSGeometryViewer.mac file
4. BigMacros folder and contents
5. Macros folder and contents
6. MaGeOutputs folder and contents
7. GATOutputs folder and contents
8. Graphs folder and contents
9. Graphs/ErrorPlots folder and contents

With that said, any outside files found in each macro directory will not be copied or
moved. For example, .root files and Python scripts in the base macro directory will not be
moved over.

NOTE: Use the -p option with care! Specify the absolute path where you want to store the
script's outputs immediately after the -p option. Remember that this script takes in
pathways to directories as positional arguments. The following two examples produce two
different results depending on where you specify the directories:

       Example:   python manageTonScaleSimulationTasks.py /Majorana/Macros -p ./Outputs
        Action:   Look for macros in /Majorana/Macros, analyze them, and store all outputs
                  in ./Outputs.

BUT

       Example:   python manageTonScaleSimulationTasks.py -p /Majorana/Macros ./Outputs
      Produces:   Look for macros in ./Outputs, analyze them, and store all outputs in
                  /Majorana/Macros.

=============================================================================================
			            SPECIFIC TASK OPTIONS
=============================================================================================
Specific task options allow the user to perform just one action in the script and then
exit. The actions available are:

1. Create .csh files from macros;
2. Submit jobs and run MaGe;
3. Remove .csh files for jobs that ran successfully;
4. Collect MaGe outputs after jobs are completed;
5. Run GAT on those MaGe outputs; and
6. Analyze those GAT outputs.

For example, suppose you just want to analyze the GAT outputs for a specify set of macro
files. You would type in the following command:

       Example:   python manageTonScaleSimulationTasks.py --analyze /Majorana/Macros
        Action:   Look for GAT outputs in /Majorana/Macros, analyze them, and exit script.

The specific task options have the specific order of precedence given above. In other
words, if two or more specific task options are given on the command line, the option with
the highest precedence will execute and the script exits, disregarding the other specific
task options. The only other option that will work if specified on the command line in
addition to the specific task option with the highest precedence is the "-p" option, which
allows the user to specify a path to store all the script's outputs. Without the "-p"
option, every output of the script gets stored in each correctly specified macros
directory. Here is an example:

       Example:   python manageTonScaleSimulationTasks.py --analyze --collect /Majorana/Macros
        Action:   Collect all MaGe outputs in /Majorana/Macros and exit. The "--collect"
	          option has higher precedence than the "--analyze" option. 

=============================================================================================
			           PROGRESSIVE TASK OPTIONS
=============================================================================================
Progressive task options allow the user to start anywhere in the script and continue to
the end. The actions available are:

1. Start with the submimission ofjobs and running of MaGe, then continue to the end;
2. Start with the removal of .csh files for jobs that ran successfully, then continue to
   the end;
3. Start with the collection of MaGe outputs, then continue to the end;
4. Start with the running of GAT on MaGe outputs, the continuo to the end.

For example, suppose you just want to start the script with the collection of MaGe outputs
and then continue to the end. You would type in the following command:

       Example:   python manageTonScaleSimulationTasks.py --collectEnd /Majorana/Macros
        Action:   Start script by collecting MaGe outputs for macros in
                  /Majorana/Macros and continue to the end.

The progressive task options also have a specific order of precedence -- that given above
-- like the specific task options. Please read the description in the section "Specific
Task Options" for more details.

=============================================================================================
                                       KNOWN ISSUES
=============================================================================================
Several issues have been noted during the writing and testing of this script. Some are
described here: 

1. If you get an error stating something along the lines of "Fatal Python error:
PyThreadState_Get: no current thread" when you run the script, it probably means that you
are running a version of Python that the script cannot support. This script was tested on
Python version 2.7. Older (and perhaps newer even) versions of Python may not execute this
script the way it was originally intended.

2. You may get an error about type casting and objects not having certain member functions
during the analysis of your macro files. This script interacts with ROOT in order to
produce all your plots and calculate their errors. If you get any errors of this sort, it
probably means that ROOT was not compiled with the Python version you are using. Make sure
these two programs are correctly compiled together.

3. The running of simulations using MaGe produces files with great size. If you are
running your simulations on PDSF, DO NOT store your MaGe outputs in your home directory.
PDSF has a quota of 5GB for every user's home directory. You can check your usage by
typing the following command: "myquota -M". Because of this size restriction, it is
recommended that you run your simulations in locations with greater quota such as eliza10.

=============================================================================================
                                 SUGGESTIONS FOR FUTURE WORK
=============================================================================================
I have tried to fix all of the bugs that I have found in this script, but there is always
the possibilty that someone will find something I have not encoutered. Fixing these new
bugs is one obvious suggestion for future work.

Another important development is to make this script more generic to analyze the decays of
other isotopes besides Thallium 208. The most important function to modify/expand in this
regard is AnalyzeGATOutputs(). There are many ways one can go about expanding the script's
capabilities to treat other elements, two of which I suggest here:

1. Modify or write a completely new function similar to the AnalyzeGATOutputs() function I
implemented here.

2. Re-organize this script into a class and make a new class deriving from it.

There is still much work to be done, but I hope this script serves as a good starting
point for future development.

=============================================================================================
                                      CLOSING REMARKS   
=============================================================================================
Failure to adhere to the guidelines in this documentation may result in unexpected outcomes!

Don't sweat too much about whether you got the syntax correct while running the script.
Many checks have been incorporated into the script to let you know what is happening.
Furthermore, status summaries are printed to the screen to inform you of all actions.

I hope you find this script useful in managing your ton scale simulation tasks. Please do
not hesistate to send feedback to myself (khangdnguyen@lbl.gov), Jason Detwiler
(jadetwiler@lbl.gov), or Ryan Martin (rdmartin@lbl.gov). 

Happy simulating!

Khang Nguyen, LBNL
Monday, July 30, 2012

=============================================================================================
			            SPECIAL DEDICATION   
=============================================================================================
I would like to dedicate this script to my mentor, teacher, and friend, Jason Detwiler. I
joined the simulations and analysis team in August 2010 with no programming experience
whatsoever. Jason took me under his wing, taught me how to program, guided my learning,
and challenged me with many projects. Words cannot completely describe how thankful I am
for everything I've learned. This script is my special gift to him as a small token of my
appreciation for all he has done for me. You will be missed at LBNL. Go Profesor Detwiler!

                          __ooooooooo__
                       oOOOOOOOOOOOOOOOOOOOOOo
                   oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo
                oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo
              oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo
            oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo
           oOOOOOOOOOOO*  *OOOOOOOOOOOOOO*  *OOOOOOOOOOOOo
          oOOOOOOOOOOO      OOOOOOOOOOOO      OOOOOOOOOOOOo
          oOOOOOOOOOOOOo  oOOOOOOOOOOOOOOo  oOOOOOOOOOOOOOo
         oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo
         oOOOO     OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO     OOOOo
         oOOOOOO OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO OOOOOOo
          *OOOOO  OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO  OOOOO*
          *OOOOOO  *OOOOOOOOOOOOOOOOOOOOOOOOOOOOO*  OOOOOO*
           *OOOOOO  *OOOOOOOOOOOOOOOOOOOOOOOOOOO*  OOOOOO*
            *OOOOOOo  *OOOOOOOOOOOOOOOOOOOOOOO*  oOOOOOO*
              *OOOOOOOo  *OOOOOOOOOOOOOOOOO*  oOOOOOOO*
                *OOOOOOOOo  *OOOOOOOOOOO*  oOOOOOOOO*      "Only a life lived for others
                   *OOOOOOOOo           oOOOOOOOO*      is a life worth while."
                       *OOOOOOOOOOOOOOOOOOOOO*                    - Albert Einstein
                            ""ooooooooo""
"""


# ----------------------------------------------------------------------#
#                            PROGRAM OPTIONS                            #
# ----------------------------------------------------------------------#
parser = OptionParser(logo + usage + usageDocument)

# ------------------------> General Options < ---------------------------
parser.add_option("-b", "--bulk", action="store_true", dest="bulkMode", default=False,
                  help="Run script for all .mac files found in current directory \033[1mand\033[0m subdirectories. NOTE: Use with caution!")
parser.add_option("-f", "--fresh", action="store_true", dest="fresh", default=False,
                  help="Clean up everything for a fresh run of the entire script.")
parser.add_option("-g", "--geometry", action="store_true", dest="geometryViewer", default=False,
		  help="Create basic macro to view geometry of detector components")
parser.add_option("-i", "--interactive", action="store_true", dest="interactiveMode", default=False,
		  help="Run script in Interactive Mode")
parser.add_option("-m", "--macro", action="store_true", dest="macro", default=False,
		  help="Create basic macro simulation file")
parser.add_option("-p", "--path", action="store", type="string", dest="path", default="",
                  help="Specify which path to store all newly converted files")
parser.add_option("-v", "--version", action="store_true", dest="printVersion", default=False,
                  help="Print version info and exit")

# ---------------------> Specific Task Options <-------------------------
taskSpecificOptions = OptionGroup(parser, "Specific Task Options",
                                  "These options allow you to perform one specific task in the script.")
taskSpecificOptions.add_option("--create", action="store_true", dest="createCSHFiles", default=False,
                               help="Create .csh files for all .mac files")
taskSpecificOptions.add_option("--submit", action="store_true", dest="submitJobs", default=False,
                               help="Submit simulation jobs using qsub")
taskSpecificOptions.add_option("--clean", action="store_true", dest="cleanUpCSHFiles", default=False,
                               help="Clean up all .csh files for jobs that completed successfully")
taskSpecificOptions.add_option("--collect", action="store_true", dest="collectMaGeOutputs", default=False,
                               help="Collect all MaGe .root files and put them in /MaGeOutputs folder")
taskSpecificOptions.add_option("--run", action="store_true", dest="runGAT", default=False,
                               help="Run GAT on all MaGe outputs in /MaGeOutputs folder")
taskSpecificOptions.add_option("--analyze", action="store_true", dest="analyzeGATOutputs", default=False,
                               help="Analyze all GAT outputs found in /GATOutputs folder")
parser.add_option_group(taskSpecificOptions)

# --------------------> Progressive Task Options <-----------------------
progressiveTaskOptions = OptionGroup(parser, "Progressive Task Options",
                                     "These options allow you to start anywhere in the script and continue to the end.")
progressiveTaskOptions.add_option("--submitEnd", action="store_true", dest="submitJobsEnd", default=False,
                               help="Start with submitting simulation jobs using qsub and continue to end")
progressiveTaskOptions.add_option("--cleanEnd", action="store_true", dest="cleanUpCSHFilesEnd", default=False,
                               help="Starting with cleaning up all .csh files for jobs that completed successfully and continue to end")
progressiveTaskOptions.add_option("--collectEnd", action="store_true", dest="collectMaGeOutputsEnd", default=False,
                               help="Start with collecting all MaGe .root files and put them in one folder and continue to end")
progressiveTaskOptions.add_option("--runEnd", action="store_true", dest="runGATEnd", default=False,
                               help="Start with running GAT on all MaGe outputs in /MaGeOutputs folder and continue to end")
parser.add_option_group(progressiveTaskOptions)

(options, args) = parser.parse_args()


# ----------------------------------------------------------------------#
#                            PRINTING HELP                              #
# ----------------------------------------------------------------------#
if not options.interactiveMode and not options.macro and not options.geometryViewer and not options.printVersion:
    if (len(sys.argv[1:]) == 0 and len(args) == 0) or (len(args) == 0):
        print usage
	print "The following commands may be useful:"
	print "1. Interactive Mode:      python manageTonScaleSimulationTasks.py -i"
	print "2. TSBasicMacro.mac:      python manageTonScaleSimulationTasks.py -m"
	print "3. TSGeometryViewer.mac:  python manageTonScaleSimulationTasks.py -g"
        print "Use option --h to show full help documentation."
        sys.exit(0)


# ----------------------------------------------------------------------#
#                           PRINT VERSION                               #
# ----------------------------------------------------------------------#
if(options.printVersion):
    print version
    sys.exit(0)


# ----------------------------------------------------------------------#
#                      ASKTOTRYAGAIN() FUNCTION                         #
# ----------------------------------------------------------------------#
def AskToTryAgain():
    '''Ask user if he/she would like to re-enter a value or entry that has been incorrectly entered.'''
    while(True):
        tryAgain = raw_input("Try again? (y/n) --> ")
        if tryAgain.lower() == 'y':
            break
        elif tryAgain.lower() == 'n' or tryAgain.lower() == 'q':
            print "Exiting script..."
            sys.exit(1)
        else:
            print "\aERROR: Invalid response!",
            continue


# ----------------------------------------------------------------------#
#                  LOCATE FILES WITH SPECIFIED PATTERN                  #
# ----------------------------------------------------------------------#
def LocateFiles(pattern, root=os.curdir, lookInSubdirectories = False):
    '''Locate files that match the given pattern. Starts search in current
    directory by default. Furthermore, only return files that match given
    pattern in the current directory by default.'''
    matches = []
    for path, dirs, files in os.walk(os.path.abspath(root)):
        for filename in fnmatch.filter(files, pattern):
            matches.append(os.path.join(path, filename))
        if not lookInSubdirectories: break # Search for files is done top down, so breaking here stops at parent directory
    return matches


# ----------------------------------------------------------------------#
#                  SHOWLISTOFMACRODIRECTORIES() FUNCTION                #
# ----------------------------------------------------------------------#
availableSelections = []
def ShowListOfMacroDirectories():
    '''Show (updated) list of macro directories to do analysis on. Used 
    mainly for Interactive Mode.'''
    print "\nHere is a listing of the macro directories you have specified:"
    print("=" * 75)
    print(" " * 27 + "\033[1mMACRO DIRECTORIES\033[0m")
    print("=" * 75)
    counter = 0
    global availableSelections
    global listOfMacroDirectories
    del availableSelections[:]
    for directory in listOfMacroDirectories:
	counter += 1
        selection = str(counter) + '. ' + directory
	selectionTuple = (counter, directory)
	availableSelections.append(selectionTuple)
        print selection
    print
    #print 'availableSelections:', availableSelections # FIXME: debug purposes


# ----------------------------------------------------------------------#
#                     SHOWLISTOFFILESFOUND() FUNCTION                   #
# ----------------------------------------------------------------------#
def ShowListOfFilesFound(MaGeOrGAT):
    '''Show list of MaGe or GAT files found. Generic version of ShowListOfMacroDirectories
    function. Used for options outside Interactive Mode.'''
    if MaGeOrGAT not in ["MaGe", "GAT"]:
	print "\aERROR: ShowListOfFilesFound() function does not take the string %s!" % MaGeOrGAT
        sys.exit(1)
    kindOfFile = MaGeOrGAT
    typeOfList = []
    MaGeOutputFilesFound = []
    GATOutputFilesFound = []
    print "\nThe following " + kindOfFile + " output files were found in each correctly specified directory..."
    print("=" * 84)
    for directory in listOfMacroDirectories:
	del typeOfList[:]
	del MaGeOutputFilesFound[:]
	del GATOutputFilesFound[:]
	# If bulk mode is activated, search for MaGe or GAT output files in subdirectories as well
	if(options.bulkMode):
	    if(options.runGAT or options.runGATEnd):
	        MaGeOutputFilesFound = LocateFiles("*.root", directory + "/MaGeOutputs", lookInSubdirectories = True)
	        typeOfList = MaGeOutputFilesFound
	    elif(options.analyzeGATOutputs): 
	        GATOutputFilesFound = LocateFiles("ae*.root", directory + "/GATOutputs", lookInSubdirectories = True)
	        typeOfList = GATOutputFilesFound
        else:
	    if(options.runGAT or options.runGATEnd):
                MaGeOutputFilesFound = LocateFiles("*.root", directory + "/MaGeOutputs")
	        typeOfList = MaGeOutputFilesFound
	    elif(options.analyzeGATOutputs): 
                GATOutputFilesFound = LocateFiles("ae*.root", directory + "/GATOutputs")
	        typeOfList = GATOutputFilesFound
        typeOfList.sort()
        number = 0
        print "\033[1mDIRECTORY:\033[0m %s" % directory
        if len(typeOfList) == 0:
           print "None\n"
           continue
        for file in typeOfList:
            number += 1
            output = str(number) + '. ' + file
            print output
        print


# ----------------------------------------------------------------------#
#             BUILDLISTOFMACRODIRECTORIESFROMLOG() FUNCTION             #
# ----------------------------------------------------------------------#
listOfMacroDirectories_stored = []
def BuildListOfMacroDirectoriesFromLog():    
    '''Build list of macro directories from log file so user does not have
    to specify the macro directories again.'''
    if not os.path.isfile(".stored_directories.log"):
        print "\aERROR: The file .stored_directories.log doesn't exist! Exiting script..."
        sys.exit(1)
    print "Building list of macro directories from .stored_directories.log..."
    global listOfMacroDirectories_stored
    logFile = open(".stored_directories.log","r")
    directoryList = []
    for i, directory in enumerate(logFile):
	directory = directory[:-1] # Slice out newline character
        #print 'i: %d  line in logFile: %s' % (i, directory) # FIXME: debug purposes
        if i != 0: # Skip line containing the time
	    directoryList.append(directory)
            if not listOfMacroDirectories_stored:
                listOfMacroDirectories_stored.append(directory)
            if directory not in listOfMacroDirectories_stored:
                listOfMacroDirectories_stored.append(directory)
    if(options.bulkMode):
        if len(directoryList) == 1: 
	    del listOfMacroDirectories_stored[:]
            listOfMacroDirectories_stored.append(directoryList[0])
        else: 
            print "\aERROR: .stored_directories.log contains more than one directory saved!"
            sys.exit(1)
    #print 'listOfMacroDirectories_stored:', listOfMacroDirectories_stored # FIXME: debug purposes
    return listOfMacroDirectories_stored


# ----------------------------------------------------------------------#
#                      CREATEBASICMACRO() FUNCTION                      #
# ----------------------------------------------------------------------#
def CreateBasicMacro():
    '''Create basic macro simulation file.'''
    basicMacro = open("TSBasicMacro.mac", "w")
    content = r'''# Basic settings
/MG/manager/mglog routine
/MG/eventaction/reportingfrequency 100
/MG/manager/seedWithUUID
/MG/processes/realm BBdecay
/MG/processes/lowenergy true

/MG/geometry/WorldMaterial Vacuum

# Select detector
/MG/geometry/detector TSMJDemModInTank

# Set parameters of lab module
# These set the size of the 'lab space' (filled with air) where the tank sits
/MG/geometry/TSLabModuleBox/labheight 15 m
/MG/geometry/TSLabModuleBox/labwidth 15 m
/MG/geometry/TSLabModuleBox/lablength 15 m
/MG/geometry/TSLabModuleBox/rockdepth 3 m
/MG/geometry/TSLabModuleBox/shotcretedepth 0.01 m
/MG/geometry/TSLabModuleBox/rockmaterial Rock
/MG/geometry/TSLabModuleBox/shotcretematerial Rock
/MG/geometry/TSLabModuleBox/labspacematerial Air

# Set parameters of the tank
/MG/geometry/TSCylinderShieldTank/height 6 m
/MG/geometry/TSCylinderShieldTank/radius 3 m
/MG/geometry/TSCylinderShieldTank/shieldmaterial LAB

# Set parameters of the crystal
/MG/geometry/TSMJDemModInTank/CrystalDiameter 77 mm
/MG/geometry/TSMJDemModInTank/CrystalHeight 33 mm
/MG/geometry/TSMJDemModInTank/CrystalsPerString 5

# Configuration, can be MJDemonstrator, 1Ring, 2Ring, 3Ring, 4Ring, 1HEXRing, 2HEXRing
/MG/geometry/TSMJDemModInTank/ModuleType 1Ring

# Set spacing between strings
/MG/geometry/TSMJDemModInTank/RadialSpacingBetweenStrings 5 mm

# Set type of crystal mount (MJDemonstrator, TS)
/MG/geometry/TSMJDemModInTank/CrystalAssemblyType TS

# The number of modules in the array (1-4)
/MG/geometry/TSMJDemModInTank/NumberOfModules 1

# Set general parameters of the ton scale setup:
/MG/geometry/TSMJDemModInTank/TankIsActive false
/MG/geometry/TSMJDemModInTank/TankVerticalOffset 1 m

/MG/eventaction/rootschema MCRun
/MG/eventaction/rootfilename TSBasicMacro.root
/MG/io/MCRun/SetSensitiveIDLabelScheme askGeom
/MG/io/MCRun/setRunID 1
/MG/io/MCRun/useTimeWindow true
/MG/io/MCRun/setTimeWindow 86400 second

/run/initialize
/MG/generator/select RDMiso
/gun/energy 0 eV
#208Tl
/grdm/ion 208 81 0

/MG/generator/confine volume
#/MG/generator/volume CryostatAssembly_001_CryostatHoop_001
#/MG/generator/volume labmodulephysical
#/MG/generator/volume ShotCretePhysical
#/MG/generator/volume LabSpacePhysical # air in the lab
/MG/generator/volume shieldtankphysical

/run/beamOn 10000000

'''
    basicMacro.write(content)
    basicMacro.close()
    print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m Successfully created file TSBasicMacro.mac in current directory."


# ----------------------------------------------------------------------#
#                  CREATEGEOMETRYVIEWERMACRO() FUNCTION                 #
# ----------------------------------------------------------------------#
def CreateGeometryViewerMacro():
    '''Create basic macro to view geometry of detector components.'''
    geometryViewerMacro = open("TSGeometryViewer.mac", "w")
    content = r'''# Basic settings
/MG/manager/mglog routine
#/MG/geometry/detector MJDemonstrator
/MG/geometry/detector TSMJDemModInTank
/MG/processes/useNoHadPhysics true
/MG/processes/realm BBdecay
/MG/processes/lowenergy true

/MG/geometry/WorldMaterial Vacuum

# Set parameters of lab module
/MG/geometry/TSLabModuleBox/labheight 15 m
/MG/geometry/TSLabModuleBox/labwidth 15 m
/MG/geometry/TSLabModuleBox/lablength 15 m
/MG/geometry/TSLabModuleBox/rockdepth 3 m
/MG/geometry/TSLabModuleBox/shotcretedepth 0.1 m
/MG/geometry/TSLabModuleBox/rockmaterial Rock
/MG/geometry/TSLabModuleBox/shotcretematerial Rock
/MG/geometry/TSLabModuleBox/labspacematerial Air

# Set parameters of the tank
/MG/geometry/TSCylinderShieldTank/height 6 m
/MG/geometry/TSCylinderShieldTank/radius 3 m
/MG/geometry/TSCylinderShieldTank/shieldmaterial LAB

# Set parameters of the crystal
/MG/geometry/TSMJDemModInTank/CrystalDiameter 77 mm 
/MG/geometry/TSMJDemModInTank/CrystalHeight 33 mm 
/MG/geometry/TSMJDemModInTank/CrystalsPerString 5

# Configuration, can be MJDemonstrator, 1Ring, 2Ring, 3Ring, 4Ring, 1HEXRing, 2HEXRing
#/MG/geometry/TSMJDemModInTank/ModuleType 1String
#/MG/geometry/TSMJDemModInTank/ModuleType 2Ring
#/MG/geometry/TSMJDemModInTank/ModuleType 1HEXRing
/MG/geometry/TSMJDemModInTank/ModuleType 2HEXRing

# Set radial clearance spacing for strings, added by K. Nguyen
#/MG/geometry/TSMJDemModInTank/StringRadialClearance 5 mm 
#/MG/geometry/TSMJDemModInTank/StringRadialClearance 0.01 mm

# Set spacing between strings, added by K. Nguyen
#/MG/geometry/TSMJDemModInTank/RadialSpacingBetweenStrings 500 mm 
#/MG/geometry/TSMJDemModInTank/RadialSpacingBetweenStrings 0.1 mm 
/MG/geometry/TSMJDemModInTank/RadialSpacingBetweenStrings 5 mm 

# Set type of crystal mount (MJDemonstrator, TS)
/MG/geometry/TSMJDemModInTank/CrystalAssemblyType TS

/run/initialize

# Visualization Driver
/vis/open OGL
#/vis/open OGLSXm
#/vis/open RayTracerX 600x450
#/vis/open RayTracer 600x450
#/vis/open RayTracer 1200x900
#/vis/open RayTracer 2400x1800
#/vis/open RayTracer 4800x3600

# RayTracer Settings
/vis/rayTracer/setShine

# Camera Settings
/vis/viewer/set/upVector 0 0 1
/vis/viewer/set/viewpointThetaPhi 60.0 -65.0    # Angled view
#/vis/viewer/set/viewpointThetaPhi 40.0 0.0     # Top view
#/vis/viewer/set/viewpointThetaPhi -230.0 0.0   # Bottom view
/vis/viewer/set/background white
/vis/viewer/set/lightsThetaPhi 60.0 -90.0
/vis/viewer/zoom 1.1
/vis/viewer/pan 0.0 0.0 m

/vis/scene/create

# Visibility Settings
/vis/geometry/set/visibility all -1 false
#/MG/geometry/SetVisForPhysVolsMatching ActiveCrystal
#/MG/geometry/SetVisForPhysVolsMatching HVRing
#/MG/geometry/SetVisForPhysVolsMatching PTFEBlock
#/MG/geometry/SetVisForPhysVolsMatching Mercedes
/MG/geometry/SetVisForPhysVolsMatching StringAssembly
#/MG/geometry/SetVisForPhysVolsMatching Cryostat

/vis/drawVolume
/vis/viewer/flush

'''
    geometryViewerMacro.write(content)
    geometryViewerMacro.close()
    print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m Successfully created file TSGeometryViewer.mac in current directory."


# ----------------------------------------------------------------------#
#                         FRESHRUN() FUNCTION                           #
# ----------------------------------------------------------------------#
def FreshRun():    
    '''Clean up everything for a fresh run of the script.'''
    softRemoval = False
    hardRemoval = False
    kindOfRemoval = ""
    while(True):
        print r"""
  WARNING: You've specified the -f option. There are two types of removal methods available to users for convenience:
  1. Soft removal: - KEEP contents in /Macros folder and /MaGeOutputs folder but REMOVE all contents in 
                     /GATOutputs and /Graphs folder.
                   - PURPOSE: So user doesn't have to re-write macros and wait for MaGe to complete simulation jobs,
      	             which probably took a long time to complete in the first place if many jobs were involved.
      	             Also, if changes were to be made to this script in the future, the user could just rerun GAT
      	             and analyze those GAT outputs on the same old simulation results.
  2. Hard removal: - REMOVE all files including contents in /MaGeOutputs folder, /GATOutputs/ folder,
                     and /Graphs folder. Any macro files in /BigMacros and /Macros will be restored.
                   - PURPOSE: So user can do a complete rerun of the script.
        """
        removalMethod = raw_input("Which removal method would you like to do? (s = soft; h = hard) --> ")
        if removalMethod.lower() == 's':
	    print "Doing a soft removal of files..."
	    kindOfRemoval = "soft"
	    softRemoval = True
            break
	elif removalMethod.lower() == 'h':
	    print "Doing a hard removal of files..."
	    kindOfRemoval = "hard"
	    hardRemoval = True
	    break
        elif removalMethod.lower() == 'q':
            print "Exiting script..."
            sys.exit(1)
        else:
            print "\aERROR: Invalid response!"
            AskToTryAgain()
    global listOfMacroDirectories
    for directory in listOfMacroDirectories:
	print "Entering directory: %s ..." % directory
	if hardRemoval or softRemoval:
            # .csh files in directory
            cshFilesInDirectory = LocateFiles("*.csh", directory)
            #print 'cshFilesInDirectory:', cshFilesInDirectory # FIXME: debug purposes
            if len(cshFilesInDirectory) > 0: 
                print "Cleaning up .csh files in %s..." % directory
                for file1 in cshFilesInDirectory:
                    try: 
                        echoCommand1 = "echo ...removing file %s..." % file1
		        os.system(echoCommand1)
		    except OSError: pass
                    try: os.remove(file1)
                    except OSError: pass

            # Remove any .csh.e and .csh.o files if ran jobs on PDSF
            listOfCSHErrorFiles = LocateFiles("*.csh.e*", directory)
            #print 'listOfCSHErrorFiles:', listOfCSHErrorFiles # FIXME: debug purposes
            if len(listOfCSHErrorFiles) > 0:
                for errorFile in listOfCSHErrorFiles:
                    try: 
                        echoCommand = "echo ...cleaning up .csh.e file %s..." % errorFile
            	        os.system(echoCommand)
                    except OSError: pass
                    try: os.remove(errorFile)
                    except OSError: pass
            listOfCSHOutputFiles = LocateFiles("*.csh.o*", directory)
            #print 'listOfCSHOutputFiles:', listOfCSHOutputFiles # FIXME: debug purposes
            if len(listOfCSHOutputFiles) > 0:
                for outputFile in listOfCSHOutputFiles:
                    try: 
                        echoCommand = "echo ...cleaning up .csh.o file %s..." % outputFile
            	        os.system(echoCommand)
                    except OSError: pass
                    try: os.remove(outputFile)
                    except OSError: pass

            # GAT output files
            filesInGATOutputsFolder = LocateFiles("*", directory + "/GATOutputs")
            #print 'filesInGATOutputsFolder:', filesInGATOutputsFolder # FIXME: debug purposes
            if len(filesInGATOutputsFolder) > 0: 
                print "Cleaning up files in %s/GATOutputs folder..." % directory
                for file2 in filesInGATOutputsFolder:
                    try: 
                        echoCommand2 = "echo ...removing file %s..." % file2
		        os.system(echoCommand2)
		    except OSError: pass
                    try: os.remove(file2)
                    except OSError: pass
            try: os.rmdir(directory + "/GATOutputs")
            except OSError: pass

            # Files in /Graphs/ErrorPlots folder
            filesInErrorPlotsFolder = LocateFiles("*", directory + "/Graphs/ErrorPlots")
            #print 'filesInErrorPlotsFolder:', filesInErrorPlotsFolder # FIXME: debug purposes
            if len(filesInErrorPlotsFolder) > 0: 
                print "Cleaning up files in %s/Graphs/ErrorPlots folder..." % directory
                for file3 in filesInErrorPlotsFolder:
                    try: 
                        echoCommand3 = "echo ...removing file %s..." % file3
		        os.system(echoCommand3)
		    except OSError: pass
                    try: os.remove(file3)
                    except OSError: pass
            try: os.rmdir(directory + "/Graphs/ErrorPlots")
            except OSError: pass

            # Files in /Graphs folder
            filesInGraphsFolder = LocateFiles("*", directory + "/Graphs")
            #print 'filesInGraphsFolder:', filesInGraphsFolder # FIXME: debug purposes
            if len(filesInGraphsFolder) > 0: 
                print "Cleaning up files in %s/Graphs folder..." % directory
                for file4 in filesInGraphsFolder:
                    try: 
                        echoCommand4 = "echo ...removing file %s..." % file4
		        os.system(echoCommand4)
		    except OSError: pass
                    try: os.remove(file4)
                    except OSError: pass
            try: os.rmdir(directory + "/Graphs")
            except OSError: pass

            # Doesn't really belong in for loop because there is only one JobsSummaryStatus.txt
            # file located in the directory where the script is ran, but for generality it's here.
            jobsSummaryStatusFile = LocateFiles("JobsSummaryStatus.txt", directory)
            if len(jobsSummaryStatusFile) > 0: 
                print "Cleaning up JobsSummaryStatus.txt file in directory %s ..." % directory
                try: 
                    echoCommand5 = "echo ...removing file %s..." % jobsSummaryStatusFile[0]
		    os.system(echoCommand5)
		except OSError: pass
                try: os.remove(jobsSummaryStatusFile[0])
                except OSError: pass

            # Doesn't really belong in for loop because there is only one TSBasicMacro.mac
            # file located in the directory where the script is ran, but for generality it's here.
            TSBasicMacroFile = LocateFiles("TSBasicMacro.mac", directory)
            if len(TSBasicMacroFile) > 0: 
                print "Cleaning up TSBasicMacro.mac file in directory %s ..." % directory
                try: 
                    echoCommand6 = "echo ...removing file %s..." % TSBasicMacroFile[0]
		    os.system(echoCommand6)
		except OSError: pass
                try: os.remove(TSBasicMacroFile[0])
                except OSError: pass

            # Doesn't really belong in for loop because there is only one TSBasicMacro.mac
            # file located in the directory where the script is ran, but for generality it's here.
            TSGeometryViewerFile = LocateFiles("TSGeometryViewer.mac", directory)
            if len(TSGeometryViewerFile) > 0: 
                print "Cleaning up TSGeometryViewer.mac file in directory %s ..." % directory
                try: 
                    echoCommand7 = "echo ...removing file %s..." % TSGeometryViewerFile[0]
		    os.system(echoCommand7)
		except OSError: pass
                try: os.remove(TSGeometryViewerFile[0])
                except OSError: pass

            # Doesn't really belong in for loop because there is only one .stored_directories.log
            # file located in the directory where the script is ran, but for generality it's here.
            directoriesLog = LocateFiles(".stored_directories.log", directory)
            if len(directoriesLog) > 0: 
                print "Cleaning up .stored_directories.log file in directory %s ..." % directory
                try: 
                    echoCommand8 = "echo ...removing file %s..." % directoriesLog[0]
		    os.system(echoCommand8)
		except OSError: pass
                try: os.remove(directoriesLog[0])
                except OSError: pass

	if hardRemoval and not softRemoval:
            # Macro files with "_Part" suffix in directory
            macroFilesInDirectory = LocateFiles("*_Part*.mac", directory)
            #print 'macroFilesInDirectory:', macroFilesInDirectory # FIXME: debug purposes
            if len(macroFilesInDirectory) > 0: 
                print "Cleaning up macro files with suffix _Part in %s..." % directory
                for file9 in macroFilesInDirectory:
                    try: 
                        echoCommand9 = "echo ...removing file %s..." % file9
		        os.system(echoCommand9)
		    except OSError: pass
                    try: os.remove(file9)
                    except OSError: pass
                # Macro files in BigMacros folder
                filesInBigMacrosFolder = LocateFiles("*.mac", directory + "/BigMacros")
                #print 'filesInBigMacrosFolder:', filesInBigMacrosFolder # FIXME: debug purposes
                if len(filesInBigMacrosFolder) > 0: 
                    print "Restoring files in %s/BigMacros folder..." % directory
                    for file10 in filesInBigMacrosFolder:
                        try: 
                            echoCommand10 = "echo ...restoring file %s..." % file10
                            os.system(echoCommand10)
                        except OSError: pass
                        try: shutil.move(file10, directory)
                        except: 
                            if file10.endswith("/"): file10 = file10[:-1]
                            lastForwardSlash = file10.rfind('/')
                            basePathName = file10[:lastForwardSlash]
                            brokenPath = file10.split('/')
                            filename = brokenPath[-1]
                            try:
                                if os.path.isfile(basePathName + "/" + filename):
                                    os.remove(basePathName + "/" + filename)
                                shutil.move(file10, directory)
                            except: pass        
                try: os.rmdir(directory + "/BigMacros")
                except OSError: pass

            # Macro files in Macros folder
            filesInMacrosFolder = LocateFiles("*.mac", directory + "/Macros")
            #print 'filesInMacrosFolder:', filesInMacrosFolder # FIXME: debug purposes
            if len(filesInMacrosFolder) > 0: 
                # Macro files with "_Part" suffix in directory
                macroFilesInDirectory = LocateFiles("*_Part*.mac", directory + "/Macros")
                #print 'macroFilesInDirectory:', macroFilesInDirectory # FIXME: debug purposes
                if len(macroFilesInDirectory) > 0: 
                    print "Cleaning up macro files with suffix _Part in %s/Macros..." % directory
                    for file11 in macroFilesInDirectory:
                        try: 
                            echoCommand11 = "echo ...removing file %s..." % file11
                            os.system(echoCommand11)
                        except OSError: pass
                        try: os.remove(file11)
                        except OSError: pass
                filesInMacrosFolder = LocateFiles("*.mac", directory + "/Macros")
		if len(filesInMacrosFolder) > 0:
                    print "Restoring files in %s/Macros folder..." % directory
                    for file12 in filesInMacrosFolder:
                        try: 
                            echoCommand12 = "echo ...restoring file %s..." % file12
                            os.system(echoCommand12)
                        except OSError: pass
                        try: shutil.move(file12, directory)
                        except: 
                            if file12.endswith("/"): file12 = file12[:-1]
                            lastForwardSlash = file12.rfind('/')
                            basePathName = file12[:lastForwardSlash]
                            brokenPath = file12.split('/')
                            filename = brokenPath[-1]
                            try:
                                if os.path.isfile(basePathName + "/" + filename):
                                    os.remove(basePathName + "/" + filename)
                                shutil.move(file10, directory)
                            except: pass        
                try: os.rmdir(directory + "/Macros")
                except OSError: pass

            # MaGe outputs files
            if os.path.exists(directory + "/MaGeOutputs"):
                filesInMaGeOutputsFolder = LocateFiles("*", directory + "/MaGeOutputs")
                #print 'filesInMaGeOutputsFolder:', filesInMaGeOutputsFolder # FIXME: debug purposes
                if len(filesInMaGeOutputsFolder) > 0: 
                    print "Cleaning up files in %s/MaGeOutputs folder..." % directory
                    for file13 in filesInMaGeOutputsFolder:
                        try: 
                            echoCommand13 = "echo ...removing file %s..." % file13
                            os.system(echoCommand13)
                        except OSError: pass
                        try: os.remove(file13)
                        except OSError: pass
                try: os.rmdir(directory + "/MaGeOutputs")
                except OSError: pass

    print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m Done doing a " + kindOfRemoval + \
          " removal of all relevant folders in the directories specified."


# ----------------------------------------------------------------------#
#                CHECKMACROFILEFORMANAGABILITY() FUNCTION               #
# ----------------------------------------------------------------------#
def CheckMacroFileForManagability(macroFilename, splitMacroFile = True):
    '''Check how many events are simulated in macro file and breaks
    the macro file up if this number is greater than 10 million. This 
    number can be changed in the code. The function splits big macro
    files up by default.'''
    if splitMacroFile:
        if macroFilename.endswith("/"): macroFilename = macroFilename[:-1]
        lastForwardSlash = macroFilename.rfind('/')
        basePathName = macroFilename[:lastForwardSlash]
        brokenPath = macroFilename.split('/')
        filename = brokenPath[-1]
        extension = filename.find(".mac")
        basename = filename[:extension]
        # Determine number of files to break macro file into
        # if number of simulated event is over 10 million
        nFiles = 0
        eventsLimit = 10000000
        numberTotalEvents = 0
        remainingEvents = 0
        macroFile = open(macroFilename, "r")
        for line in macroFile:
            if not line.startswith("#") and line.find("/run/beamOn") > -1:
        	splitLine = line.split()
                if len(splitLine) == 2:
        	    numberTotalEvents = int(splitLine[1])
        	    if numberTotalEvents > eventsLimit:
        	        #print "numberTotalEvents is greater than %d" % eventsLimit # FIXME: debug purposes
                        nFiles = numberTotalEvents/float(eventsLimit)
        	        #print "nFiles raw: ", nFiles # FIXME: debug purposes
        	        remainingEvents = int( numberTotalEvents - ( int(math.floor(nFiles)) * eventsLimit ) )
        	        #print 'remainingEvents: ', remainingEvents # FIXME: debug purposes
                        if numberTotalEvents % eventsLimit != 0: nFiles = math.ceil(nFiles)
            	        nFiles = int(nFiles)
        	        #print "nFiles adjusted: ", nFiles # FIXME: debug purposes
        macroFile.close()		
        if nFiles > 1:
            eventsWritten = 0
            for i in range(0, nFiles):
                macroFile = open(macroFilename, "r")
                newBasename = basename + "_Part" + str(i+1)
                newMacro = open(newBasename + ".mac", "w")
                eventsWritten += eventsLimit
                for line in macroFile:
            	    if not line.startswith("#"):
                        if line.find("/MG/eventaction/rootfilename") > -1:
                            line = "/MG/eventaction/rootfilename " + newBasename + ".root\n"
                        if line.find("/run/beamOn") > -1: 
                            if eventsWritten <= numberTotalEvents:
                                line = "/run/beamOn " + str(eventsLimit) + "\n"
                            else:
                                line = "/run/beamOn " + str(remainingEvents) + "\n" 
                    newMacro.write(line)
                newMacro.close()
                macroFile.close()
            try: os.mkdir(basePathName + "/BigMacros")
            except OSError: pass
            try: shutil.move(macroFilename, basePathName + "/BigMacros")
            except: 
                try:
                    if os.path.isfile(basePathName + "/BigMacros/" + filename):
                        os.remove(basePathName + "/BigMacros/" + filename)
                    shutil.move(macroFilename, basePathName + "/BigMacros")
                except: pass        


# ----------------------------------------------------------------------#
#                      CREATECSHFILES() FUNCTION                        #
# ----------------------------------------------------------------------#
def CreateCSHFiles():
    '''Create .csh files for all .mac files for each macro directory.'''
    print 'Creating .csh files for all macro files specified...'
    global listOfMacroFiles
    numberFiles = 0
    for macro in listOfMacroFiles:
	if macro.endswith("/"): macro = macro[:-1]
        lastForwardSlash = macro.rfind('/')
        basePathName = macro[:lastForwardSlash]
	brokenPath = macro.split('/')
	filename = brokenPath[-1]
        extension = filename.find(".mac")
        basename = filename[:extension]
        scriptName = basename + ".csh"
	print "...creating %s/%s..." % (basePathName, scriptName)
        cshFile = open(scriptName, "w")
        cshFile.write("#!/bin/csh\n")
        cshFile.write("MaGe " + macro + "\n")
        cshFile.close()
        numberFiles += 1
        # Create a /Macros folder for each macro directory specified
        try: os.mkdir(basePathName +"/Macros")
        except OSError: pass
        # Collect only single macro files and move them to /Macros directory
        print "...copying macro file %s to %s/Macros directory..." % (macro, basePathName)
	try: shutil.copy2(macro, basePathName + "/Macros")
	except: pass
    print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m %d .csh files have been created for all macro files found." % numberFiles


# ----------------------------------------------------------------------#
#                        SUBMITJOBS() FUNCTION                          #
# ----------------------------------------------------------------------#
listOfGoodJobs = []
def SubmitJobs():
    '''Submit simulation jobs using qsub for each macro directory.'''
    print 'Submitting simulation jobs...'
    numberBadJobs = 0
    numberGoodJobs = 0
    numberRerunJobs = 0
    listOfBadJobs = []
    listOfJobsToRerun = []
    global listOfGoodJobs
    global listOfMacroDirectories
    for directory in listOfMacroDirectories:
	print "Entering directory: %s ..." % directory
        for file in LocateFiles("*.csh", directory):
            try: 
                echoCommand = "echo ...submitting file %s" % file
	        os.system(echoCommand)
            except OSError: pass
            qsubCommand = "qsub %s" % file
            if os.system(qsubCommand) != 0:
	        listOfJobsToRerun.append(file)
                numberRerunJobs += 1
                continue
            listOfGoodJobs.append(file)
            numberGoodJobs += 1
	if len(listOfJobsToRerun) > 0:
	    print "Resubmitting %d jobs that failed to submit in this directory the first time for whatever reason..." % numberRerunJobs
            for rerun in listOfJobsToRerun:
                echoCommand = "echo ...resubmitting file %s" % rerun
                qsubCommand = "qsub %s" % file
                try: os.system(echoCommand)
		except OSError: pass
                if os.system(qsubCommand) != 0:
                    print "ERROR...could not submit job for the following file: %s" % rerun
                    listOfBadJobs.append(file)
                    numberBadJobs += 1
                    continue
        jobsSummaryStatusFile = open("JobsSummaryStatus.txt", "w")
        jobsSummaryStatusFile.write("=" * 84 + "\n")
        jobsSummaryStatusFile.write(" " * 31 + "JOBS THAT SUCCEEDED" + " " * 31 + "\n")
        jobsSummaryStatusFile.write("=" * 84 + "\n")
        for goodJob in listOfGoodJobs:
            jobsSummaryStatusFile.write("qsub " + goodJob + '\n')
        jobsSummaryStatusFile.write("Count: %d\n" % len(listOfGoodJobs))
        jobsSummaryStatusFile.write('\n')
        jobsSummaryStatusFile.write("=" * 84 + "\n")
        jobsSummaryStatusFile.write(" " * 32 + "JOBS THAT FAILED" + " " * 32 + "\n")
        jobsSummaryStatusFile.write("=" * 84 + "\n")
        for badJob in listOfBadJobs:
            jobsSummaryStatusFile.write("qsub " + badJob + '\n')
        jobsSummaryStatusFile.write("Count: %d\n" % len(listOfBadJobs))
	jobsSummaryStatusFile.write('\n')
        jobsSummaryStatusFile.close()
    print("=" * 100)
    print(" " * 40 + "\033[4m\033[1mSTATUS SUMMARY\033[0m\033[0m")
    print "Statuses of all jobs have been written to a 'JobsStatusSummary.txt' file located in each directory."
    print "SUCCESSES: %d total jobs have been submitted successfully." % numberGoodJobs
    print "FAILURES: %d total jobs failed to submit." % numberBadJobs
    print("=" * 100)


# ----------------------------------------------------------------------#
#                      CLEANUPCSHFILES() FUNCTION                       #
# ----------------------------------------------------------------------#
def CleanUpCSHFiles():
    '''Clean up all .csh files for jobs that completed successfully.'''
    print 'Cleaning up .csh files for jobs that ran successfully...'
    numberFilesRemoved = 0
    if len(listOfGoodJobs) > 0:
        for job in listOfGoodJobs:
            echoCommand = "echo ...cleaning up .csh file %s..." % job
	    try: os.system(echoCommand)
	    except OSError: pass
	    try: os.remove(job)
	    except OSError: pass
            numberFilesRemoved += 1
    print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m %d .csh files have been removed." % numberFilesRemoved


# ----------------------------------------------------------------------#
#                     COLLECTMAGEOUTPUTS() FUNCTION                     #
# ----------------------------------------------------------------------#
def CollectMaGeOutputs():
    '''Collect all MaGe .root files and put them in /MaGeOutputs folder for each macro directory.'''
    print "Collecting all MaGe outputs..."
    global listOfMacroDirectories
    for directory in listOfMacroDirectories:
	print "Entering directory: %s ..." % directory
	# Remove any .csh.e and .csh.o files if ran jobs on PDSF
	listOfCSHErrorFiles = LocateFiles("*.csh.e*")
	if len(listOfCSHErrorFiles) > 0:
            for errorFile in listOfCSHErrorFiles:
                try: 
                    echoCommand = "echo ...cleaning up .csh.e file %s..." % errorFile
		    os.system(echoCommand)
                except OSError: pass
                try: os.remove(errorFile)
                except OSError: pass
	listOfCSHOutputFiles = LocateFiles("*.csh.o*")
	if len(listOfCSHOutputFiles) > 0:
            for outputFile in listOfCSHOutputFiles:
                try: 
                    echoCommand = "echo ...cleaning up .csh.o file %s..." % outputFile
		    os.system(echoCommand)
                except OSError: pass
                try: os.remove(outputFile)
                except OSError: pass
        listOfROOTFiles = LocateFiles("*.root", directory)
        try: os.mkdir(directory + "/MaGeOutputs")
        except OSError: pass
        numberFiles = 0
        for ROOTFile in listOfROOTFiles:
            if ROOTFile.find("ae_") > -1: continue
	    try: shutil.move(ROOTFile, directory + "/MaGeOutputs")
	    except: 
                if ROOTFile.endswith("/"): ROOTFile = ROOTFile[:-1]
                lastForwardSlash = ROOTFile.rfind('/')
                basePathName = ROOTFile[:lastForwardSlash]
                brokenPath = ROOTFile.split('/')
                filename = brokenPath[-1]
		try:
		    if os.path.isfile(basePathName + "/MaGeOutputs/" + filename):
		        os.remove(basePathName + "/MaGeOutputs/" + filename)
	            shutil.move(ROOTFile, directory + "/MaGeOutputs")
		except: pass        
            numberFiles += 1
        print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m %d files have been moved to the /MaGeOutputs folder." % numberFiles


# ----------------------------------------------------------------------#
#                           RUNGAT() FUNCTION                           #
# ----------------------------------------------------------------------#
def RunGAT():
    '''Run GAT on all MaGe output files found in /MaGeOutputs folder for each macro directory.'''
    print 'Running GAT on all MaGe outputs...'
    global listOfMacroDirectories
    for directory in listOfMacroDirectories:
	print "Entering directory: %s ..." % directory
        if not os.path.exists(directory + "/MaGeOutputs"):
	    print "...%s/MaGeOutputs folder doesn't exist!" % directory 
	    print "Make sure you've put all your MaGe outputs in a folder called 'MaGeOutputs'",
	    print "if you used the option --run or --runEnd."
	    print "Moving on to next directory if any..."
	    continue
        listOfROOTFiles = LocateFiles("*.root", directory + "/MaGeOutputs")
        #print 'listOfROOTFiles:', listOfROOTFiles # FIXME: debug purposes
        if len(listOfROOTFiles) == 0:
            print "No .root files found in %s/MaGeOutputs folder. Moving on to next directory if any..." % directory
            continue
        numberFilesRan = 0
        for ROOTFile in listOfROOTFiles:
	    # Make sure we don't run GAT on a file that has already been processed
            if ROOTFile.find("ae_") > -1: continue
            runScriptCommand = "python $GATDIR/Scripts/process_demo_mage_results.py %s" % ROOTFile
            if os.system(runScriptCommand) != 0:
                print "ERROR...could not run GAT on %s." % ROOTFile
                continue
            numberFilesRan += 1
        print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m Ran GAT on %d file(s) successfully." % numberFilesRan
	if numberFilesRan > 0:
            try: os.mkdir(directory + "/GATOutputs")
            except OSError: pass
            print 'Collecting all GAT outputs...'
            numberFilesMoved = 0
            for GATFile in LocateFiles("*ae_*.root"):
            #for GATFile in LocateFiles("*ae_*", directory):
		try: shutil.move(GATFile, directory + "/GATOutputs")
	        except: 
                    if GATFile.endswith("/"): GATFile = GATFile[:-1]
                    lastForwardSlash = GATFile.rfind('/')
                    basePathName = GATFile[:lastForwardSlash]
                    brokenPath = GATFile.split('/')
                    filename = brokenPath[-1]
	            try:
	                if os.path.isfile(directory + "/GATOutputs/" + filename):
	                    os.remove(directory + "/GATOutputs/" + filename)
		        shutil.move(GATFile, directory + "/GATOutputs")
	            except: pass        
                numberFilesMoved += 1
            print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m %d file(s) has/have been moved to the %s/GATOutputs folder." \
	           % (numberFilesMoved, directory)


# ----------------------------------------------------------------------#
#                       CREATETCHAINS() FUNCTION                        #
# ----------------------------------------------------------------------#
def CreateTChains(GATFiles):
    '''Check if macro files have been broken up and combine them if necessary.
    Return list of TChains.'''
    listOfTChains = []
    newOriginalName = ""
    storedOriginalName = ""
    nTimes = 0
    GATFiles.sort()
    for i, file in enumerate(GATFiles):
        if file.endswith("/"): file = file[:-1]
        lastForwardSlash = file.rfind('/')
        basePathName = file[:lastForwardSlash]
        brokenPath = file.split('/')
        filename = brokenPath[-1]
        extension = filename.find(".root")
        basename = filename[:extension]
	#print 'filename: %s    basename: %s' % (filename, basename) # FIXME: debug purposes
	if i == 0: chainOfFiles = TChain("fTree")
	if i == len(GATFiles) - 1:
	    if nTimes >= 1: chainOfFiles.SetTitle(newOriginalName)
	    else: chainOfFiles.SetTitle(basename)
	    chainOfFiles.AddFile(file)
	    listOfTChains.append(chainOfFiles)
            entriesAdded = chainOfFiles.GetListOfFiles().GetEntries()
            #print 'End of GATFiles   entriesAdded 1: ', entriesAdded # FIXME: debug purposes
	    break
        if basename.find("_Part") > -1:
	    nTimes += 1
	    found = basename.find("_Part")
	    if nTimes == 1: 
                storedOriginalName = newOriginalName = basename[:found]
		chainOfFiles.SetTitle(newOriginalName)
		chainOfFiles.AddFile(file)
		continue
	    newOriginalName = basename[:found]    
	    #print 'storedOriginalName: %s    newOriginalName: %s' % (storedOriginalName, newOriginalName) # FIXME: debug purposes
            if newOriginalName == storedOriginalName: chainOfFiles.AddFile(file)
            else: 
                #entriesAdded = chainOfFiles.GetListOfFiles().GetEntries()
                #print 'entriesAdded 2: ', entriesAdded # FIXME: debug purposes
	        listOfTChains.append(chainOfFiles)
		chainOfFiles = TChain("fTree")
		storedOriginalName = newOriginalName
		nTimes = 1
		chainOfFiles.SetTitle(newOriginalName)
		chainOfFiles.AddFile(file)
		continue
                #entriesAdded = chainOfFiles.GetListOfFiles().GetEntries()
                #print 'entriesAdded: ', entriesAdded
        else:
	    if nTimes > 1:
                #entriesAdded = chainOfFiles.GetListOfFiles().GetEntries()
                #print 'entriesAdded 3: ', entriesAdded # FIXME: debug purposes
	        listOfTChains.append(chainOfFiles)
		chainOfFiles = TChain("fTree")
	        nTimes = 0
	    chainOfFiles.SetTitle(basename)
	    chainOfFiles.AddFile(file)
	    listOfTChains.append(chainOfFiles)
            entriesAdded = chainOfFiles.GetListOfFiles().GetEntries()
            #print 'entriesAdded 4: ', entriesAdded # FIXME: debug purposes
	    chainOfFiles = TChain("fTree")
    #print 'len of listOfTChains:', len(listOfTChains) # FIXME: debug purposes
    #for chain in listOfTChains:
    #    print 'chain title: %s   chain entries: %d' %  (chain.GetTitle(), chain.GetEntries())
    #    answer = raw_input("Press any key to continue")
    #sys.exit(0)
    return listOfTChains


# ----------------------------------------------------------------------#
#                      CREATEERRORPLOT() FUNCTION                       #
# ----------------------------------------------------------------------#
def CreateErrorPlot(filename, totalCountsNoCuts, efficiency, GranOrPSA = "Gran"):
    '''Create error plot of totalCountsNoCuts based on given efficiency and
    return the error.'''
    if filename.find(".") > -1: 
	dot = filename.find(".")
        filename = filename[:dot]
    if GranOrPSA not in ["Gran", "PSA"]: 
        print "\aERROR: CreateErrorPlot() does not take the string %s!" % GranOrPSA
	sys.exit(1)
    canvas = TCanvas()
    canvas.SetFillColor(0)
    gStyle.SetStatFontSize(0.03)
    gStyle.SetTitleFillColor(0)
    gStyle.SetOptFit(1111)
    fit = TF1("fit","gaus(0)", 0, 1)
    gaus = TF1("gaus","gaus", 0, 1)
    histogram = TH1D("Efficiency Plot", "X/Y", 200, 0, 1)
    histogram.SetTitle(filename)
    histogram.GetXaxis().SetTitle("X/Y")
    histogram.GetYaxis().SetTitle("Counts")
    for i in range(0, 50000):
    #for i in range(0, 1000000):
        totalCounts = gRandom.Poisson(totalCountsNoCuts)
	countsWithCut = gRandom.Binomial(totalCounts, efficiency)
	ratio = countsWithCut/float(totalCounts)
        value = histogram.Fill(countsWithCut/float(totalCounts))
	#print "i: %d   totalCounts: %f   countsWithCut: %f   ratio: %f   filled value: %f" % (i, totalCounts, countsWithCut, ratio, value)
    histogram.Fit(gaus, "q", "", 0, 1)
    histogram.Draw()
    error = gaus.GetParameter(2)
    canvas.Print(filename + "_ErrorPlot" + GranOrPSA + ".png")
    return error


# ----------------------------------------------------------------------#
#                     ANALYZEGATOUTPUTS() FUNCTION                      #
# ----------------------------------------------------------------------#
def AnalyzeGATOutputs(elements = "Thallium-208", xMinInMeVForIntegration = 2, xMaxInMeVForIntegration = 2.1):
    '''Analyze all GAT output files found in /GATOutputs folder for each macro directory.
    Takes three parameters: (1) String listing elements, (2) minimum x value for calculating
    integral, and (3) maximum x value for calculating integral.'''
    print 'Analyzing all GAT ae_*.root output files...'
    listOfGATOutputs = []
    global listOfMacroDirectories
    for directory in listOfMacroDirectories:
	print "Entering directory: %s ..." % directory
        if not os.path.exists(directory + "/GATOutputs"):
	    print "...%s/GATOutputs folder doesn't exist!" % directory 
	    print "Make sure you've put all your GAT outputs in a folder called 'GATOutputs'",
	    print "if you used the option --analyze."
	    print "Moving on to next directory if any..."
	    continue
	del listOfGATOutputs[:]
        listOfGATOutputs = LocateFiles("ae_*.root", directory + "/GATOutputs")
        listOfGATOutputs.sort()
        #print 'listOfGATOutputs:', listOfGATOutputs # FIXME: debug purposes
	listOfReturnedTChains = CreateTChains(listOfGATOutputs)
	#canvas = TCanvas()
	#for iChain in listOfReturnedTChains:
	#    iChain.Draw("fTotalEnergy", "", "")
	#    canvas.Update()
        #    answer = raw_input("Press any key to continue")
        #sys.exit(0)
        print("=" * 127)
        print(" " * 54 + "\033[1mSUMMARY OF ANALYSES\033[0m" + " " * 54)
        print("=" * 127)
        print("FILENAME (Truncated if longer               COUNTS           COUNTS w/ GRAN   COUNTS w/ GRAN    EFFICIENCY      EFFICIENCY GRAN")
        print("than 40 characters long)                    IN ROI           CUT IN ROI       PSA CUT IN ROI    GRAN CUT(%)     +PSA CUT(%)    ")
        print("+" * 127)
        summaryOfAnalysesFile = open("SummaryOfAnalyses.txt","w")
        summaryOfAnalysesFile.write("=" * 127 + "\n")
        summaryOfAnalysesFile.write(" " * 54 + "SUMMARY OF ANALYSES" + " " * 54 + "\n")
        summaryOfAnalysesFile.write("=" * 127 + "\n")
        summaryOfAnalysesFile.write("FILENAME (Truncated if longer"+ " " * 15 + "COUNTS" + " " * 11 + "COUNTS w/ GRAN   COUNTS w/ GRAN    EFFICIENCY      EFFICIENCY GRAN\n")
        summaryOfAnalysesFile.write("than 40 characters long)" + " " * 20 + "IN ROI           CUT IN ROI       PSA CUT IN ROI    GRAN CUT(%)     +PSA CUT(%)\n")
        summaryOfAnalysesFile.write("+" * 127 + "\n")
        numberFiles = 0
	for iChain in listOfReturnedTChains:
            # Filename manipulations
	    filename = iChain.GetTitle()
            aeExtension = filename.find("ae_")
            basename = filename[aeExtension+3:] # aeExtension+3 is after 'ae_'
            # Create canvas, set log scale y, and make entries box disappear
            canvas = TCanvas()
            canvas.SetFillColor(0)
            canvas.SetLogy(1)
            gStyle.SetOptStat(0)
            gStyle.SetTitleFillColor(0)
            # Draw commands
            iChain.Draw("fTotalEnergy >> black(3500, 0, 3.5)", "", "")
            iChain.SetLineColor(632) # 632 = color red
            iChain.Draw("fTotalEnergy >> red(3500, 0, 3.5)", "fNWaveforms == 1", "SAME")
            iChain.SetLineColor(600) # 600 = color blue
            iChain.Draw("fTotalEnergy >> blue(3500, 0, 3.5)", "fNWaveforms == 1 && fAnalysisEventDataVector.fIsMultiSite == 0", "SAME")
            # Calculate number of counts in ROI
	    divisor = float( ((xMaxInMeVForIntegration - xMinInMeVForIntegration) * 1000)/4 ) # Determine divisor for 4 keV ROI
            blackGraph = gROOT.FindObject("black")
            binMin = blackGraph.FindBin(xMinInMeVForIntegration)
            binMax = blackGraph.FindBin(xMaxInMeVForIntegration)
            countsNoCuts = blackGraph.Integral(binMin, binMax, "")
            countsNoCutsROI = countsNoCuts/divisor
            # Calculate number of counts in ROI after applying granularity cut
            redGraph = gROOT.FindObject("red")
            binMinCut = redGraph.FindBin(xMinInMeVForIntegration)
            binMaxCut = redGraph.FindBin(xMaxInMeVForIntegration)
            countsWithCut = redGraph.Integral(binMinCut, binMaxCut, "")
            countsWithCutROI = countsWithCut/divisor
            # Calculate number of counts in ROI after applying granularity and PSA cut
            blueGraph = gROOT.FindObject("blue")
            binMinPSACut = blueGraph.FindBin(xMinInMeVForIntegration)    # FIXME: Doing this for Thallium 208. Need to change
            binMaxPSACut = blueGraph.FindBin(xMaxInMeVForIntegration)    # if simulating a different decay chain!
            countsWithPSACut = blueGraph.Integral(binMinPSACut, binMaxPSACut, "")
            countsWithPSACutROI = countsWithPSACut/divisor
            # Calculate efficiency of cuts
            efficiencyOfGranCut = countsWithCutROI/countsNoCutsROI
	    efficiencyOfGranAndPSACut = countsWithPSACutROI/countsNoCutsROI
	    #efficiencyOfPSAGivenGranCut = countsWithPSACutROI/countsWithCutROI
	    # Calculate errors of cuts
            errorNoCuts = math.sqrt(countsNoCuts)/divisor
            errorWithCut = math.sqrt(countsWithCut)/divisor
            errorWithPSACut = math.sqrt(countsWithPSACut)/divisor
            # Set axis titles
            blackGraph.SetTitle(basename)
            blackGraph.SetXTitle("Energy [MeV]")
            blackGraph.SetYTitle("Counts")
            # Add legend    
            legend = TLegend(0.35, 0.76, 0.59, 0.90)
            legend.SetBorderSize(0)
            legend.SetFillStyle(0)
            legend.SetFillColor(0)
            legend.SetTextSize(0.028)
            blackGraphDescription = "Summed energy (%.1f +/- %.1f counts in ROI)" % (countsNoCutsROI, errorNoCuts)
            redGraphDescription = "After Granularity (%.1f +/- %.1f counts in ROI)" % (countsWithCutROI, errorWithCut)
            blueGraphDescription = "After Granularity+PSA (%.1f +/- %.1f counts in ROI)" % (countsWithPSACutROI, errorWithPSACut)
            legend.AddEntry(blackGraph, blackGraphDescription, "l")
            legend.AddEntry(redGraph, redGraphDescription, "l")
            legend.AddEntry(blueGraph, blueGraphDescription, "l")
            legend.Draw("SAME")
            # Print out graph
            canvas.Print(basename + ".png")
            numberFiles += 1
	    # Calculate errors of efficiency of cuts
	    errorOfEfficiencyOfGranCut = CreateErrorPlot(basename, countsNoCuts, efficiencyOfGranCut, "Gran")
	    errorOfEfficiencyOfGranAndPSACut = CreateErrorPlot(basename, countsNoCuts, efficiencyOfGranAndPSACut, "PSA")
	    #errorOfEfficiencyOfGranCut = efficiencyOfGranCut * math.sqrt( pow((errorWithCut/countsWithCutROI), 2) + \
            #                             pow((errorNoCuts/countsNoCutsROI), 2) )
	    #errorOfEfficiencyOfPSAGivenGranCut = efficiencyOfPSAGivenGranCut * math.sqrt( pow((errorWithPSACut/countsWithPSACutROI), 2) + \
            #                                     pow((errorWithCut/countsWithCutROI), 2) )
            # Format strings to look nice
            countsNoCutsROIString = "%.1f" % countsNoCutsROI
            countsWithCutROIString = "%.1f" % countsWithCutROI
            countsWithPSACutROIString = "%.1f" % countsWithPSACutROI
            errorNoCutsString = "%.1f" % errorNoCuts
            errorWithCutString = "%.1f" % errorWithCut
            errorWithPSACutString = "%.1f" % errorWithPSACut
            efficiencyOfGranCutString = "%.1f" % (efficiencyOfGranCut * 100)
            efficiencyOfGranAndPSACutString = "%.1f" % (efficiencyOfGranAndPSACut * 100)
            errorOfEfficiencyOfGranCutString = "%.1f" % (errorOfEfficiencyOfGranCut * 100)
            errorOfEfficiencyOfGranAndPSACutString = "%.1f" % (errorOfEfficiencyOfGranAndPSACut * 100)
            #errorOfEfficiencyOfPSAGivenGranCutString = "%.1f" % (errorOfEfficiencyOfPSAGivenGranCut * 100)
            # Neat trick to write nice line of data: Redirect print's output to screen,
            # store it in a string, then write it to file 
	    filename += ".root"
	    if len(filename) > 40:       # Truncate filename just for purpose of writing to screen and table
	        filename = filename[:40]
            old_stdout = sys.stdout
	    sys.stdout = mystdout = StringIO()
	    print filename.ljust(43), (countsNoCutsROIString + "+/-" + errorNoCutsString).ljust(16), \
                  (countsWithCutROIString + "+/-" + errorWithCutString).ljust(16), \
                  (countsWithPSACutROIString + "+/-" + errorWithPSACutString).ljust(17), \
                  (efficiencyOfGranCutString + "+/-" + errorOfEfficiencyOfGranCutString).ljust(15), \
		  (efficiencyOfGranAndPSACutString + "+/-" + errorOfEfficiencyOfGranAndPSACutString).ljust(15)
            sys.stdout = old_stdout
	    data = mystdout.getvalue()
	    data = data[:-1] # Get rid of newline character
	    print data # Print out summary of analyses to screen 
            # Write results to output file
            summaryOfAnalysesFile.write(data + "\n")
            # Make Graphs folder to put all .png's in it
            try: os.mkdir(directory + "/Graphs")
            except OSError: pass
	    pngFile = LocateFiles(basename + ".png")
            if len(pngFile) > 0:
                try: 
		    shutil.move(pngFile[0], directory + "/Graphs")
                except: 
                    if pngFile[0].endswith("/"): pngFile[0] = (pngFile[0])[:-1]
                    lastForwardSlashNew = pngFile[0].rfind('/')
                    basePathNameNew = (pngFile[0])[:lastForwardSlashNew]
                    brokenPathNew = pngFile[0].split('/')
                    filenameNew = brokenPathNew[-1]
                    try:
                        if os.path.isfile(directory + "/Graphs/" + filenameNew):
                            os.remove(directory + "/Graphs/" + filenameNew)
                        shutil.move(pngFile[0], directory + "/Graphs")
                    except: pass
	    # Make ErrorPlots folder to put all error plots in it
            try: os.mkdir(directory + "/Graphs/ErrorPlots")
            except OSError: pass
	    errorGranPlot = LocateFiles(basename + "_ErrorPlotGran.png")
            if len(errorGranPlot) > 0:
                try: shutil.move(errorGranPlot[0], directory + "/Graphs/ErrorPlots")
                except: 
                    if errorGranPlot[0].endswith("/"): errorGranPlot[0] = (errorGranPlot[0])[:-1]
                    lastForwardSlashNew = errorGranPlot[0].rfind('/')
                    basePathNameNew = (errorGranPlot[0])[:lastForwardSlashNew]
                    brokenPathNew = errorGranPlot[0].split('/')
                    filenameNew = brokenPathNew[-1]
                    try:
                        if os.path.isfile(directory + "/Graphs/ErrorPlots/" + filenameNew):
                            os.remove(directory + "/Graphs/ErrorPlots/" + filenameNew)
                        shutil.move(errorGranPlot[0], directory + "/Graphs/ErrorPlots")
                    except: pass
	    errorPSAPlot = LocateFiles(basename + "_ErrorPlotPSA.png")
            if len(errorPSAPlot) > 0:
                try: shutil.move(errorPSAPlot[0], directory + "/Graphs/ErrorPlots")
                except: 
                    if errorPSAPlot[0].endswith("/"): errorPSAPlot[0] = (errorPSAPlot[0])[:-1]
                    lastForwardSlashNew = errorPSAPlot[0].rfind('/')
                    basePathNameNew = (errorPSAPlot[0])[:lastForwardSlashNew]
                    brokenPathNew = errorPSAPlot[0].split('/')
                    filenameNew = brokenPathNew[-1]
                    try:
                        if os.path.isfile(directory + "/Graphs/ErrorPlots/" + filenameNew):
                            os.remove(directory + "/Graphs/ErrorPlots/" + filenameNew)
                        shutil.move(errorPSAPlot[0], directory + "/Graphs/ErrorPlots")
                    except: pass
	summaryOfAnalysesFile.write("WARNING: Efficiency errors of granularity cuts are not reliable if counts in ROI are less than 1.6.\n")
	summaryOfAnalysesFile.write("\n")
        summaryOfAnalysesFile.close()
	summaryOfAnalysesFileFound = LocateFiles("SummaryOfAnalyses.txt")
        if len(summaryOfAnalysesFileFound) > 0:
            try: shutil.move(summaryOfAnalysesFileFound[0], directory + "/Graphs")
            except: 
                try:
                    if os.path.isfile(directory + "/Graphs/SummaryOfAnalyses.txt"):
                        os.remove(directory + "/Graphs/SummaryOfAnalyses.txt")
                    shutil.move(summaryOfAnalysesFileFound[0], directory + "/Graphs")
                except: pass
	print "WARNING: Efficiency errors of granularity cuts are not reliable if counts in ROI are less than 1.6."
        print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m %d files have been analyzed successfully for %s in this directory!" \
               % (numberFiles, elements)
        print "Please look in the %s/Graphs directory for your plots." % directory

#def AnalyzeGATOutputs():
#    '''Analyze all GAT output files found in /GATOutputs folder for each macro directory.'''
#    print 'Analyzing all GAT ae_*.root output files...'
#    global listOfMacroDirectories
#    for directory in listOfMacroDirectories:
#	print "Entering directory: %s ..." % directory
#        if not os.path.exists(directory + "/GATOutputs"):
#	    print "...%s/GATOutputs folder doesn't exist!" % directory 
#	    print "Make sure you've put all your GAT outputs in a folder called 'GATOutputs'",
#	    print "if you used the option --analyze."
#	    print "Moving on to next directory if any..."
#	    continue
#        listOfGATOutputs = LocateFiles("ae_*.root", directory + "/GATOutputs")
#        listOfGATOutputs.sort()
#        #print 'listOfGATOutputs:', listOfGATOutputs # FIXME: debug purposes
#        print("=" * 130)
#        print(" " * 55 + "\033[1mSUMMARY OF ANALYSES\033[0m" + " " * 55)
#        print("=" * 130)
#        print("FILENAME (Truncated if longer               COUNTS           COUNTS w/ GRAN   COUNTS w/ GRAN    EFFICIENCY       EFFICIENCY PSA   ")
#        print("than 35 characters long)                    IN ROI           CUT IN ROI       PSA CUT IN ROI    GRAN CUT(%)      GIVEN GRAN CUT(%)")
#        print("+" * 130)
#        summaryOfAnalysesFile = open("SummaryOfAnalyses.txt","w")
#        summaryOfAnalysesFile.write("=" * 130 + "\n")
#        summaryOfAnalysesFile.write(" " * 55 + "SUMMARY OF ANALYSES" + " " * 55 + "\n")
#        summaryOfAnalysesFile.write("=" * 130 + "\n")
#        summaryOfAnalysesFile.write("FILENAME (Truncated if longer"+ " " * 15 + "COUNTS" + " " * 11 + "COUNTS w/ GRAN   COUNTS w/ GRAN    EFFICIENCY       EFFICIENCY PSA   \n")
#        summaryOfAnalysesFile.write("than 35 characters long)" + " " * 20 + "IN ROI           CUT IN ROI       PSA CUT IN ROI    GRAN CUT(%)      GIVEN GRAN CUT(%)\n")
#        summaryOfAnalysesFile.write("+" * 130 + "\n")
#        numberFiles = 0
#        for file in listOfGATOutputs:
#            # Open the input file, get the MJOR output tree from it
#            inputFile = TFile(file)                       
#            try:
#                tree = inputFile.Get("fTree")
#            except AttributeError:
#                print 'File %s does not contain a TTree fTree' % file
#                return
#            tree.GetEntry(0) # Hack for ROOT bug: make sure all internal variables are set up
#            # Filename manipulations
#            splitPath = file.split('/')
#            filename = splitPath[-1]
#            aeExtension = filename.find("ae_")
#            extension = filename.find(".root") 
#            basename = filename[aeExtension+3:extension] # aeExtension+3 is after 'ae_'
#            # Create canvas, set log scale y, and make entries box disappear
#            canvas = TCanvas(basename, basename, 200, 10, 700, 500)
#            canvas.SetLogy()
#            gStyle.SetOptStat(0)
#            # Draw commands
#            tree.Draw("fTotalEnergy >> black(3500, 0, 3.5)", "", "")
#            tree.SetLineColor(632) # 632 = color red
#            tree.Draw("fTotalEnergy >> red(3500, 0, 3.5)", "fNWaveforms == 1", "SAME")
#            tree.SetLineColor(600) # 600 = color blue
#            tree.Draw("fTotalEnergy >> blue(3500, 0, 3.5)", "fNWaveforms == 1 && fAnalysisEventDataVector.fIsMultiSite == 0", "SAME")
#            # Calculate number of counts in ROI
#            blackGraph = gROOT.FindObject("black")
#            binMin = blackGraph.FindBin(2)
#            binMax = blackGraph.FindBin(2.1)
#            countsNoCuts = blackGraph.Integral(binMin, binMax, "")
#            countsNoCutsROI = countsNoCuts/25
#            # Calculate number of counts in ROI after applying granularity cut
#            redGraph = gROOT.FindObject("red")
#            binMinCut = redGraph.FindBin(2)
#            binMaxCut = redGraph.FindBin(2.1)
#            countsWithCut = redGraph.Integral(binMinCut, binMaxCut, "")
#            countsWithCutROI = countsWithCut/25
#            # Calculate number of counts in ROI after applying granularity and PSA cut
#            blueGraph = gROOT.FindObject("blue")
#            binMinPSACut = blueGraph.FindBin(2)    # FIXME: Doing this for Thallium 208. Need to change
#            binMaxPSACut = blueGraph.FindBin(2.1)  # if simulating a different decay chain!
#            countsWithPSACut = blueGraph.Integral(binMinPSACut, binMaxPSACut, "")
#            countsWithPSACutROI = countsWithPSACut/25
#            # Calculate efficiency of cuts
#            efficiencyOfGranCut = countsWithCutROI/countsNoCutsROI
#	    efficiencyOfPSAGivenGranCut = countsWithPSACutROI/countsWithCutROI
#	    # Calculate errors
#            errorNoCuts = math.sqrt(countsNoCuts)/25
#            errorWithCut = math.sqrt(countsWithCut)/25
#            errorWithPSACut = math.sqrt(countsWithPSACut)/25
#	    errorOfEfficiencyOfGranCut = efficiencyOfGranCut * math.sqrt( pow((errorWithCut/countsWithCutROI), 2) + \
#                                         pow((errorNoCuts/countsNoCutsROI), 2) )
#	    errorOfEfficiencyOfPSAGivenGranCut = efficiencyOfPSAGivenGranCut * math.sqrt( pow((errorWithPSACut/countsWithPSACutROI), 2) + \
#                                                 pow((errorWithCut/countsWithCutROI), 2) )
#            # Set axis titles
#            blackGraph.SetTitle(basename)
#            blackGraph.SetXTitle("Energy [MeV]")
#            blackGraph.SetYTitle("Counts")
#            # Add legend    
#            legend = TLegend(0.35, 0.76, 0.59, 0.90)
#            legend.SetBorderSize(0)
#            legend.SetFillStyle(0)
#            legend.SetFillColor(0)
#            legend.SetTextSize(0.028)
#            blackGraphDescription = "Summed energy (%.2f +/- %.2f counts in ROI)" % (countsNoCutsROI, errorNoCuts)
#            redGraphDescription = "After Granularity (%.2f +/- %.2f counts in ROI)" % (countsWithCutROI, errorWithCut)
#            blueGraphDescription = "After Granularity+PSA (%.2f +/- %.2f counts in ROI)" % (countsWithPSACutROI, errorWithPSACut)
#            legend.AddEntry(blackGraph, blackGraphDescription, "l")
#            legend.AddEntry(redGraph, redGraphDescription, "l")
#            legend.AddEntry(blueGraph, blueGraphDescription, "l")
#            legend.Draw("SAME")
#            # Print out graph
#            canvas.Print(basename + ".png")
#            numberFiles += 1
#            # Format strings to look nice
#            countsNoCutsROIString = "%.2f" % countsNoCutsROI
#            countsWithCutROIString = "%.2f" % countsWithCutROI
#            countsWithPSACutROIString = "%.2f" % countsWithPSACutROI
#            errorNoCutsString = "%.2f" % errorNoCuts
#            errorWithCutString = "%.2f" % errorWithCut
#            errorWithPSACutString = "%.2f" % errorWithPSACut
#            efficiencyOfGranCutString = "%.2f" % (efficiencyOfGranCut * 100)
#            efficiencyOfPSAGivenGranCutString = "%.2f" % (efficiencyOfPSAGivenGranCut * 100)
#            errorOfEfficiencyOfGranCutString = "%.2f" % (errorOfEfficiencyOfGranCut * 100)
#            errorOfEfficiencyOfPSAGivenGranCutString = "%.2f" % (errorOfEfficiencyOfPSAGivenGranCut * 100)
#            # Neat trick to write nice line of data: Redirect print's output to screen,
#            # store it in a string, then write it to file 
#	    if len(filename) > 35: filename = filename[:35] # Truncate filename just for purpose of writing to screen and table
#            old_stdout = sys.stdout
#	    sys.stdout = mystdout = StringIO()
#	    print filename.ljust(43), (countsNoCutsROIString + "+/-" + errorNoCutsString).ljust(16), \
#                  (countsWithCutROIString + "+/-" + errorWithCutString).ljust(16), \
#                  (countsWithPSACutROIString + "+/-" + errorWithPSACutString).ljust(17), \
#                  (efficiencyOfGranCutString + "+/-" + errorOfEfficiencyOfGranCutString).ljust(16), \
#		  (efficiencyOfPSAGivenGranCutString + "+/-" + errorOfEfficiencyOfPSAGivenGranCutString).ljust(15)
#            sys.stdout = old_stdout
#	    data = mystdout.getvalue()
#	    data = data[:-1] # Get rid of newline character
#	    print data # Print out summary of analyses to screen 
#            # Write results to output file
#            summaryOfAnalysesFile.write(data + "\n")
#            # Make Graphs folder to put all .png's into it
#            try: os.mkdir(directory + "/Graphs")
#            except OSError: pass
#            pngFile = LocateFiles(basename + ".png", directory)
#	    try: shutil.move(pngFile[0], directory + "/Graphs")
#	    except: 
#		try:
#		    if os.path.isfile(pngFile[0]):
#		        os.remove(pngFile[0])
#		except: pass        
#        summaryOfAnalysesFile.close()
#	try: shutil.move(directory + "/SummaryOfAnalyses.txt", directory + "/Graphs")
#	except: 
#	    try:
#	        if os.path.isfile(directory + "/SummaryOfAnalyses.txt"):
#	            os.remove(directory + "/SummaryOfAnalyses.txt")
#	    except: pass        
#        print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m %d files have been analyzed successfully in this directory!" % numberFiles,
#        print "Please look in the %s/Graphs directory for your plots." % directory


# ----------------------------------------------------------------------#
#                    EXECUTETASKFUNCTIONS() FUNCTION                    #
# ----------------------------------------------------------------------#
def Function1():
    print 'Function 1 [CreateCSHFiles] running...'
def Function2():
    print 'Function 2 [SubmitJobs] running...'
def Function3():
    print 'Function 3 [CleanUpCSHFiles] running...'
def Function4():
    print 'Function 4 [CollectMaGeOutputs] running...'
def Function5():
    print 'Function 5 [RunGAT] running...'
def Function6():
    print 'Function 6 [AnalyzeGATOutputs] running...'
listOfTestFunctions = [ Function1, Function2, Function3, Function4, Function5, Function6 ]

listOfTaskFunctions = [ CreateCSHFiles, SubmitJobs, CleanUpCSHFiles, CollectMaGeOutputs, RunGAT, AnalyzeGATOutputs ]
def ExecuteTaskFunctions(functionList, sequence, parameter = ""):
    '''Execute task functions in the sequence given.'''
    for i in sequence:
        function = functionList[i]
        if parameter != "": function(parameter)
	else: function()


# ----------------------------------------------------------------------#
#                   MOVEFILESTOOUTPUTPATH() FUNCTION                    #
# ----------------------------------------------------------------------#
def CopyOrMoveFilesToPath():
    '''Copy or move all files created by script to path specified by user.'''
    if options.path != "":
        path = options.path
        if path.endswith("/"): path = path[:-1]
        if not os.path.exists(path):
	    try: os.mkdir(path)
	    except OSError: pass
	else: 
	    for file in LocateFiles("*", path):
	        try: os.remove(file)
	        except: pass
	    foldersInPath = os.listdir(path)
	    for folder in foldersInPath:
		foldersInFolder = os.listdir(path + "/" + folder)
		if len(foldersInFolder) > 0:
		    for newFolder in foldersInFolder:
			for newFile in LocateFiles("*", path + "/" + folder + "/" + newFolder):
			    try: os.remove(newFile)
			    except: pass
		for file in LocateFiles("*", path + "/" + folder):
	            try: os.remove(file)
		    except: pass
		try: os.rmdir(path + "/" + folder)
	        except: pass
        copyAction = False
        moveAction = False
        whichAction = ""
        while(True):
            print r"""
  You've specified the -p option. There are two types of options available to users for convenience:
  1. Copy option: - Copy all contents to the specified path. All contents in the macro directories given
                    on the command line will stay intact. This is the default action.
  2. Move option: - Move all contents to the specified path. All contents in the macro directories given
                    on the command line will no longer be there!
            """
            action = raw_input("Do you want to copy or move all your files? (C = copy by default; m = move) --> ")
            if action == '' or action.lower() == 'c':
                print "Copying all files to %s..." % path
                whichAction = "Copying"
                copyAction = True
                break
            elif action.lower() == 'm':
                print "Moving all files to %s..." % path
                whichAction = "Moving"
                moveAction = True
                break
            elif action.lower() == 'q':
                print "Exiting script..."
                sys.exit(1)
            else:
                print "\aERROR: Invalid response!"
                AskToTryAgain()
       
	def CopyOrMoveMaGeOutputFiles(directory):    
            # MaGe output files
            filesInMaGeOutputsFolder = LocateFiles("*", directory + "/MaGeOutputs")
            #print 'filesInMaGeOutputsFolder:', filesInMaGeOutputsFolder # FIXME: debug purposes
            if len(filesInMaGeOutputsFolder) > 0: 
                try: os.mkdir(path + "/MaGeOutputs")
                except OSError: pass
                print "%s files in %s/MaGeOutputs folder to %s/MaGeOutputs..." % (whichAction, directory, path)
                for file3 in filesInMaGeOutputsFolder:
                    try: 
                        echoCommand3 = "echo ...%s %s to %s/MaGeOutputs..." % (whichAction, file3, path)
		        os.system(echoCommand3)
		    except OSError: pass
                    if copyAction:
		        try: shutil.copy2(file3, path + "/MaGeOutputs")
		        except: pass
                    elif moveAction:
                        try: shutil.move(file3, path + "/MaGeOutputs")
                        except: 
                            try:
                                if file3.endswith("/"): file3 = file3[:-1]
                                lastForwardSlash = file3.rfind('/')
                                basePathName = file3[:lastForwardSlash]
                                brokenPath = file3.split('/')
                                filename = brokenPath[-1]
                                if os.path.isfile(path + "/MaGeOutputs/" + filename):
                                    os.remove(path + "/MaGeOutputs/" + filename)
                                shutil.move(file3, path + "/MaGeOutputs")
                            except: pass        
            if moveAction:
                try: os.rmdir(directory + "/MaGeOutputs")
                except OSError: pass
       
	def CopyOrMoveGATOutputFiles(directory):    
            # GAT output files
            filesInGATOutputsFolder = LocateFiles("*", directory + "/GATOutputs")
            #print 'filesInGATOutputsFolder:', filesInGATOutputsFolder # FIXME: debug purposes
            if len(filesInGATOutputsFolder) > 0: 
                try: os.mkdir(path + "/GATOutputs")
                except OSError: pass
                print "%s files in %s/GATOutputs folder to %s/GATOutputs..." % (whichAction, directory, path)
                for file4 in filesInGATOutputsFolder:
                    try: 
                        echoCommand4 = "echo ...%s %s to %s/GATOutputs..." % (whichAction, file4, path)
		        os.system(echoCommand4)
		    except OSError: pass
                    if copyAction:
                        try: shutil.copy2(file4, path + "/GATOutputs")
                        except: pass
                    elif moveAction:
                        try: shutil.move(file4, path + "/GATOutputs")
                        except: 
                            try:
                                if file4.endswith("/"): file4 = file4[:-1]
                                lastForwardSlash = file4.rfind('/')
                                basePathName = file4[:lastForwardSlash]
                                brokenPath = file4.split('/')
                                filename = brokenPath[-1]
                                if os.path.isfile(path + "/GATOutputs/" + filename):
                                    os.remove(path + "/GATOutputs/" + filename)
                                shutil.move(file4, path + "/GATOutputs")
                            except: pass        
            if moveAction:
                try: os.rmdir(directory + "/GATOutputs")
                except OSError: pass
       
	def CopyOrMoveErrorPlotsFiles(directory):    
            # Files in /Graphs/ErrorPlots folder
            filesInErrorPlotsFolder = LocateFiles("*_ErrorPlot*.png", directory + "/Graphs/ErrorPlots")
            #print 'filesInErrorPlotsFolder:', filesInErrorPlotsFolder # FIXME: debug purposes
            if len(filesInErrorPlotsFolder) > 0: 
		try: os.mkdir(path + "/Graphs")
		except: pass
                try: os.mkdir(path + "/Graphs/ErrorPlots")
                except OSError: pass
                print "%s files in %s/Graphs/ErrorPlots folder to %s/Graphs/ErrorPlots..." % (whichAction, directory, path)
                for file5 in filesInErrorPlotsFolder:
                    try: 
                        echoCommand5 = "echo ...%s %s to %s/Graphs/ErrorPlots..." % (whichAction, file5, path)
		        os.system(echoCommand5)
		    except OSError: pass
                    if copyAction:
                        try: shutil.copy2(file5, path + "/Graphs/ErrorPlots")
                        except: pass
                    elif moveAction:
                        try: shutil.move(file5, path + "/Graphs/ErrorPlots")
                        except: 
                            try:
                                if file5.endswith("/"): file5 = file5[:-1]
                                lastForwardSlash = file5.rfind('/')
                                basePathName = file5[:lastForwardSlash]
                                brokenPath = file5.split('/')
                                filename = brokenPath[-1]
                                if os.path.isfile(path + "/Graphs/ErrorPlots/" + filename):
                                    os.remove(path + "/Graphs/ErrorPlots/" + filename)
                                shutil.move(file5, path + "/Graphs/ErrorPlots")
                            except: pass        
            if moveAction:
                try: os.rmdir(directory + "/Graphs/ErrorPlots")
                except OSError: pass

	def CopyOrMoveSummaryOfAnalysesFile(directory):    
            # SummaryOfAnalyses.txt file
	    summaryOfAnalysesFile = LocateFiles("SummaryOfAnalyses.txt", directory + "/Graphs")
            if len(summaryOfAnalysesFile) > 0: 
                if not os.path.isfile(path + "/Graphs/SummaryOfAnalyses.txt"):
                    newSummaryOfAnalysesFile = open(path + "/Graphs/SummaryOfAnalyses.txt", "w")
                else: 
                    newSummaryOfAnalysesFile = open(path + "/Graphs/SummaryOfAnalyses.txt", "a")
		directoryLine = "DIRECTORY: %s" % directory
		newSummaryOfAnalysesFile.write(directoryLine + "\n")
                try: 
                    echoCommand1 = "echo %s file %s to %s/Graphs..." % (whichAction, summaryOfAnalysesFile[0], path)
		    os.system(echoCommand1)
		except OSError: pass
                file = open(summaryOfAnalysesFile[0], "r")
                for line in file:
                    newSummaryOfAnalysesFile.write(line)
                if moveAction:
                    if os.path.isfile(directory + "/Graphs/SummaryOfAnalyses.txt"):
                        try: os.remove(directory + "/Graphs/SummaryOfAnalyses.txt")
                        except: pass

	def CopyOrMoveGraphsFiles(directory):    
            # Files in /Graphs folder
            filesInGraphsFolder = LocateFiles("*.png", directory + "/Graphs")
            #print 'filesInGraphsFolder:', filesInGraphsFolder # FIXME: debug purposes
            if len(filesInGraphsFolder) > 0: 
                print "%s .png files in %s/Graphs folder to %s/Graphs..." % (whichAction, directory, path)
                for file6 in filesInGraphsFolder:
                    try: 
                        echoCommand6 = "echo ...%s %s to %s/Graphs..." % (whichAction, file6, path)
		        os.system(echoCommand6)
		    except OSError: pass
                    if copyAction:
                        try: shutil.copy2(file6, path + "/Graphs")
                        except: pass
                    elif moveAction:
                        try: shutil.move(file6, path + "/Graphs")
                        except: 
                            try:
                                if file6.endswith("/"): file6 = file6[:-1]
                                lastForwardSlash = file6.rfind('/')
                                basePathName = file6[:lastForwardSlash]
                                brokenPath = file6.split('/')
                                filename = brokenPath[-1]
                                if os.path.isfile(path + "/Graphs/" + filename):
                                    os.remove(path + "/Graphs/" + filename)
                                shutil.move(file6, path + "/Graphs")
                            except: pass        
		    if file6.find("SummaryOfAnalyses.txt") > -1:
		        CopyOrMoveSummaryOfAnalysesFile(directory)
            if moveAction:
                try: os.rmdir(directory + "/Graphs")
                except OSError: pass

	def CopyOrMoveJobsSummaryStatusFile(directory):    
            # JobsSummaryStatus.txt file
	    jobsSummaryStatusFile = LocateFiles("JobsSummaryStatus.txt", directory)
            #print 'jobsSummaryStatusFile:', jobsSummaryStatusFile # FIXME: debug purposes
            if len(jobsSummaryStatusFile) > 0: 
                if not os.path.isfile(path + "/JobsSummaryStatus.txt"):
                    newJobsSummaryStatusFile = open(path + "/JobsSummaryStatus.txt", "w")
                else: 
                    newJobsSummaryStatusFile = open(path + "/JobsSummaryStatus.txt", "a")
		directoryLine = "DIRECTORY: %s" % directory
		newJobsSummaryStatusFile.write(directoryLine + "\n")
                try: 
                    echoCommand1 = "echo %s file %s to %s..." % (whichAction, jobsSummaryStatusFile[0], path)
		    os.system(echoCommand1)
		except OSError: pass
                file = open(jobsSummaryStatusFile[0], "r")
                for line in file:
                    newJobsSummaryStatusFile.write(line)
                if moveAction:
                    if os.path.isfile(directory + "/JobsSummaryStatus.txt"):
                        try: os.remove(directory + "/JobsSummaryStatus.txt")
                        except: pass

	def CopyOrMoveTSBasicMacroFile(directory):    
            # TSBasicMacro.mac file
	    TSBasicMacroFile = LocateFiles("TSBasicMacro.mac", directory)
            #print 'TSBasicMacroFile:', TSBasicMacroFile # FIXME: debug purposes
            if len(TSBasicMacroFile) > 0: 
                if not os.path.isfile(path + "/TSBasicMacro.mac"):
                    newTSBasicMacroFile = open(path + "/TSBasicMacro.mac", "w")
                else: 
                    newTSBasicMacroFile = open(path + "/TSBasicMacro.mac", "a")
		directoryLine = "DIRECTORY: %s" % directory
		newTSBasicMacroFile.write(directoryLine + "\n")
                try: 
                    echoCommand1 = "echo %s file %s to %s..." % (whichAction, TSBasicMacroFile[0], path)
		    os.system(echoCommand1)
		except OSError: pass
                file = open(TSBasicMacroFile[0], "r")
                for line in file:
                    newTSBasicMacroFile.write(line)
                if moveAction:
                    if os.path.isfile(directory + "/TSBasicMacro.mac"):
                        try: os.remove(directory + "/TSBasicMacro.mac")
                        except: pass

	def CopyOrMoveTSGeometryViewerFile(directory):    
            # TSGeometryViewer.mac file
	    TSGeometryViewerFile = LocateFiles("TSGeometryViewer.mac", directory)
            #print 'TSGeometryViewerFile:', TSGeometryViewerFile # FIXME: debug purposes
            if len(TSGeometryViewerFile) > 0: 
                if not os.path.isfile(path + "/TSGeometryViewer.mac"):
                    newTSGeometryViewerFile = open(path + "/TSGeometryViewer.mac", "w")
                else: 
                    newTSGeometryViewerFile = open(path + "/TSGeometryViewer.mac", "a")
		directoryLine = "DIRECTORY: %s" % directory
		newTSGeometryViewerFile.write(directoryLine + "\n")
                try: 
                    echoCommand1 = "echo %s file %s to %s..." % (whichAction, TSGeometryViewerFile[0], path)
		    os.system(echoCommand1)
		except OSError: pass
                file = open(TSGeometryViewerFile[0], "r")
                for line in file:
                    newTSGeometryViewerFile.write(line)
                if moveAction:
                    if os.path.isfile(directory + "/TSGeometryViewer.mac"):
                        try: os.remove(directory + "/TSGeometryViewer.mac")
                        except: pass

	def CopyOrMoveBigMacrosFiles(directory):    
            # Big macro files
            filesInBigMacrosFolder = LocateFiles("*", directory + "/BigMacros")
            #print 'filesInBigMacrosFolder:', filesInBigMacrosFolder # FIXME: debug purposes
            if len(filesInBigMacrosFolder) > 0: 
                try: os.mkdir(path + "/BigMacros")
                except OSError: pass
                print "%s files in %s/BigMacros folder to %s/BigMacros..." % (whichAction, directory, path)
                for file1 in filesInBigMacrosFolder:
                    try: 
                        echoCommand1 = "echo ...%s %s to %s/BigMacros..." % (whichAction, file1, path)
		        os.system(echoCommand1)
		    except OSError: pass
		    if copyAction:
		        try: shutil.copy2(file1, path + "/BigMacros")
		        except: pass
		    elif moveAction:
                        try: shutil.move(file1, path + "/BigMacros")
                        except: 
                            try:
                                if file1.endswith("/"): file1 = file1[:-1]
                                lastForwardSlash = file1.rfind('/')
                                basePathName = file1[:lastForwardSlash]
                                brokenPath = file1.split('/')
                                filename = brokenPath[-1]
                                if os.path.isfile(path + "/BigMacros/" + filename):
                                    os.remove(path + "/BigMacros/" + filename)
                                shutil.move(file1, path + "/BigMacros")
                            except: pass        
            if moveAction:
                try: os.rmdir(directory + "/BigMacros")
                except OSError: pass

	def CopyOrMoveMacrosFiles(directory):    
            # Macro files
            filesInMacrosFolder = LocateFiles("*", directory + "/Macros")
            #print 'filesInMacrosFolder:', filesInMacrosFolder # FIXME: debug purposes
            if len(filesInMacrosFolder) > 0: 
                try: os.mkdir(path + "/Macros")
                except OSError: pass
                print "%s files in %s/Macros folder to %s/Macros..." % (whichAction, directory, path)
                for file2 in filesInMacrosFolder:
                    try: 
                        echoCommand2 = "echo ...%s %s to %s/Macros..." % (whichAction, file2, path)
		        os.system(echoCommand2)
		    except OSError: pass
                    if copyAction:
		        try: shutil.copy2(file2, path + "/Macros")
		        except: pass
                    elif moveAction:
                        try: shutil.move(file2, path + "/Macros")
                        except: 
                            try:
                                if file2.endswith("/"): file2 = file2[:-1]
                                lastForwardSlash = file2.rfind('/')
                                basePathName = file2[:lastForwardSlash]
                                brokenPath = file2.split('/')
                                filename = brokenPath[-1]
                                if os.path.isfile(path + "/Macros/" + filename):
                                    os.remove(path + "/Macros/" + filename)
                                shutil.move(file2, path + "/Macros")
                            except: pass        
            if moveAction:
                try: os.rmdir(directory + "/Macros")
                except OSError: pass

	def CopyOrMoveStoredDirectoriesLog(directory):    
            # .stored_directories.log file
	    storedDirectoriesLogFile = LocateFiles(".stored_directories.log", directory)
            #print 'storedDirectoriesLogFile:', storedDirectoriesLogFile # FIXME: debug purposes
            if len(storedDirectoriesLogFile) > 0: 
                if not os.path.isfile(path + "/.stored_directories.log"):
                    newStoredDirectoriesLogFile = open(path + "/.stored_directories.log", "w")
                else: 
                    newStoredDirectoriesLogFile = open(path + "/.stored_directories.log", "a")
		directoryLine = "DIRECTORY: %s" % directory
		newStoredDirectoriesLogFile.write(directoryLine + "\n")
                try: 
                    echoCommand3 = "echo %s file %s to %s..." % (whichAction, storedDirectoriesLogFile[0], path)
		    os.system(echoCommand3)
		except OSError: pass
                file = open(storedDirectoriesLogFile[0], "r")
                for line in file:
                    newStoredDirectoriesLogFile.write(line)
                if moveAction:
                    if os.path.isfile(directory + "/.stored_directories.log"):
                        try: os.remove(directory + "/.stored_directories.log")
                        except: pass

        listOfCopyOrMoveFunctions = [ CopyOrMoveMaGeOutputFiles, CopyOrMoveGATOutputFiles, CopyOrMoveErrorPlotsFiles, \
	                              CopyOrMoveSummaryOfAnalysesFile, CopyOrMoveGraphsFiles, CopyOrMoveJobsSummaryStatusFile, \
				      CopyOrMoveTSBasicMacroFile, CopyOrMoveTSGeometryViewerFile, CopyOrMoveBigMacrosFiles, \
				      CopyOrMoveMacrosFiles, CopyOrMoveStoredDirectoriesLog ]
        global listOfMacroDirectories
        for directory in listOfMacroDirectories:
            print "Entering directory: %s ..." % directory
            if(options.collectMaGeOutputs):
	        ExecuteTaskFunctions(listOfCopyOrMoveFunctions, range(0, 1), directory)
	    elif(options.collectMaGeOutputsEnd):
	        ExecuteTaskFunctions(listOfCopyOrMoveFunctions, range(0, 11), directory)
	    elif(options.runGAT):
	        ExecuteTaskFunctions(listOfCopyOrMoveFunctions, range(1, 2), directory)
	    elif(options.runGATEnd):
	        ExecuteTaskFunctions(listOfCopyOrMoveFunctions, range(1, 11), directory)
	    elif(options.analyzeGATOutputs):
	        ExecuteTaskFunctions(listOfCopyOrMoveFunctions, range(2, 5), directory)
	    if moveAction:
	        try: os.rmdir(directory)
	        except: pass
        print "\033[4m\033[1mSTATUS SUMMARY:\033[0m\033[0m Done moving all script outputs to %s." % path


# ******************************************************************************************************** #
# *                                 All script operations happen here                                    * #
# ******************************************************************************************************** #
# ----------------------------------------------------------------------#
#                           INTERACTIVE MODE                            #
# ----------------------------------------------------------------------#
listOfMacroFiles = []
listOfMacroDirectories = [] 
if(options.interactiveMode and len(args) == 0):
    print logo + scriptDescription
    while(True):
        print "Please specify a location where your macros can be found (ex. ~/Work/Majorana/TSSimulations/Macros)."
        macroLocation = raw_input("Default is current directory --> ")
	if macroLocation == '':
	    print "Looking for macros in current directory..."
	    macroLocation = "./"
	    break
	elif macroLocation.lower() == 'q':
	    print "Exiting script..."
	    sys.exit(1)
	elif not os.path.exists(macroLocation):
	    print '\aThe path %s does not exist!' % macroLocation,
	    AskToTryAgain()
        else:
	    break
    # If user specifies path name with '/' at the end, remove it. This is done to
    # make sure commands to create folders in this script work properly.
    if macroLocation.endswith("/"): macroLocation = macroLocation[:-1]
    if not listOfMacroDirectories:
        listOfMacroDirectories.append(macroLocation)
    if macroLocation not in listOfMacroDirectories:
        listOfMacroDirectories.append(macroLocation)
    while(True):        
        giveMore = raw_input("Would you like to add another directory? (y/N) --> ")
        if giveMore == '' or giveMore.lower() == 'n':
            print "Not adding any more macro directories to analyze..."
            break
        elif giveMore.lower() == 'y':
	    while(True):
                newDirectory = raw_input("New directory --> ")
                if newDirectory == '':
                    print "\aERROR: You must type in a directory name."
                    AskToTryAgain()
                elif newDirectory.lower() == 'q':
                    print "Exiting script..."
                    sys.exit(1)
                elif not os.path.exists(newDirectory):
                    print '\aThe path %s does not exist!' % newDirectory,
                    AskToTryAgain()
                else:
                    # If user specifies path name with '/' at the end, remove it. This is done to
                    # make sure commands to create folders in this script work properly.
                    if newDirectory.endswith("/"): newDirectory = newDirectory[:-1]
		    if newDirectory not in listOfMacroDirectories:
                        listOfMacroDirectories.append(newDirectory)
                    break
            continue
        elif giveMore.lower() == 'q':
            print "Exiting script..."
            sys.exit(1)
    # Allow user to add, delete, or change list of macro directories
    while(True):
        ShowListOfMacroDirectories()
        print "This is your last chance to (+) add, (-) delete, or (d) be done with this list..."
        changeList = raw_input("[+ = add; - = delete, d = done, q = quit] --> ")
        if changeList == '':
	    print "\aERROR: Invalid response! You must select from the options given."
	    AskToTryAgain()
	elif changeList == '+':
	    while(True):
                newDirectory = raw_input("New directory --> ")
                if newDirectory == '':
                    print "\aERROR: You must type in a directory name."
                    AskToTryAgain()
                elif newDirectory.lower() == 'q':
                    print "Exiting script..."
                    sys.exit(1)
                elif not os.path.exists(newDirectory):
                    print '\aThe path %s does not exist!' % newDirectory,
                    AskToTryAgain()
                else:
                    if newDirectory.endswith("/"): newDirectory = newDirectory[:-1]
		    if newDirectory not in listOfMacroDirectories:
			print "Adding %s to list of macro directories..." % newDirectory
                        listOfMacroDirectories.append(newDirectory)
                    break
	elif changeList == '-':
	    while(True):
                ShowListOfMacroDirectories()
		directoryToDelete = raw_input('Enter a number from the list above to delete a directory --> ')
		if directoryToDelete == '':
                    print "\aERROR: You must type in a number corresponding to a directory from the list."
                    AskToTryAgain()
		elif directoryToDelete.lower() == 'q':
		    print 'Exiting script...'
		    sys.exit(1)
		try: 
                    if int(directoryToDelete) <= 0:
			print '\aERROR: Invalid selection!',
			AskToTryAgain()
		    elif int(directoryToDelete) >= 1:
		        foundChoice = False
		        for i, (number, choice) in enumerate(availableSelections):
			    if int(directoryToDelete) == number:
				foundChoice = True
				directoryToDelete = choice
				print "Removing %s from list of macro directories..." % directoryToDelete
				listOfMacroDirectories.remove(directoryToDelete)
				break
		        if foundChoice == False:
			    print '\aERROR: Invalid selection!',
			    AskToTryAgain()
		        else: break
		except ValueError: # If user doesn't enter in a number but perhaps a string instead
		    if directoryToDelete not in listOfMacroDirectories:
		        print "Directory %s is not in list!" % directoryToDelete
			AskToTryAgain()
		    elif not os.path.exists(directoryToDelete):
			print "\aERROR: The directory %s does not exist!" % directoryToDelete
			AskToTryAgain()
                    else:
			listOfMacroDirectories.remove(directoryToDelete)
			break
	elif changeList.lower() == 'd':
	    print "Done with list..."
	    break
	elif changeList.lower() == 'q':
	    print "Exiting script..."
	    sys.exit(1)
        else:
	    print "\aERROR: Invalid selection!",
	    AskToTryAgain()
    #print 'listOfMacroDirectories:', listOfMacroDirectories # FIXME: debug purposes
    if len(listOfMacroDirectories) > 0: 
        # Create a log file to store all directories supplied by user in most current run.
        # This file will be hidden, hence the "." in front of the name.
        directoriesLog = open(".stored_directories.log","w")
        now = time.localtime(time.time())
        nowString = str(time.asctime(now))
        directoriesLog.write(nowString)
        directoriesLog.write("\n")
        print "The following macro files were found in each correctly specified directory..."
        print("=" * 80)
	macroFilesFound = []
        for directory in listOfMacroDirectories:
	    directoriesLog.write(directory)
            directoriesLog.write("\n")
	    del macroFilesFound[:]
            macroFilesFound = LocateFiles("*.mac", directory)
	    for macroFile in macroFilesFound:
	        CheckMacroFileForManagability(macroFile)
            macroFilesFound = LocateFiles("*.mac", directory)
            macroFilesFound.sort()
            number = 0
            print "\033[1mDIRECTORY:\033[0m %s" % directory
            if len(macroFilesFound) == 0:
               print "None\n"
               continue
            for file in macroFilesFound:
                listOfMacroFiles.append(file)
                number += 1
                output = str(number) + '. ' + file
                print output
            print
	directoriesLog.write("\n")
        directoriesLog.close()
        # Ask user to confirm all macro files before continuing script
        while(True):
            print "Please confirm that these directories contain all the ton scale simulation files you want to analyze."
            continueScript = raw_input("Continue? (Y/n) --> ")
            if continueScript == '' or continueScript.lower() == 'y':
                print "Continuing with analysis..."
                break
            elif continueScript.lower() == 'q' or continueScript.lower() == 'n':
                print "Exiting script..."
                sys.exit(1)
            else:
                print "\aERROR: Invalid response!"
                AskToTryAgain()
    #ExecuteTaskFunctions(listOfTestFunctions, range(0, 3)) # FIXME: debug purposes
    ExecuteTaskFunctions(listOfTaskFunctions, range(0, 3))
    print "After simulation jobs are completed, run the following command to continue where script left off:"
    print "python manageTonScaleSimulationTasks.py --collectEnd @"
    sys.exit(0)
elif(options.interactiveMode == True and len(args) > 0):
    print '\aERROR: Interactive Mode does not take any positional arguments.'
    print 'Do the following command: python manageTonScaleSimulationTasks.py -i'
    sys.exit(1)
elif(options.macro == True and len(args) > 0):
    print '\aERROR: The -m option does not take any positional arguments.'
    print 'Do the following command: python manageTonScaleSimulationTasks.py -m'
    sys.exit(1)
else:
    print logo
    # "-m" or "--macro" option does not require any positional arguments
    if(options.macro):
        CreateBasicMacro()
	sys.exit(0)
    # "-g" or "--geometry" option does not require any positional arguments
    if(options.geometryViewer):
        CreateGeometryViewerMacro()
	sys.exit(0)
    # Bulk mode requires special and careful implementation
    if(options.bulkMode):
        if len(args) > 1:
	    print "\aERROR: Bulk mode is too risky! Please specify only \033[1mONE\033[0m base directory to look for macros in subdirectories."
	    sys.exit(1)
    if options.path != "":
        if(not options.collectMaGeOutputs and not options.runGAT and not options.analyzeGATOutputs \
           and not options.collectMaGeOutputsEnd and not options.runGATEnd):
            print "\aERROR: The -p option can only be used with the following options:"
            print "       1. --collect           4. --collectEnd"
            print "       2. --run               5. --runEnd"           
            print "       3. --analyze"
            sys.exit(1)
    for k in range(0, len(args)):
        path = str(args[k])
	# If path is '@', use list of macro directories stored in .stored_directories.log file
        if len(args) == 1 and path == "@": 
            listOfMacroDirectories = BuildListOfMacroDirectoriesFromLog()
            break
	# If user specifies path name with '/' at the end, remove it. This is done to
	# make sure commands to create folders in this script work properly.
        if path.endswith("/"): path = path[:-1]
        if not os.path.exists(path):
            print "\aThe path %s does not exist!\n" % path,
            continue
        if not listOfMacroDirectories:
            listOfMacroDirectories.append(path)
        if path not in listOfMacroDirectories:
            listOfMacroDirectories.append(path)

# List all available macro files in each existing directory
if len(listOfMacroDirectories) > 0: 
    # Create a log file to store all directories supplied by user in most current run.
    # This file will be hidden, hence the "." in front of the name.
    directoriesLog = open(".stored_directories.log","w")
    now = time.localtime(time.time())
    nowString = str(time.asctime(now))
    directoriesLog.write(nowString)
    directoriesLog.write("\n")
    if(not options.collectMaGeOutputs and not options.runGAT and not options.analyzeGATOutputs \
       and not options.collectMaGeOutputsEnd and not options.runGATEnd):
        print "The following macro files were found in each correctly specified directory..."
        print("=" * 80)
    macroFilesFound = []
    for directory in listOfMacroDirectories:
	directoriesLog.write(directory)
        directoriesLog.write("\n")
	del macroFilesFound[:]
	# If bulk mode is activated, search for macro files in subdirectories as well
	if(options.bulkMode):
	    macroFilesFound = LocateFiles("*.mac", directory, lookInSubdirectories = True)
	    for macroFile in macroFilesFound:
                if(options.collectMaGeOutputs or options.runGAT or options.analyzeGATOutputs \
                   or options.collectMaGeOutputsEnd or options.runGATEnd):
		    CheckMacroFileForManagability(macroFile, False)
	        else: CheckMacroFileForManagability(macroFile)
	    macroFilesFound = LocateFiles("*.mac", directory, lookInSubdirectories = True)
        else:
            macroFilesFound = LocateFiles("*.mac", directory)
	    for macroFile in macroFilesFound:
                if(options.collectMaGeOutputs or options.runGAT or options.analyzeGATOutputs \
                   or options.collectMaGeOutputsEnd or options.runGATEnd):
		    CheckMacroFileForManagability(macroFile, False)
	        else: CheckMacroFileForManagability(macroFile)
            macroFilesFound = LocateFiles("*.mac", directory)
        macroFilesFound.sort()
        number = 0
        if(not options.collectMaGeOutputs and not options.runGAT and not options.analyzeGATOutputs \
           and not options.collectMaGeOutputsEnd and not options.runGATEnd):
            print "\033[1mDIRECTORY:\033[0m %s" % directory
            if len(macroFilesFound) == 0:
               print "None\n"
               continue
            for file in macroFilesFound:
                listOfMacroFiles.append(file)
                number += 1
                output = str(number) + '. ' + file
                print output
            print
    directoriesLog.close()
    # Ask user to confirm all macro files before continuing script
    if(not options.collectMaGeOutputs and not options.runGAT and not options.analyzeGATOutputs \
       and not options.collectMaGeOutputsEnd and not options.runGATEnd):
        while(True):
            if options.cleanUpCSHFiles or options.fresh:
                print "Please confirm that these directories contain all the ton scale simulation files you want to remove."
            else: 
                print "Please confirm that these directories contain all the ton scale simulation files you want to analyze."
            continueScript = raw_input("Continue? (Y/n) --> ")
            if continueScript == '' or continueScript.lower() == 'y':
                if options.cleanUpCSHFiles or options.fresh:
                    print "Continuing with removal of all files..."
                else:
                    print "Continuing with analysis..."
                break
            elif continueScript.lower() == 'q' or continueScript.lower() == 'n':
                print "Exiting script..."
                sys.exit(1)
            else:
                print "\aERROR: Invalid response!"
                AskToTryAgain()

#print 'listOfMacroDirectories:', listOfMacroDirectories      # FIXME: debug purposes
#print 'listOfMacroFiles:', listOfMacroFiles                  # FIXME: debug purposes
#print 'length of listOfMacroFiles:', len(listOfMacroFiles)   # FIXME: debug purposes                       

# Implementation of specific task options
if(options.createCSHFiles):
    #Function1()
    CreateCSHFiles()
    sys.exit(0)
elif(options.submitJobs):
    #Function2()
    SubmitJobs()
    sys.exit(0)
elif(options.cleanUpCSHFiles):
    #Function3()
    CleanUpCSHFiles()
    sys.exit(0)
elif(options.collectMaGeOutputs):
    #Function4()
    CollectMaGeOutputs()
    CopyOrMoveFilesToPath()
    sys.exit(0)
elif(options.runGAT):
    # Ask user to confirm all MaGe output files before continuing script
    while(True):
	ShowListOfFilesFound("MaGe")
        print "Please confirm that these are all the MaGe output files you want to analyze."
        continueScript = raw_input("Continue? (Y/n) --> ")
        if continueScript == '' or continueScript.lower() == 'y': break
        elif continueScript.lower() == 'q' or continueScript.lower() == 'n':
            print "Exiting script..."
            sys.exit(1)
        else:
            print "\aERROR: Invalid response!"
            AskToTryAgain()
    #Function5()
    RunGAT()
    CopyOrMoveFilesToPath()
    sys.exit(0)
elif(options.analyzeGATOutputs):
    # Ask user to confirm all GAT output files before continuing script
    while(True):
	ShowListOfFilesFound("GAT")
        print "Please confirm that these are all the GAT output files you want to analyze."
        continueScript = raw_input("Continue? (Y/n) --> ")
        if continueScript == '' or continueScript.lower() == 'y': break
        elif continueScript.lower() == 'q' or continueScript.lower() == 'n':
            print "Exiting script..."
            sys.exit(1)
        else:
            print "\aERROR: Invalid response!"
            AskToTryAgain()
    #Function6()
    AnalyzeGATOutputs()
    CopyOrMoveFilesToPath()
    sys.exit(0)
# Implementation of progressive task options
elif(options.submitJobsEnd):
    print "Starting script with submission of jobs..."
    #ExecuteTaskFunctions(listOfTestFunctions, range(1, 3))
    ExecuteTaskFunctions(listOfTaskFunctions, range(1, 3))
    print "After simulation jobs are completed, run the following command to continue where script left off:"
    print "python manageTonScaleSimulationTasks.py --collectEnd @"
    sys.exit(0)
elif(options.cleanUpCSHFilesEnd):
    print "Starting script with cleanup of .csh files for completed jobs..."
    #ExecuteTaskFunctions(listOfTestFunctions, range(2, 3))
    ExecuteTaskFunctions(listOfTaskFunctions, range(2, 3))
    print "After simulation jobs are completed, run the following command to continue where script left off:"
    print "python manageTonScaleSimulationTasks.py --collectEnd @"
    sys.exit(0)
elif(options.collectMaGeOutputsEnd):
    print "Starting script with collection of all MaGe .root files and continuing to the end..."
    #ExecuteTaskFunctions(listOfTestFunctions, range(3, 6))
    ExecuteTaskFunctions(listOfTaskFunctions, range(3, 6))
    CopyOrMoveFilesToPath()
    sys.exit(0)
elif(options.runGATEnd):
    print "Starting script with running of GAT on all MaGe outputs in /MaGeOutputs folder and continuing to the end..."
    # Ask user to confirm all MaGe output files before continuing script
    while(True):
	ShowListOfFilesFound("MaGe")
        print "Please confirm that these are all the MaGe output files you want to analyze."
        continueScript = raw_input("Continue? (Y/n) --> ")
        if continueScript == '' or continueScript.lower() == 'y': break
        elif continueScript.lower() == 'q' or continueScript.lower() == 'n':
            print "Exiting script..."
            sys.exit(1)
        else:
            print "\aERROR: Invalid response!"
            AskToTryAgain()
    #ExecuteTaskFunctions(listOfTestFunctions, range(4, 6))
    ExecuteTaskFunctions(listOfTaskFunctions, range(4, 6))
    CopyOrMoveFilesToPath()
    sys.exit(0)
elif(options.fresh):
    FreshRun()
    sys.exit(0)
else:
    #ExecuteTaskFunctions(listOfTestFunctions, range(0, 3))
    ExecuteTaskFunctions(listOfTaskFunctions, range(0, 3))
    print "After simulation jobs are completed, run the following command to continue where script left off:"
    print "python manageTonScaleSimulationTasks.py --collectEnd @"
    sys.exit(0)
