//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MJDemoColdPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part Origin: COM
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 *
 * 03-26-2014, Fixed touching surfaces with "shallowflatcut" and "thruHole1" J. MacMullin 
 * 01-09-2013, Updated to match current prototype design, according to Solid Work Coldplate 77mm SLDPRT. Note: The center of Coldplate w_Connectors SubAssy.SLDAM is not (0,0) 
 * W. Xu, wxu@lanl.gov
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 * 07-22-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4GeometryTolerance.hh"



#include "G4Polyhedra.hh"//wenqin added hex holes  
//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoColdPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoColdPlate::MJDemoColdPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "Cold Plate", "Copper-EF")
{;}

MJDemoColdPlate::MJDemoColdPlate(const MJDemoColdPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoColdPlate::~MJDemoColdPlate()
{;}

G4LogicalVolume* MJDemoColdPlate::ConstructPart()
{
//25.4mm is one inch
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);


  //wenqin: G4Tubs( const G4String& pName,G4double pRMin,G4double pRMax,G4double pDz,G4double pSPhi,G4double pDPhi );

  if (pVol == NULL){
	G4double mjdSurfTol = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() * 10.;
	//Wenqin:make it mjdSurfTol smaller to avoid overlap 
	
	//G4Tubs* body = new G4Tubs("body", 0, 6.099*25.4*mm, 0.2*25.4*mm, 0, 2*pi);//Original
	G4Tubs* body = new G4Tubs("body", 0, (6.25*25.4*mm-mjdSurfTol), 0.2*25.4*mm, 0, 2*pi); 
	//Wenqin: dimension checked
	//Wenqin: this is the whole plate, 2*0.2*25.4mm=10.16mm, or 0.4" thick
	//Wenqin: the plate in the prototype cryostat is 317.5 mm in diameter, or 6.25 inch in radius

        G4Box* bodycut = new G4Box("bodycut",5*25.4*mm,5*25.4*mm,5*25.4*mm);     
	//Wenqin: the edge of the disk is a flat cut, distance to center = 6.15"	    
 
	G4SubtractionSolid* body1 = new G4SubtractionSolid("body1", body, bodycut, 0, G4ThreeVector((5+6.15)*25.4*mm,0,0));
	//Wenqin: dimension checked

	//Wenqin: NOW below are the big thru holes for the string
	G4Tubs* stringThruHole = new G4Tubs("stringThruHole", 0, 0.425*25.4*mm, 0.25*25.4*mm, 0, 2*pi);
	//Wenqin: dimension checked
	//wenqin: the diameter of the big thru holes for strings is D=0.85 inch, or R=0.425inch 
	//Wenqin: 0.25>0.2, thru hole, for the strings
	//Wenqin: D=0.85" or 0.425 inch radius in drawing
	
	G4Tubs* stringStepHole1 = new G4Tubs("stringStepHole1", 0, 0.53*25.4*mm, 0.045*25.4*mm, 0, 2*pi);
	//Wenqin: dimension checked
	//Wenqin: bigger step hole with D=1.06"

	G4Tubs* stringStepHole2 = new G4Tubs("stringStepHole2", 0, 0.50*25.4*mm, 0.111*25.4*mm, 0, 2*pi);
	//Wenqin: dimension checked
	//smaller step hole with D=1"

	G4UnionSolid* stringMidHole = new G4UnionSolid("stringMidHole", stringThruHole, stringStepHole1,0,
				G4ThreeVector(0,0,0.155));
	//Wenqin: dimension checked
	//Wenqin: Zmax=0.2, Zheight=0.09, so Zmid=0.2-0.09/2=0.155

	G4UnionSolid* stringFinalHole = new G4UnionSolid("stringFinalHole", stringMidHole, stringStepHole2,0,
				G4ThreeVector(0,0,-0.001));
	//Wenqin: dimension checked
	//Wenqin: Zmax=0.2-0.09=0.11, so Zmid=0.11-0.222/2=-0.001	

	G4Tubs* no10thruhole = new G4Tubs("no10thruhole", 0, 0.1105*25.4*mm, 0.25*25.4*mm, 0, 2*pi);
	//Wenqin: dimension checked
	//Wenqin: small thru holes surrounding the string holes. 3 small for each string hole. D=5.613mm, or R=0.1105inch

	//Wenqin: These small holes have counterbore at the bottom
        G4Tubs* no10counter = new G4Tubs("no10counter", 0, 0.195*25.4*mm, 0.055*25.4*mm, 0, 2*pi);
	//Wenqin: dimension checked

        G4UnionSolid* no10thru = new  G4UnionSolid("no10thru",no10thruhole, no10counter, 0, G4ThreeVector(0,0,-1*(0.4-0.11)/2*25.4*mm)); //distance = half of the height difference
	//Wenqin: dimension checked

    	G4double holeRad = 0.75*25.4*mm;//Wenqin: on the xy plane, the distance from the small holes to big hole (center distance) is 19.085mm, or 0.75inch 
    	G4double holeAng = -10*deg;	//Wenqin: well, it is not arbitrary, and it is consistant with my survey, but why???
	G4UnionSolid* stringPatternA = new G4UnionSolid("stringPatternA", stringFinalHole, no10thru, 0,
					G4ThreeVector(holeRad*cos(holeAng), holeRad*sin(holeAng)));
	G4UnionSolid* stringPatternB = new G4UnionSolid("stringPatternB", stringPatternA, no10thru, 0,
					G4ThreeVector(holeRad*cos(holeAng+120*deg), holeRad*sin(holeAng+120*deg)));								
	G4UnionSolid* stringPattern = new G4UnionSolid("stringPattern", stringPatternB, no10thru, 0,
					G4ThreeVector(holeRad*cos(holeAng+240*deg), holeRad*sin(holeAng+240*deg)));			

	//Wenqin: the code above addes 3 small thru holes to each big thru hole for string, so UnionSolid is used

    	G4double stringRad = 3.73*25.4*mm; 	//Wenqin: the distnce between the big holes for strings
	//Wenqin: dimension checked
    	G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body1, stringPattern);
	G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, stringPattern, 0,
					G4ThreeVector(stringRad*cos(30*deg), stringRad*sin(30*deg), 0)); 
	G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, stringPattern, 0,
					G4ThreeVector(stringRad*cos(90*deg), stringRad*sin(90*deg), 0));
	G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, stringPattern, 0,
					G4ThreeVector(stringRad*cos(150*deg), stringRad*sin(150*deg), 0));
	G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, stringPattern, 0,
					G4ThreeVector(stringRad*cos(210*deg), stringRad*sin(210*deg), 0));					
	G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body6, stringPattern, 0,
					G4ThreeVector(stringRad*cos(270*deg), stringRad*sin(270*deg), 0));					
	G4SubtractionSolid* body8 = new G4SubtractionSolid("body8", body7, stringPattern, 0,
					G4ThreeVector(stringRad*cos(330*deg), stringRad*sin(330*deg), 0));					
	//Wenqin: the above subtracts the thru holes for 7 string assemblies, so SubtractionSolid is used 


	//Wenqin: NOW Below  are the 6 tapped holes in 2 x 3 array, close the the flat edge of the coldPlate, major D=0.312"	
	//Wenqin: according to SolidWork drawing, "MJD-Cryostat Systme 0.SLDASM", the bolts are "5-16th Hex Head.SLDPRT", major D=0.312" 

	/*G4Tubs* tappedHole = new G4Tubs("tappedHole", 0, 0.1435*25.4*mm, 0.25*25.4, 0, 2*pi);//original, no unit
	G4SubtractionSolid* body9 = new G4SubtractionSolid("body9", body8, tappedHole, 0,
					G4ThreeVector(1.625*25.4*mm, 1.0*25.4*mm, 0));			//wenqin: it s 1.645inch x
	G4SubtractionSolid* body10 = new G4SubtractionSolid("body10", body9, tappedHole, 0,
					G4ThreeVector(4.375*25.4*mm, 1.0*25.4*mm, 0));			//wenqin: it s 4.395inch x
	G4SubtractionSolid* body11 = new G4SubtractionSolid("body11", body10, tappedHole, 0,
					G4ThreeVector(5.375*25.4*mm, 1.0*25.4*mm, 0));			//wenqin: it is 5.395inch x
	G4SubtractionSolid* body12 = new G4SubtractionSolid("body12", body11, tappedHole, 0,
					G4ThreeVector(1.625*25.4*mm, -1.0*25.4*mm, 0));
	G4SubtractionSolid* body13 = new G4SubtractionSolid("body13", body12, tappedHole, 0,
					G4ThreeVector(4.375*25.4*mm, -1.0*25.4*mm, 0));
	G4SubtractionSolid* body14 = new G4SubtractionSolid("body14", body13, tappedHole, 0,
					G4ThreeVector(5.375*25.4*mm, -1.0*25.4*mm, 0));
	*/
	G4Tubs* tappedHole = new G4Tubs("tappedHole", 0, 0.136*25.4*mm, 0.25*25.4*mm, 0, 2*pi);
	//Wenqin: dimension NOT checked! The Diameter in drawing is 0.272", or r=0.136. But this is not the pitch diameter
	G4SubtractionSolid* body9 = new G4SubtractionSolid("body9", body8, tappedHole, 0,
					G4ThreeVector(1.645*25.4*mm, 1.0*25.4*mm, 0));			//wenqin: it s 1.645inch x
	G4SubtractionSolid* body10 = new G4SubtractionSolid("body10", body9, tappedHole, 0,
					G4ThreeVector(4.395*25.4*mm, 1.0*25.4*mm, 0));			//wenqin: it s 4.395inch x
	G4SubtractionSolid* body11 = new G4SubtractionSolid("body11", body10, tappedHole, 0,
					G4ThreeVector(5.395*25.4*mm, 1.0*25.4*mm, 0));			//wenqin: it is 5.395inch x
	G4SubtractionSolid* body12 = new G4SubtractionSolid("body12", body11, tappedHole, 0,
					G4ThreeVector(1.645*25.4*mm, -1.0*25.4*mm, 0));
	G4SubtractionSolid* body13 = new G4SubtractionSolid("body13", body12, tappedHole, 0,
					G4ThreeVector(4.395*25.4*mm, -1.0*25.4*mm, 0));
	G4SubtractionSolid* body14 = new G4SubtractionSolid("body14", body13, tappedHole, 0,
					G4ThreeVector(5.395*25.4*mm, -1.0*25.4*mm, 0));

    //Wenqin: NOW below are the small holes on the edge, they each have a shallow flat cut
	//according to "MJD-Cryostat Systme 0.SLDASM", the bolts are "Screw Copper Hex Hd 8-32x5_8.SLDPRT", major D=0.164"
    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.0885*25.4*mm, 0.25*25.4, 0, 2*pi); 
    //Wenqin: dimension NOT checked! The Major Diameter in drawing is 0.177", or r=0.0885". But this is not the pitch diameter. Would 0.07475 be the pitch diameter?
    G4Box* shallowflatcut = new G4Box("shallowflatcut", 0.25*25.4*mm, 0.025*25.4*mm, (0.01+0.005)*25.4*mm);	
	G4UnionSolid* thruHole1 = new G4UnionSolid("thruHole1", thruHole, shallowflatcut, 0,
									G4ThreeVector(0.25*25.4*mm, 0, (0.2-0.01+0.005)*25.4*mm) );

    G4double screwRad = 5.86*25.4*mm;
    G4double screwAng = 360.0/9.0*deg;
    G4double screwOffset = 40*deg;

	G4SubtractionSolid *body15s[8];
    G4RotationMatrix* flatRotations[8];
	char  flatcutname[8][100];

	flatRotations[0] = new G4RotationMatrix();
    	flatRotations[0] ->rotateZ(-1*(0*screwAng+screwOffset));
	//Wenqin: the rotation seems to begin in the opposite direction, and thus the -1*
	sprintf(flatcutname[0],"body15s%d",0);
    body15s[0] = new G4SubtractionSolid(flatcutname[0], body14, thruHole1, flatRotations[0],
    					G4ThreeVector(screwRad*cos(0*screwAng+screwOffset),	screwRad*sin(0*screwAng+screwOffset), 0));

	for(int ibody15=1;ibody15<8;ibody15++) {
		sprintf(flatcutname[ibody15],"body15s%d",ibody15);
		flatRotations[ibody15] = new G4RotationMatrix();
		flatRotations[ibody15]->rotateZ(-1*(ibody15*screwAng+screwOffset));
		body15s[ibody15] = new G4SubtractionSolid(flatcutname[ibody15], body15s[ibody15-1], thruHole1, 
												  flatRotations[ibody15],
                                        		  G4ThreeVector(screwRad*cos(ibody15*screwAng+screwOffset), 
                                        			  			screwRad*sin(ibody15*screwAng+screwOffset), 0)); }

        //Wenqin: NOW below we cut the for the hex bolts
        //Wenqin: all dimensions of the hex cut are checked!
        G4double    zPlanes2[]={-0.2*25.4*mm,0.2*25.4*mm};	
	G4double    rInner2[]={0*mm,0*mm};
	G4double    rOuter2[]={0.866*0.147*25.4*mm,0.866*0.147*25.4*mm}; 
    	G4Polyhedra* hexthru = new G4Polyhedra("hexthru", pi/2, 2*pi, 6, 2, zPlanes2, rInner2, rOuter2);
	//Wenqin: hex thru cut, pi/2 is phi start, which doesn't matter

        G4Tubs* hexcountertop = new G4Tubs("hexcountertop", 0, 0.24*25.4*mm, 0.055*25.4*mm, 0, 2*pi);
	//Wenqin: round top counterbore, 0.11" deep (0.4" disk - 0.21" hex hole -0.08" bottom counterbore =0.11)

        G4Tubs* hexcounterbot = new G4Tubs("hexcounterbot", 0, 0.19*25.4*mm, 0.04*25.4*mm, 0, 2*pi);
	//Wenqin: round bottom counterbore, 0.08" deep
 
     
  	G4UnionSolid* hexCutTmp = new G4UnionSolid("hexCutTmp", hexthru, hexcountertop, 0,
                                                                                G4ThreeVector(0,0,(0.4-0.11)/2*25.4*mm));

  	G4UnionSolid* hexCut = new G4UnionSolid("hexCut", hexCutTmp, hexcounterbot, 0,
                                                                                G4ThreeVector(0,0,-1*(0.4-0.08)/2*25.4*mm));

