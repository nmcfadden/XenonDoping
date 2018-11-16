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
/**
 *
 * DESCRIPTION: Please see the header file.
 * 
 * AUTHOR:
 * 	Jing Liu
 * 
 * REVISION: MM-DD-YYYY
 *
 * 	06-30-2008, created, Jing Liu
 * 	07-09-2008, initialized fGammaMatrixReady in constructor;
 * 	            added more comments and some FIXMEs;
 * 	            set very small elements in matrix to zero;
 * 	            implemented CLHEP unit system;
 * 	            and some other minor changes. Jing
 * 	07-11-2008, added some output. Jing
 * 	08-07-2008, took into account the difference between the lab coordinate 
 * 	            systerm and the coordinate defined by axes <100>,<010> and
 * 	            <001> in the calculation of hole drift. Jing
 * 	08-12-2008, got the components of eVector in coordin.<100><010><001> 
 * 	            before the calculation of hole drift. Jing
 * 	08-22-2008, changed some output from "routine" to "debugging". Jing
 * 	09-11-2008, added another set of input parameters for the calculation.
 * 	            Jing
 *      05-27-2011, Change name to MGBruyneelDriftVelocity, use base class
 *                  MGVDriftVelocity, call SetupGammaMatrix() in constructor
 *                  to adapt to const protections in base class. J.Detwiler
 *
 */

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Matrix/Vector.h"

#include "MGDOUtils.hh"

#include "io/MGLogger.hh"
#include "waveform/MGBruyneelDriftVelocity.hh"

using namespace CLHEP;

// ----------------------------------------------------------------------------

MGBruyneelDriftVelocity::MGBruyneelDriftVelocity(const MGVCrystalFields& fields) :
  MGVDriftVelocity(fields)
{
  SetupGammaMatrix();
}

void MGBruyneelDriftVelocity::SetCrystalFields(const MGVCrystalFields& fields)
{
  MGVDriftVelocity::SetCrystalFields(fields);
  SetupGammaMatrix();
}

// ----------------------------------------------------------------------------
// prepare gamma matrix for the calculation of electron drift velocity

double SetSmallValueToZero(double value, int, int)
{
   if (std::fabs(value)>1e-7) return value;
   else return 0.0;
}

HepMatrix MGBruyneelDriftVelocity::RotationAroundX(double angle)
{
   HepMatrix matrix(3,3,0);
   matrix(1,1) = 1.;
   matrix(2,2) = cos(angle);
   matrix(2,3) = sin(angle);
   matrix(3,2) =-sin(angle);
   matrix(3,3) = cos(angle); 

   return matrix;
}

HepMatrix MGBruyneelDriftVelocity::RotationAroundZ(double angle)
{
   HepMatrix matrix(3,3,0);
   matrix(1,1) = cos(angle);
   matrix(1,2) = sin(angle);
   matrix(2,1) =-sin(angle);
   matrix(2,2) = cos(angle); 
   matrix(3,3) = 1.;

   return matrix;
}

// FIXME: crystral axis 001 is assumed to be parallel to Z-axis,
// this is not always the case...
void MGBruyneelDriftVelocity::SetupGammaMatrix()
{
   MGLog(debugging)<<"Setting up Gamma Matrix..."<<endlog;
   
   HepMatrix RxLeft(3,3),RxRight(3,3); // rotation matrix around x
   HepMatrix RzLeft(3,3),RzRight(3,3); // rotation matrix around z

   RxRight= RotationAroundX(asin(1./sqrt(3.)));
   RxLeft = RxRight.T();

   double Mt = 0.0819; // transverse effective electron mass, = 0.0819*Me
   double Ml = 1.64;   // longitudinal effective electron mass, = 1.64*Me
		       // Because the results are independent of Me,
                       // Me is set to be 1 through all the calculation.
   
   // initialize fGamma[i] to 3x3 matrix with all elements = 0
   for (int i=0; i<5; i++)
      fGamma[i] = HepMatrix(3,3,0);
   
   // setup fGamma[0]
   fGamma[0](1,1) = 1./Mt;
   fGamma[0](2,2) = 1./Ml;
   fGamma[0](3,3) = 1./Mt;

   // setup fGamma[i]
   double phi110 = GetCrystalData().GetCrystalAxesAngle();
   MGLog(debugging)<<"crystal axis angle = "<<phi110/degree
                   <<" degree"<<endlog;
   for (int i=1; i<5; i++){
      RzRight= RotationAroundZ(phi110+(i-1)*halfpi);
      RzLeft = RzRight.T();
      fGamma[i] = (RzRight*RxLeft*fGamma[0]*RxRight*RzLeft)
	 .apply(SetSmallValueToZero);
   }

   MGLog(debugging)<<"Gamma Matrix are Ready."<<endlog;
}

