void wf(const char* filename = "WFGenGaussResponseOutput.root", int n=-1)
{
  // to use this script from the command line, go to the MaGe base
  // directory and do 
  // root waveform/wf.C
  // (assuming you have a file WFGenGaussResponseOutput.root in the MaGe
  // base directory). If instead you have a file "myfile.root" with
  // hundreds of entries, you might try
  // root 'waveform/wf.C("myfile.root", 5)'
  // to print the first 5 events in your file.

  gROOT->ProcessLine(".L waveform/libWaveformROOT.so");
  TFile* file = TFile::Open(filename);
  TTree* tree = (TTree*) file->Get("wfTree");
  if(n == -1) n = tree->GetEntries();
  for(int i=0; i<n; i++) {
    for(int j=0; j<MGWaveformCollectionTreeData::GetNCrystalsHit(tree, i); j++) {
      MGWaveformCollectionTreeData::DrawWFCollection(tree, i, j);
      c1->Update();
      c1->Print(Form("c1_%d_%d.png",i,j));
    }
  }
}