//    G4SubtractionSolid* RodWithHexCut = new G4SubtractionSolid("RodWithHexCut", hexAndThread,
  //                                                              hexCut);

       
      	//Wenqin: The coordinates in the Solid Work is a reflection of GEANT4, likely caused by an upside-down orintation in the Solid Work	
      	//Wenqin: As a result, one of the x or y coordiates read from the Solid Work must be applied a -1	 
  	G4double xHexhole[21]={
                        1.8*25.4*mm,1.74*25.4*mm,
                        4.854*25.4*mm,4.581*25.4*mm,1.614*25.4*mm,1.108*25.4*mm, -2.219*25.4*mm, -1.563*25.4*mm,
                        -1.708*25.4*mm,-1.112*25.4*mm,2.016*25.4*mm,1.887*25.4*mm,4.609*25.4*mm,4.882*25.4*mm,
                        //Wenqin: These are the 2x7=14 hex tapped holes for HV Twists, Clock-wise+first one in the center        
                        2.415*25.4*mm,-1.664*25.4*mm,-4.49*25.4*mm,-3.74*25.4*mm,-0.35*25.4*mm,2.566*25.4*mm,-0.10*25.4*mm
                        //Wenqin: These are the 7 hex tapped holes for slica stacks     
                        };//Wenqin: They are organized as Clock-wise+last one in the center        

  	G4double yHexhole[21]={
                       -0.01*25.4*mm, 1.529*25.4*mm,
                       -1.106*25.4*mm,-2.624*25.4*mm,-3.386*25.4*mm,-4.84*25.4*mm,-3.066*25.4*mm,-1.673*25.4*mm,
                       //Wenqin: These are the 2x7=14 hex tapped holes for HV Twists, Clock-wise+first one in the center        
                       3.478*25.4*mm,2.058*25.4*mm,3.893*25.4*mm,5.428*25.4*mm,0.801*25.4*mm,2.316*25.4*mm,
                       -0.05*25.4*mm,-4.000*25.4*mm,-2.23*25.4*mm,3.72*25.4*mm,5.39*25.4*mm,3.646*25.4*mm,1.65*25.4*mm
                      };
