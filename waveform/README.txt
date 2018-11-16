The essential classes in this package are:

MGWaveform: encapsulates a waveform

MGVWaveformGenerator: base class for waveform generation algorithms.
Just define the pure virtual functions and your algorithm is fully
implemented.

MGWaveformManager: does all the management, i.e. preparing the MGWaveforms
to hold the results, pulling (x,y,z,t,E) info from the step data,
sending them to an MGVWaveformGenerator to be converted into waveforms,
and storing the results in a ROOT file.

This package still lacks good documentation. In the mean time, users
should work from examples. An example MGVWaveformGenerator has been
provided, MGWFGenGaussResponse, which is located in the waveform package
directories. 

An application has been written using mjprocessors to run over a G4Steps
output file and produce waveforms. To compile this application, first
make MaGe as normal.  Then remove mjprocessors and waveform from the
list of packages to be skipped in the MaGe base GNUMakefile, and edit
mjprocessor/GNUMakefile so that MJAnalysisWF.cc is the bincc file to be
compiled.  Then do 'make waveform.ROOTlib', and then 'make waveform.lib'.
Then make mjprocessor.lib, and finally make mjprocessor.bin. Run as
follows:

bin/mjprocessorApp [file]

Once the root file is produced, you can load the
waveform/libWaveformROOT.so library and call the static function
MGWaveformCollectionTreeData::DrawWFCollection(tree, iEntry, iCrystal)
to draw the waveforms for the i'th crystal of of the i'th entry in the
tree. It is possible to draw waveforms without loading the library,
but it is a bit difficult. See below for a more complete drawing routine.

To use your own class instead of the example MGWFGenGaussResponse in
the processor, simply replace the instantiation and preparation of this
class in MJAnalysisWF.cc with that of your own class and recompile.


Below is a ROOT macro to draw waveform events, one after the next. You
must load the waveform library and a file with a wfTree in it before
calling the function.

void DrawWaveforms(TTree* wfTree)
{
  int nEntries = wfTree->GetEntries();
  for(int iEntry = 0; iEntry < nEntries; iEntry++) {
    int nXtalHits = MGWaveformCollectionTreeData::GetNCrystalHits(wfTree, iEntry);
    for(int iXtal = 0; iXtal < nXtalHits; iXtal++) {
      cout << "Drawing waveforms for crystal " << wfTree->GetLeaf("fXtalID")->GetValue(iCrystal) << endl;
      MGWaveformCollectionTreeData::DrawWFCollection(wfTree, iEntry, iXtal);
      getchar();
    }
  }
}
