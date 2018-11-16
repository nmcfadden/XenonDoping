# GERDA macro files
Contents:
* `historic/`: old macros
* `visualization/`: files for visualization purposes, see next section
## Geometry visualization
In general, Geant4 provides a variety of Visualization Drivers to meet the needs of the user. Two of them are explained here, DAWN and HepRep (HepRepFile and HepRepXML). For others such as OpenGL, OpenInventor, VRML, RayTracer and ASCIITree, see the G4 User's Guide at:
<http://geant4.cern.ch/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/Visualization/visdrivers.html>

HepRep creates a wireframe or simple area fill drawing, it does not provide photorealistic pictures. HepRep has many interactive features like zooming, translating and rotating the drawing. Attributes can be shown by clicking on objects. HepRep provides a hierarchical view of the geometry  which can be controlled from a tree-like structure (World -> MotherVolume -> etc.) Files created by HepRep can be exported to many vector graphic formats like (Encapsulated) PostScript, PDF etc. However, HepRep requires the WIRED Browser which you have to install additionally to geant4.

Two visualization modes can be used in HepRep, HepRepFile or HepRepXML:

HepRepFile creates a HepRepXML file in the so-called "HepRep1" format. This format is suitable for viewing with the WIRED3 HepRepBrowser. This Browser has to be installed additionally to geant4. The generated files have the extension ".heprep" (default) or, as a binary, ".bheprep". The command to open a filename.heprep-file with wired3 is:
```
wired filename.heprep
```
HepRepXML creates a HepRepXML file in the so-called "HepRep2" format. This format is suitable for viewing with the WIRED4 plugin to the JAS3 Analysis System. This jas3 also has to be installed seperately. The generated files have the extension ".heprep.zip" (default), ".heprep.gz",and as a binary ".bheprep.zip"or ".bheprep.gz". To open a filename.heprep.zip-file with jas3 (wired4), the command is:
```
jas3 filename.heprep.zip
```
jas3 opens a window for drawing, you should choose "File -> New.." on the toolbar, and then "Wired4 View". Then your drawing will appear, you can zoom, rotate, translate.. by clicking on the picture. You can export different file formats (eps, pdf...) directly from the window.

DAWN is a vectorized 3D Postscript processor and creates technically high quality photorealistic outputs. It is therefore suitable for preparing presentations etc. However, there are no interactive features in DAWN. DAWNFILE creates a ".prim"-file which transforms into an encapsulated Postscript ".eps"-file by invoking "dawn filename.prim". This enables the user to easily include the drawing in LaTeX.

All Visualization Drivers are coded in a rather similar way: First, a viewer is created:
```
/vis/open [Visualization Driver]
```
You'll get a list of the available visualization drivers when you run `MaGe`, e.g. DAWNFILE, HepRepFile, HepRepXML etc.

In the next step, an empty scene is created:
```
 /vis/scene/create [filename.*]
```
`[filename]` is the name of the scene. It can have various extensions depending on the Visualization Driver. For HepRepFile: filename.heprep/filename.bheprep, for HepRepXML: filename.heprep.zip/filename.heprep.gz/filename.bheprep.zip/filename.bheprep.gz. If you do not specify the filename, geant4 will do this for you. DAWN creates its own filenames, so the command goes without `[filename.*]`.

Filling the scene consisting of a physical volume and making the current viewer draw it requires:
```
/vis/drawVolume [physical volume]
```
If no physical volume is specified, the default value is "world". The appearance of the drawing can be specified by the following commands:
```
/vis/viewer/set/viewpointThetaPhi 80 -90 deg     # sets viewing angle
/vis/viewer/zoom                  2.5            # sets magnification of drawinge
/vis/viewer/set/viewpointVector   1 0.25 0.25    # sets viewpoint coordinates
```
For more options see User's Guide.
The end of a visualization is declared by:
```
/vis/viewer/flush
```
**NOTE:** with some drivers you have to issue also `/vis/sceneHandler/attach` after adding your volumes.

In the `visualization/` folder there is a collection of volume lists for visualization purposes, to use them start an interactive MaGe session with
```
MaGe
```
Then load the geometry with
```
/control/execute load-phaseII.mac
```
Now you can open and set your favourite visualization driver with `/vis/open` etc. Add the volumes you want to plot with
```
/control/execute [file with volumes]
```
where `[file with volumes]` is one of the files you can find under `visualization/` that refer to a macro-volume (e.g. mini-shrouds, LAr instrumentation, holder mounting etc). You can adjust them to include only the volumes you need, up to now every available GERDA volume is added to the visualizing scene.

At this point all you have to do is
```
/vis/viewer/flush
```
### TL;DR
```
$ MaGe
Idle> /control/execute load-phaseII.mac
Idle> /vis/open [VIEVER]
Idle> /vis/scene/create [OPTIONS]
Idle> /control/execute [VOLUMES FILE1]
Idle> /control/execute [VOLUMES FILE2]
Idle> /vis/sceneHandler/attach
...
Idle> /vis/viewer/refresh
```
You can find a complete example macro that can be directly run via `/control/execute` under `visualization/run-vis.mac`.