/*
	G4double xHexhole[21]={	-4.49*25.4*mm,-3.74*25.4*mm, -2.219*25.4*mm,-1.664*25.4*mm,-1.563*25.4*mm,
				1.108*25.4*mm,1.614*25.4*mm,1.8*25.4*mm,2.415*25.4*mm,4.581*25.4*mm,4.854*25.4*mm,
				-1.708*25.4*mm,-1.112*25.4*mm,-0.35*25.4*mm,-0.10*25.4*mm,
				1.74*25.4*mm,1.887*25.4*mm,2.016*25.4*mm,2.566*25.4*mm,4.609*25.4*mm,4.882*25.4*mm};

	G4double yHexhole[21]={	-2.23*25.4*mm,3.72*25.4*mm,-3.066*25.4*mm,-4.000*25.4*mm,-1.673*25.4*mm,
				-4.84*25.4*mm,-3.386*25.4*mm,-0.01*25.4*mm,-0.05*25.4*mm,-2.624*25.4*mm,-1.106*25.4*mm,
				3.478*25.4*mm,2.058*25.4*mm,5.39*25.4*mm,1.65*25.4*mm,
				1.529*25.4*mm,5.428*25.4*mm,3.893*25.4*mm,3.646*25.4*mm,0.801*25.4*mm,2.316*25.4*mm};
	//Wenqin: same coordinates as above, just organized according to x values rouphly.
*/

	G4SubtractionSolid* body15h_hex[21];
	body15h_hex[0] = new G4SubtractionSolid("body15h_hex0", body15s[7], hexCut,0,
					G4ThreeVector(-1*xHexhole[0],-1*yHexhole[0], 0));
	char hexnames[21][100];
	for(int hexi=1;hexi<21;hexi++)
	{
		sprintf(hexnames[hexi],"body15h_hex%d",hexi);
		body15h_hex[hexi] = new G4SubtractionSolid(hexnames[hexi], body15h_hex[hexi-1], hexCut,0, G4ThreeVector(-1*xHexhole[hexi],-1*yHexhole[hexi], 0));
	}//there are 21 of them
	
	
