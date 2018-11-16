//Author: Daniel Lenz
//created: some time ago
//
//ROOT script to calculate the electric field along r inside a true coaxial germanium detector. For N and P type
//
//Last modification: 21.08.2008
//
//
////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////
double myEFieldPTypefunction(double x){
  //this is explicitly only for p-type!
  //for n-type the sign of the chargeCarrierDensity * elementary charge is reversed.

  //change this!
  double chargeCarrierDensity = 0e10 * 1000000; //[m^-3] 
  double Potential = 3000;//[V] defined as V = Volt(router)-Volt(rinner)
  double innerR    = 0.0015; //[m]
  double outerR    = 0.014; //[m]

  //  constansts
  double elementaryCharge = 1.602176487e-19; //[C]
  double epsilon0 =  8.8541878176e-12; //[C^2 *N^-1*m^-2]
  double epsilonR =  16;

  double firstTerm = x * elementaryCharge * chargeCarrierDensity /(2 * epsilon0 * epsilonR ); //[ m * C * m^-3 *C^-2 * N * m^2] = [N/C] =[V/m]
  

  double denominator = x * log(outerR/innerR); //[m]

  double secondTerm = Potential; // [V/m]


  double thirdTerm = ((elementaryCharge * chargeCarrierDensity / (4 * epsilon0 *epsilonR)) * (outerR*outerR - innerR*innerR)); //[ C * m^-3 * C^-2 * N *m^2 * m^2 m^-1] = [N/C] =[V/m]

  //changed first term sign to plus and third term sign to minus
  //not any more
  return   - firstTerm + (( - secondTerm + thirdTerm) / denominator);
}
////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////
double myEFieldNTypefunction(double x){
  //this is explicitly only for n-type!

  //change this according to your crystal 
  double chargeCarrierDensity = 0.7e10 * 1000000; //[m^-3] 
  double Potential = -3000;//[V] defined as V = Volt(router)-Volt(rinner)
  double innerR    = 0.005; //[m]
  double outerR    = 0.0375; //[m]


  double elementaryCharge = 1.602176487e-19; //[C]
  //  double chargeCarrierDensity = 0.; //[m^-3] 
  double epsilon0 =  8.8541878176e-12; //[C^2 *N^-1*m^-2]
  double epsilonR =  16;

  double firstTerm = x * elementaryCharge * chargeCarrierDensity /(2 * epsilon0 * epsilonR ); //[ m * C * m^-3 *C^-2 * N * m^2] = [N/C] =[V/m]
  

  double denominator = x * log(outerR/innerR); //[m]

  double secondTerm = Potential; // [V/m]


  double thirdTerm = ((elementaryCharge * chargeCarrierDensity / (4 * epsilon0 *epsilonR)) * (outerR*outerR - innerR*innerR)); //[ C * m^-3 * C^-2 * N *m^2 * m^2 m^-1] = [N/C] =[V/m]


  return    firstTerm - (( secondTerm + thirdTerm) / denominator);  
}
////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////
void EFieldAnalyticPType(){
  TF1* efield = new TF1("efield","myEFieldPTypefunction(x)",0.0015,0.014);
  efield->GetYaxis()->SetTitle("EField [V/m]");
  efield->GetXaxis()->SetTitle("Radius [m]");
  efield->Draw();
}
////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////
void EFieldAnalyticNType(){
  TF1* efield = new TF1("efield","myEFieldNTypefunction(x)",0.005,0.0375);
  efield->GetYaxis()->SetTitle("EField [V/m]");
  efield->GetXaxis()->SetTitle("Radius [m]");
  efield->Draw();
} 
