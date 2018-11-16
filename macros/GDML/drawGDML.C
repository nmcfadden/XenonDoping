// $Id: drawGDML.C,v 1.1 2007-09-28 10:08:00 jliu Exp $
// $Author: jliu $
// $Date: 2007-09-28 10:08:00 $
 
// a simple ROOT macro to draw the GDML geometry in ROOT
 
// usage:
// -----
// $ root
// root [] .x drawGDML.C("Corrado.gdml")


void drawGDML(char *filename="test.gdml") 
{
   gSystem->Load("libGeom");
   
   gSystem->Load("libGdml");
   
   TGeoManager::Import(filename);
   
   gGeoManager->GetTopVolume()->Draw();
}