/*	G4SubtractionSolid* body15 = new G4SubtractionSolid("body10", body9, tappedHole, 0,
					G4ThreeVector(4.375*25.4*mm, 1.0*25.4*mm, 0));
	G4SubtractionSolid* body11 = new G4SubtractionSolid("body11", body10, tappedHole, 0,
					G4ThreeVector(5.375*25.4*mm, 1.0*25.4*mm, 0));
	G4SubtractionSolid* body12 = new G4SubtractionSolid("body12", body11, tappedHole, 0,
					G4ThreeVector(1.625*25.4*mm, -1.0*25.4*mm, 0));
	G4SubtractionSolid* body13 = new G4SubtractionSolid("body13", body12, tappedHole, 0,
					G4ThreeVector(4.375*25.4*mm, -1.0*25.4*mm, 0));
	G4SubtractionSolid* body14 = new G4SubtractionSolid("body14", body13, tappedHole, 0,
					G4ThreeVector(5.375*25.4*mm, -1.0*25.4*mm, 0));
*/

	//Wenqin: NOW these are the three taps at the edge 
    	//G4Box* tab = new G4Box("tab", 0.25*25.4*mm, 0.25*25.4*mm, 0.2*25.4*mm); //original
	G4Box* tab = new G4Box("tab", 0.1035*25.4*mm, 0.25*25.4*mm, 0.2*25.4*mm); //6.457"-6.25"=0.207", 0.207/2=0.1035"
	//Wenqin: dimension checked. tap edge to center = 6.457", disk edge to center = 6.25". Tap = 0.207" wide

    	G4Tubs* blindHole = new G4Tubs("blind hole", 0, 0.065*25.4*mm, 0.125*25.4*mm, 0, 2*pi);
	//Wenqin: dimension checked

    	G4SubtractionSolid* tab2 = new G4SubtractionSolid("tab2", tab, blindHole, 0, 
    										G4ThreeVector(0, 0, -0.1375*25.4*mm));
    										//G4ThreeVector(0, 0, -0.1575*25.4*mm));//original
	//Wenqin: dimension checked. distance = depth difference/2= (0.4-0.125)/2=0.1375

    	G4UnionSolid* body16 = new G4UnionSolid("body16", body15h_hex[20], tab2, 0, 

    										G4ThreeVector(-6.32*25.4*mm, 0, 0));    
	//Wenqin: 6.3202(orignially)->6.32. dimension checked

    	G4RotationMatrix* tabRotation1 = new G4RotationMatrix();
    	tabRotation1->rotateZ(-120*deg);
    	G4UnionSolid* body17 = new G4UnionSolid("body17", body16, tab2, tabRotation1, 
    										G4ThreeVector(-6.32*25.4*mm*cos(120*deg), -6.32*25.4*mm*sin(120*deg), 0));    
    	G4RotationMatrix* tabRotation2 = new G4RotationMatrix();
    	tabRotation2->rotateZ(-240*deg);
    	G4UnionSolid* body18 = new G4UnionSolid("body18", body17, tab2, tabRotation2,
							    			G4ThreeVector(-6.32*25.4*mm*cos(240*deg), -6.32*25.4*mm*sin(240*deg), 0));    

        //Wenqin: don't know why need the cylinder cuts, skip body19 and 20
    	//G4Tubs* cylinderCut1 = new G4Tubs("cylinder1", 6.462*25.4*mm, 7*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    	//G4SubtractionSolid* body19 = new G4SubtractionSolid("body19", body18, cylinderCut1);
	//original    

    	//G4Tubs* cylinderCut2 = new G4Tubs("cylinder2", 6.099*25.4*mm, 6.18*25.4*mm, 0.08*25.4*mm, 0, 2*pi);//6.1
    	//G4SubtractionSolid* body20 = new G4SubtractionSolid("body20", body19, cylinderCut2, 0,
    	//									G4ThreeVector(0, 0, -0.1575*25.4*mm));
	//original

	

	//Wenqin: NOW, below should be the 3 small pin holes ON the edge of the disk (one of them is close to the tap. view from the cross section)
    	//G4Tubs* pinHole = new G4Tubs("pinHole", 0, 0.065*25.4*mm, 0.487*25.4*mm, 0, 2*pi); //0.487*2~1" very deep hole. not matching the drawing
    	G4Tubs* pinHole = new G4Tubs("pinHole", 0, 0.065*25.4*mm, 0.125*25.4*mm, 0, 2*pi);
    	G4RotationMatrix* pinRotation1 = new G4RotationMatrix();
    	pinRotation1->rotateZ(-20*deg);
	//Wenqin: the 20 degree angle is not checked!
    	pinRotation1->rotateY(pi/2);