void MGBruyneelDriftVelocity::PrintGammaMatrix()
{
   for (int i=0; i<5; i++)
      std::cout<<"Gamma["<<i<<"] = "<<fGamma[i];
}

// ----------------------------------------------------------------------------
// FIXME: All the parameters in the empirical formula below are hard coded. 
// Better to provide a function to load parameters from file.
// OR, to get the velocity by interpolating data tables.
double MGBruyneelDriftVelocity::V100e(double eMag) const
{
   //data from NIMA 447 (2000) 350
   //double mu_0 = 4018000.0*mm2/(volt*second);
   //double mu_n = 58900.0*mm2/(volt*second);
   //double E_0  = 49.3*volt/mm;
   //double beta = 0.72;
   
   //data from Bart Bruyneel's thesis
   double mu_0 = 3860900.0*mm2/(volt*second);
   double mu_n = -17100.0*mm2/(volt*second);
   double E_0  = 51.1*volt/mm;
   double beta = 0.805;
   return mu_0*eMag/pow((1 + pow((eMag/E_0),beta)),1/beta) - mu_n*eMag;
}

double MGBruyneelDriftVelocity::V111e(double eMag) const
{
   //data from NIMA 447 (2000) 350
   //double mu_0 = 4242000.0*mm2/(volt*second);
   //double mu_n = 6200.0*mm2/(volt*second);
   //double E_0  = 25.1*volt/mm;
   //double beta = 0.87;

   //data from Bart Bruyneel's thesis
   double mu_0 = 3853600.0*mm2/(volt*second);
   double mu_n = 51000.0*mm2/(volt*second);
   double E_0  = 53.8*volt/mm;
   double beta = 0.641;
   return mu_0*eMag/pow((1 + pow((eMag/E_0),beta)),1/beta) - mu_n*eMag;
}

double MGBruyneelDriftVelocity::V100h(double eMag) const
{
   //data from Phys. Rev. B 16 (1977) 2781
   //double mu_0 = 6633300.0*mm2/(volt*second);
   //double E_0  = 18.1*volt/mm;
   //double beta = 0.744;

   //data from Bart Bruyneel's thesis
   double mu_0 = 6182400.0*mm2/(volt*second);
   double E_0  = 18.5*volt/mm;
   double beta = 0.942;
   return mu_0*eMag/pow((1 + pow((eMag/E_0),beta)),1/beta);
}

double MGBruyneelDriftVelocity::V111h(double eMag) const
{
   //data from Phys. Rev. B 16 (1977) 2781
   //double mu_0 = 10727000.0*mm2/(volt*second);
   //double E_0  = 10.0*volt/mm;
   //double beta = 0.580;

   //data from Bart Bruyneel's thesis
   double mu_0 = 6121500.0*mm2/(volt*second);
   double E_0  = 18.2*volt/mm;
   double beta = 0.662;
   return mu_0*eMag/pow((1 + pow((eMag/E_0),beta)),1/beta);
}

double MGBruyneelDriftVelocity::A(double eMag) const
{
   //double Afactor = 1.15;
   double Afactor = 1.;
   double gamma0  = 2.888470213;
   return Afactor*V100e(eMag)/gamma0;
}

double MGBruyneelDriftVelocity::R(double eMag)  const
{
   //double Rfactor = 1.69;
   double Rfactor = 1.;
   return Rfactor*(-1.182108526*V111e(eMag)/A(eMag) + 3.160660533);
}