//    	G4SubtractionSolid* body21 = new G4SubtractionSolid("body21", body20, pinHole, pinRotation1, G4ThreeVector(6.1*25.4*mm*cos(20*deg), 6.1*25.4*mm*sin(20*deg), 0));//original
    	G4SubtractionSolid* body21 = new G4SubtractionSolid("body21", body18, pinHole, pinRotation1, 
				    						G4ThreeVector(6.125*25.4*mm*cos(20*deg), 6.125*25.4*mm*sin(20*deg), 0));
	//Wenqin: 6.25"-0.125"=6.125", same for the other 2. Dimension checked, but angle not!
    	G4RotationMatrix* pinRotation2 = new G4RotationMatrix();
    	pinRotation2->rotateZ(20*deg);
    	pinRotation2->rotateY(pi/2);

    	G4SubtractionSolid* body22 = new G4SubtractionSolid("body22", body21, pinHole, pinRotation2, 
				    						G4ThreeVector(6.125*25.4*mm*cos(-20*deg), 6.125*25.4*mm*sin(-20*deg), 0));
    	G4RotationMatrix* pinRotation3 = new G4RotationMatrix();
    	pinRotation3->rotateZ(-190*deg);
    	pinRotation3->rotateY(pi/2);

    	G4SubtractionSolid* plate_only = new G4SubtractionSolid("plate_only", body22, pinHole, pinRotation3, 
				    						G4ThreeVector(6.125*25.4*mm*cos(190*deg), 6.125*25.4*mm*sin(190*deg), 0));    						
    	//======================= The Cold Plate itself is finished	 		=========================//


    	//======================= Now add the covers, bolts and other attachments 	=========================//
    	//Wenqin: NOW The covers for the string hole
	//Wenqin: all the dimensions for the covers are checked   
 	G4Tubs* holecoverupper1 = new G4Tubs("holecoverupper1", (0.50*25.4*mm-mjdSurfTol), (0.53*25.4*mm-mjdSurfTol), 0.06*25.4*mm, 0,2*pi);
    	//The rim of upper cover, 0.06" height, D_out=1.06", D_in=1.0", 	
    	G4Tubs* holecoverupper2 = new G4Tubs("holecoverupper2", 0, (0.53*25.4*mm-mjdSurfTol), 0.04*25.4*mm, 0, 2*pi);
 	//The base of upper cover, 0.04" height

    	//now, we cut out the open part of the cover
    	G4Box*  covercut = new G4Box("covercut",1*25.4*mm, 1*25.4*mm, 1*25.4*mm); 
    	//the dimension just should be > 0.15", since the position of edge to the cover is 0.25"



    	G4UnionSolid* coverupper2 = new G4UnionSolid("coverupper2", holecoverupper1, holecoverupper2,0,
                                G4ThreeVector(0,0,-0.03*25.4*mm)); //combine to one cover
    	G4SubtractionSolid* coverupper = new G4SubtractionSolid("coverupper",coverupper2,covercut,0,G4ThreeVector((1+0.25)*25.4*mm,0,0)); 

    	G4Tubs* holecoverlower1 = new G4Tubs("holecoverlower1", (0.47*25.4*mm-mjdSurfTol), (0.50*25.4*mm-mjdSurfTol), 0.06*25.4*mm, 0, 2*pi);
    	//The rim of lower cover, 0.06" height, D_out=1.0", D_in=0.94"	
    	G4Tubs* holecoverlower2 = new G4Tubs("holecoverlower2", 0, (0.50*25.4*mm-mjdSurfTol), 0.04*25.4*mm, 0, 2*pi);
    	//The base of lower cover, 0.04" height
    	G4UnionSolid* coverlower2 = new G4UnionSolid("coverlower2", holecoverlower1, holecoverlower2,0,
                                G4ThreeVector(0,0,-0.03*25.4*mm));
    	G4SubtractionSolid* coverlower = new G4SubtractionSolid("coverlower",coverlower2,covercut,0,G4ThreeVector((1+0.25)*25.4*mm,0,0));

    	G4double cover_upperandlower_distance = (0.04+0.222)*25.4*mm; //0.04" thick base + 0.222" between two bases

    	G4RotationMatrix* cover_upperandlower_Rotation = new  G4RotationMatrix();
    	cover_upperandlower_Rotation->rotateZ(pi);

    	G4UnionSolid* holecover = new  G4UnionSolid("holecover", coverupper,coverlower,cover_upperandlower_Rotation, G4ThreeVector(0,0,-1*cover_upperandlower_distance));

    	G4double cover_upperandhole_distance=((0.09+0.222+0.088)/2 + (0.1-0.09) - 0.06/2)*25.4*mm;
    	//Wenqin: from top to bottom, the 3 step holes have depth: 0.09", 0.222", and 0.088". The distance = hole top - hole center + (cover top - hole top) - (cover rim /2)
    	//  Wenqin: hole top - hole center = (0.09+0.222+0.088)/2 " = 
    	// Wenqin: cover top - hole top = cover height - top step hole depth, since the upper cover is sitting on top of the top step hole

//    	G4UnionSolid* plate_only1 = new G4UnionSolid("plate_only1", coverupper2, holecover,0, // for viewing the covers only 
    	G4UnionSolid* plate_only1 = new G4UnionSolid("plate_only1", plate_only, holecover,0,  
				    	G4ThreeVector(0,0,cover_upperandhole_distance));						

    	G4UnionSolid* plate_only2 = new G4UnionSolid("plate_only2", plate_only1, holecover,0,  
                                        G4ThreeVector(stringRad*cos(30*deg), stringRad*sin(30*deg), cover_upperandhole_distance));

    	G4UnionSolid* plate_only3 = new G4UnionSolid("plate_only3", plate_only2, holecover,0,  
                                        G4ThreeVector(stringRad*cos(90*deg), stringRad*sin(90*deg), cover_upperandhole_distance));

    	G4UnionSolid* plate_only4 = new G4UnionSolid("plate_only4", plate_only3, holecover,0,  
                                        G4ThreeVector(stringRad*cos(150*deg), stringRad*sin(150*deg), cover_upperandhole_distance));

    	G4UnionSolid* plate_only5 = new G4UnionSolid("plate_only5", plate_only4, holecover,0,  
                                        G4ThreeVector(stringRad*cos(210*deg), stringRad*sin(210*deg), cover_upperandhole_distance));

    	G4UnionSolid* plate_only6 = new G4UnionSolid("plate_only6", plate_only5, holecover,0,  
                                        G4ThreeVector(stringRad*cos(270*deg), stringRad*sin(270*deg), cover_upperandhole_distance));

    	G4UnionSolid* plate_hole_covered       = new G4UnionSolid("plate_hole_covered"      , plate_only6, holecover,0,  
                                        G4ThreeVector(stringRad*cos(330*deg), stringRad*sin(330*deg), cover_upperandhole_distance));
	
	//Wenqin: NOW the Coldplate Captured Hex Bolt. See Coldplate Captured Hex Bolt.SLDPRT
	//Wenqin: We make them pointing up with the head down, as the way they will be installed to the cold plate
        G4double    zPlanes[]={0,-0.135*25.4*mm,-0.135*25.4*mm,-0.195*25.4*mm}; //Z of the bolt is the junction of the small hex head and the round body	
	G4double    rInner[]={0*mm,0*mm,0*mm,0*mm};
	G4double    rOuter[]={0.866*0.144*25.4*mm,0.866*0.144*25.4*mm,0.866*0.18*25.4*mm,0.866*0.18*25.4*mm}; //it should be sqrt(3)/2=0.886 
    	G4Polyhedra* CapturedHexBolthead = new G4Polyhedra("CapturedHexBoltheat", pi/2, 2*pi, 6, 4, zPlanes, rInner, rOuter);
	//Wenqin: dimensions checked, and the size 0.144" is smaller than the hex hole 0.147",  so no tolerance is needed
	//Wenqin: The tangent distance should be sqrt(3)/2=0.886 times the length of the sides.

        G4Tubs * CapturedHexBoltbody = new G4Tubs("CapturedHexBoltbody",0,0.059*25.4*mm-mjdSurfTol,0.24*25.4*mm,0, 2*pi);
    	//Wenqin: major diameter = 0.138", minor diameter = 0.098", so pitch diameter = 0.118

	G4UnionSolid* CapturedHexBolt       = new G4UnionSolid("CapturedHexBolt"      , CapturedHexBolthead,CapturedHexBoltbody, 0, 
                                        G4ThreeVector(0,0,0.24*25.4*mm));
	
	
	G4UnionSolid* plate_w_captured_hex_bolts[21];
	//plate_w_captured_hex_bolts[0] = new G4UnionSolid("plate_w_captured_hex_bolts0", CapturedHexBoltbody, CapturedHexBolt,0, 		//Wenqin: for views the bolts and nuts only
	plate_w_captured_hex_bolts[0] = new G4UnionSolid("plate_w_captured_hex_bolts0", plate_hole_covered, CapturedHexBolt,0,
					G4ThreeVector(-1*xHexhole[0],-1*yHexhole[0], 0.045*25.4*mm));
  	//Wenqin: NOW the round top counterbore, 0.11" deep and the bigger hex head sits on it, not the samll hex
	//Wenqin: so the -0.135" surface of the CapturedHexBolt is at a distance of (0.2"-0.11")=0.09" below Z=0, i.e., the 0" center of the CapturedHexBolt is at z=-0.09-(-0.135)=0.045"
	//Wenqin: since I am making a UnionSolid, no tolerance is really necessary, otherwise 0.045"-tolerance should be used, since the bolt comes from the bottom

	char plate_bolt_names[21][100];
	for(int bolti=1;bolti<21;bolti++)
	{
		sprintf(plate_bolt_names[bolti],"plate_w_captured_hex_bolts%d",bolti);
		plate_w_captured_hex_bolts[bolti] = new G4UnionSolid(plate_bolt_names[bolti], plate_w_captured_hex_bolts[bolti-1], CapturedHexBolt,0, G4ThreeVector(-1*xHexhole[bolti],-1*yHexhole[bolti], 0.045*25.4*mm));
	}//there are 21 of them
	//Wenqin: Omission, The very shallow groove for each hex hole is omitted
	

	//Wenqin: NOW the wider No. 6 hex nuts that sits on the counterbore of the hex hole, underneath the silica stack hold block
	//Wenqin: we make the lower hex surface to be at z=0.09", since it sits at 0.2"-0.11"=0.09" (0.11"= depth of the counterbore) above disk center. Do not need shift later  
	//Wenqin: the height also happens to be 0.09", and 0.09"+0.09"=0.18"<0.2", so it does not touch the silica 
        G4double    zPlanesNut6Wider[]={0.09*25.4*mm+mjdSurfTol,0.09*25.4*mm+0.09*25.4*mm}; //the height also happens to be 0.09"
        G4double    rInnerNut6Wider[]={0*mm,0*mm};
        G4double    rOuterNut6Wider[]={0.866*0.18*25.4*mm,0.866*0.18*25.4*mm}; 
	G4Polyhedra* hexNut6wider_hex = new G4Polyhedra("hexNut6wider_hex", pi/2, 2*pi, 6, 2, zPlanesNut6Wider, rInnerNut6Wider, rOuterNut6Wider);

	G4Tubs* hexNut6_Hole = new G4Tubs("hexNut6_Hole", 0, 0.063*25.4*mm, 1*25.4*mm, 0, 2*pi);
	//Wenqin: minor D= 0.106", major D=? unknown. The minor D < the minor D of the the CapturedHexBolt above 0.098" by 0.106-0.098 = 0.008" 
	//Wenqin: use the pitch diameter from the CapturedHexBolt above + 0.008". 0.059"+0.008"/2=0.063", no tolerance needed. The height is not a problem

    	G4SubtractionSolid* hexNut6Wider = new G4SubtractionSolid("hexNut6Wider", hexNut6wider_hex, hexNut6_Hole, 0, G4ThreeVector(0,0,0));


	//Wenqin: NOW the No. 6 hex nuts that sits on the silica stack hold block, which in  turn sits on the top surface of the hex coldplate. 
	//Wenqin: and the silica block is 0.084" height, so the lower hex surface is at 0.2+0.084=0.284" from the plate center. Do not need shift later 
        G4double    zPlanesNut6[]={0.284*25.4*mm+mjdSurfTol,0.284*25.4*mm+0.109*25.4*mm}; //the height is 0.109"
        G4double    rInnerNut6[]={0*mm,0*mm};
        G4double    rOuterNut6[]={0.866*0.144*25.4*mm,0.866*0.144*25.4*mm}; 
	G4Polyhedra* hexNut6_hex = new G4Polyhedra("hexNut6_hex", pi/2, 2*pi, 6, 2, zPlanesNut6, rInnerNut6, rOuterNut6);

    	G4SubtractionSolid* hexNut6 = new G4SubtractionSolid("hexNut6", hexNut6_hex, hexNut6_Hole, 0, G4ThreeVector(0,0,0));
	//Wenqin: it is the same hole as for the wider Nut


        G4UnionSolid* plate_w_nuts1[21];
        G4UnionSolid* plate_w_nuts2[21];
        plate_w_nuts1[0] = new G4UnionSolid("plate_w_nuts1_0", plate_w_captured_hex_bolts[20], hexNut6Wider,0,
                                        G4ThreeVector(-1*xHexhole[0],-1*yHexhole[0], 0.045*25.4*mm));
        plate_w_nuts2[0] = new G4UnionSolid("plate_w_nuts2_0", plate_w_nuts1[0], hexNut6,0,
                                        G4ThreeVector(-1*xHexhole[0],-1*yHexhole[0], 0.045*25.4*mm));
        char plate_nut_names1[21][100];
        char plate_nut_names2[21][100];
        for(int nuti=1;nuti<21;nuti++)
        {
                sprintf(plate_nut_names1[nuti],"plate_w_nuts1_%d",nuti);
                sprintf(plate_nut_names2[nuti],"plate_w_nuts2_%d",nuti);
                plate_w_nuts1[nuti]=new G4UnionSolid(plate_nut_names1[nuti], plate_w_nuts2[nuti-1], hexNut6Wider,0, G4ThreeVector(-1*xHexhole[nuti],-1*yHexhole[nuti], 0.045*25.4*mm));
                plate_w_nuts2[nuti]=new G4UnionSolid(plate_nut_names2[nuti], plate_w_nuts1[nuti], hexNut6,0, G4ThreeVector(-1*xHexhole[nuti],-1*yHexhole[nuti], 0.045*25.4*mm));
        }//Wenqin:there are 21 of them. plate_w_nuts2[20] is the plate with all of them	
	

	//Wenqin: Note, the bottom of the cold plate is at z=-0.2".

    	//======================= Set Copper Color 					===========================//					
    	// G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    	G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    	// redVisAtt->SetForceWireframe( false );
    	copperVisAtt->SetForceWireframe( false );
    	G4Material *material = G4Material::GetMaterial(this->GetMaterial());
//    	pVol = new G4LogicalVolume(plate, material, logicalName);
//    	pVol = new G4LogicalVolume(plate_hole_covered, material, logicalName);
    	pVol = new G4LogicalVolume(plate_w_nuts2[20], material, logicalName);
    	// pVol->SetVisAttributes(redVisAtt); 
    	pVol->SetVisAttributes(copperVisAtt); 
    	MGLog(debugging) << "Created Cold Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cold Plate Logical" << endlog; 
  return pVol;
} 