// ----------------------------------------------------------------------------
// electrons: please refer to NIMA 447 (2000) 350-360
// (there are some mistakes in the paper, but the basic idea is there.)
Hep3Vector MGBruyneelDriftVelocity::GetVe(const Hep3Vector& eVector) const
{
   Hep3Vector ve(0,0,0);

   double eMag = eVector.mag();
   if (eMag/(volt/mm) < 1e-2){
      MGLog(debugging)<<"The magnitude of E = "
	            <<eMag/(volt/mm)
		    <<" volt/mm,"<<endlog;
      MGLog(debugging)<<"which is too small. Ve is set to be zero."<<endlog;
   }
   else{
      HepVector e0(3,0);
      e0 = eVector.unit();

      HepVector gammaE[5];
      double oneOverSqrtEgE[5], sumOfOneOverSqrtEgE = 0.;
      for (int i=1; i<=4; i++){
	 gammaE[i] = fGamma[i]*e0;
	 oneOverSqrtEgE[i] = 1./sqrt(dot(e0,gammaE[i]));
	 sumOfOneOverSqrtEgE += oneOverSqrtEgE[i];
      }
   
      double re = R(eMag), ae = -A(eMag);
      double NiOverN[5];
      HepVector gamma0(3,0);
      for (int i=1; i<=4; i++){
	 NiOverN[i] = re*(oneOverSqrtEgE[i]/sumOfOneOverSqrtEgE-0.25)+0.25;
	 gamma0 += NiOverN[i]*gammaE[i]*oneOverSqrtEgE[i];
      }
      if (std::fabs(gamma0(1))>1e-5) ve.setX(ae*gamma0(1));
      if (std::fabs(gamma0(2))>1e-5) ve.setY(ae*gamma0(2));
      if (std::fabs(gamma0(3))>1e-5) ve.setZ(ae*gamma0(3));
   }
   return ve;
}

// ----------------------------------------------------------------------------
// holes: refer to Ph.D. thesis of Bart Bruyneel:
// "Characterization of Segmented Large Volume, High Purity Germanium
// Detectors" page 59-61
Hep3Vector MGBruyneelDriftVelocity::GetVh(const Hep3Vector& eVector) const
{
   double phi110 = fCrystalFields->GetCrystalData().GetCrystalAxesAngle();
   
   Hep3Vector vh(0,0,0);
   Hep3Vector myEVector = eVector;

   double eMag = myEVector.mag();
   if (eMag/(volt/mm) < 1e-2){
      MGLog(debugging)<<"The magnitude of E = "<<eMag<<" voltage/mm,"<<endlog;
      MGLog(debugging)<<"which is too small. Vh is set to be zero."<<endlog;
   }
   else{
      myEVector.rotateZ(-pi/4-phi110);//eVector in coordin.<100><010><001>
      double theta= myEVector.theta();
      double phi  = myEVector.phi();

      double sinTheta  = sin(theta);
      double sinTheta3 = sinTheta*sinTheta*sinTheta;
      double sinTheta4 = sinTheta3*sinTheta;
      double sin2Theta = sin(2*theta);
      double sin2Theta2= sin2Theta*sin2Theta;
      double sin4Theta = sin(4*theta);
      double sin4Phi   = sin(4*phi);
      double sin2Phi   = sin(2*phi);
      double sin2Phi2  = sin2Phi*sin2Phi;
      double cosTheta  = cos(theta);

      double k0 = K0(V111h(eMag)/V100h(eMag));
   
      double v_rad   = V100h(eMag)*(1-Lambda(k0)*(sinTheta4*sin2Phi2+sin2Theta2));
      double v_theta = V100h(eMag)*Omega(k0)*(2*sinTheta3*cosTheta*sin2Phi2+sin4Theta);
      double v_phi   = V100h(eMag)*Omega(k0)*sinTheta3*sin4Phi;

      double vx=0., vy=0., vz=0.;
      if (std::fabs(v_rad/(mm/ns))>1e-5) vz = v_rad;
      if (std::fabs(v_theta/(mm/ns))>1e-5) vx = v_theta;
      if (std::fabs(v_phi/(mm/ns))>1e-5) vy = v_phi;
      
      vh.set(vx, vy, vz);
      // rotate from local coordin. to lab coordin.
      vh.rotateY(theta);
      vh.rotateZ(phi+pi/4.+phi110);
   }
   return vh;
}

