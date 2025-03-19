///////////////////////////////////////////////////////////////////////////////
//                 Spectrum of radiative neutron capture by Gadolinium            
//                                    version 1.0.0                                
//                                    (Sep.09.2005)                               

//                Author : karim.zbiri@subatech.in2p3.fr                  

//This file contains the gammas spectrum produced in radiative capture of 
//neutrons by gadolinium.
//This work is adapted from earlier work in geant3 for chooz 1.

//Continuum spectrum is newly coded by Yano.
//Mod by Takatomi Yano, Dec, 2014
//First version by Karim Zbiri, April, 2005
///////////////////////////////////////////////////////////////////////////////


//#include "GdCaptureGammas.hh"
#include "GdCaptureGammas_ggarnet.hh"
#include "Randomize.hh"
#include <vector>
#include "G4Gamma.hh"

#define File_name "GdCaptureGammas_ggarnet.cc"
extern G4String Gd157_File; 
extern G4String Gd155_File; 
extern G4int Gd_CAPTURE;   
extern G4int Gd_CASCADE;    
extern std::ofstream outf;

//////////////////////////////////////
GdCaptureGammas_ggarnet::GdCaptureGammas_ggarnet ()
	//////////////////////////////////////
{
	intc();	
}

////////////////////////////////////
GdCaptureGammas_ggarnet::~GdCaptureGammas_ggarnet ()
	////////////////////////////////////
{
}

////////////////////////////////////////////////////////
G4ReactionProductVector *  GdCaptureGammas_ggarnet::GetGammas ()
	////////////////////////////////////////////////////////
{
	G4ReactionProductVector * theGammas = new G4ReactionProductVector;
	vector<double> nrj = Initialize();
	for(unsigned int i=0; i<nrj.size(); i++)
	{
//		outf<<nrj[i]<<" ";
		G4ReactionProduct *theOne = new G4ReactionProduct;
		theOne->SetDefinition(G4Gamma::Gamma());

		G4double costheta = 2.*G4UniformRand()-1;
		G4double theta = acos(costheta);
		G4double phi = twopi*G4UniformRand();
		G4double sinth = sin(theta);

		theOne->SetTotalEnergy(nrj[i]);
		G4ThreeVector temp(nrj[i]*sinth*cos(phi), nrj[i]*sinth*sin(phi),nrj[i]*cos(theta));
		theOne->SetMomentum(temp) ;
		theGammas->push_back(theOne);  
		//G4cout << "PID: Generated Gamma in GdCaptureGammas_ggarnet.cc "  << temp << G4endl;
		//G4cout << "PID: Generated Gamma in GdCaptureGammas_ggarnet.cc ,total energy: "  << theOne->GetTotalEnergy() << G4endl;
	}
//	outf<<endl;
	return theGammas;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::Initialize ()
	//////////////////////////////////////////////
{
	vector<double> Eg;
	double DefElem;
	switch(Gd_CAPTURE){
		case 1:
			DefElem = G4UniformRand()*48.764;	//14.8% (abundance of Gd155) * 60900 (cross-section of Gd155) + 15.65% (abundance of Gd157) * 254000 (cross-section of Gd157) = 4.87642e+6
			if(DefElem < 39.751){
				Eg = CapGad157(); 			//15.65% (abundance of Gd157) * 254000 (cross-section of Gd157) = 3.9751e+6
			}else{
				Eg = CapGad155();
			}
			break;
		case 2:
			Eg = CapGad157();
			break;
		case 3:
			Eg = CapGad155();
			break;
		default:
			Printing.Error(File_name,"Gd_CAPTURE");	
			break;
	}
	return Eg;
}

////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::CapGad155()
	////////////////////////////////////////////
{
	//  gammas from GAD155
	//  either 2 gammas
	//  either a continuum
	//  total energy = 8.46 MeV
	//  Elevel = 8.536;
	double Nlevel = G4UniformRand();
	double a=0;
	vector<double> Egammas;
	switch(Gd_CASCADE){
		case 1:
//			a=0.14;	//Default Continuum is 86% and Discrete is 14%
//			a=0.026;
			a=0.0278;
			break;
		case 2:
			a=1.;	//Discrete Peaks Only
			break;
		case 3:
			a=0.;	//Continuum Only
			break;
		default:
			Printing.Error(File_name,"Gd_CASCADE");
			break;
	}
	double ProbD1 = 0.0064*a;	//8.448MeV + ...
	double ProbD2 =	0.0838*a;	//7.382MeV + ...
	double ProbD3 = 0.1628*a;	//7.288MeV + ...
	double ProbD4 = 0.1266*a;	//6.474MeV + ...
	double ProbD5 = 0.1163*a;	//6.430MeV + ...
	double ProbD6 = 0.1088*a;	//6.348MeV + ...
	double ProbD7 = 0.0338*a;	//6.319MeV + ...
	double ProbD8 = 0.0733*a;	//6.034MeV + ...
	double ProbD9 = 0.0627*a;	//5.885MeV + ...
	double ProbD10= 0.0674*a;	//5.779MeV + ...
	double ProbD11= 0.1029*a;	//5.698MeV + ...
	double ProbD12= 0.0552*a;	//5.661MeV + ...
	double ProbC  = 1.-a;
//	Nlevel = Nlevel * ( ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 );

	if(Nlevel < ProbC ) Egammas=continuum155();
	else if(Nlevel < ProbC + ProbD1) Egammas=casca1Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2)  Egammas=casca2Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3)  Egammas=casca3Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4)  Egammas=casca4Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5)  Egammas=casca5Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6)  Egammas=casca6Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6 + ProbD7)  Egammas=casca7Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6 + ProbD7 + ProbD8)  Egammas=casca8Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6 + ProbD7 + ProbD8 + ProbD9)  Egammas=casca9Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6 + ProbD7 + ProbD8 + ProbD9 + ProbD10)  Egammas=casca10Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6 + ProbD7 + ProbD8 + ProbD9 + ProbD10 + ProbD11)  Egammas=casca11Gd155(); 
	else if(Nlevel < ProbC + ProbD1 + ProbD2 + ProbD3 + ProbD4 + ProbD5 + ProbD6 + ProbD7 + ProbD8 + ProbD9 + ProbD10 + ProbD11 + ProbD12)  Egammas=casca12Gd155(); 

	return Egammas;
}

////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::CapGad157()
	////////////////////////////////////////////
{
	//  gammas from GAD157
	//                   either 2 gammas
	//                   either a continuum
	//                   total energy = 7.87 MeV
	//                   either 1 gamma 

	//	Elevel = 7.87;
	//Elevel = 7.937;
	double Nlevel = G4UniformRand();
	double b=0;
	vector<double> Egammas;
	switch(Gd_CASCADE){
		case 1:
//			b=0.029015;	//Default Continuum is 97.0985% and Discrete is 2.9015% (calculated by T.Tanaka at 2017.03.03)
//			b=0.068;	//Default Continuum is 93.2% and Discrete is 6.8% (calculated by Tanaka at 2017.05.30)
			b=0.06938;
			break;
		case 2:
			b=1.;	//Discrete Peaks Only 
			break;
		case 3:
			b=0.;	//Continuum Only
			break;
		default:
			Printing.Error(File_name,"Gd_CASCADE");
			break;
	}

	double ProbD1 = 0.0008*b;	//7.937MeV+...
	double ProbD2 = 0.0034*b;	//7.857MeV+...
	double ProbD3 = 0.0030*b;	//6.960MeV+...
	double ProbD4 = 0.0182*b;	//6.914MeV+...
	double ProbD5 = 0.3476*b;	//6.750MeV+...
	double ProbD6 = 0.0274*b;	//6.672MeV+...
	double ProbD7 = 0.0470*b;	//6.420MeV+...
	double ProbD8 = 0.0227*b;	//6.006MeV+...
	double ProbD9 = 0.1694*b;	//5.903MeV+...
	double ProbD10 = 0.0285*b;	//5.784MeV+...
	double ProbD11 = 0.0913*b;	//5.677MeV+...
	double ProbD12 = 0.0961*b;	//5.610MeV+...
	double ProbD13 = 0.0343*b;	//5.543MeV+...
	double ProbD14 = 0.0234*b;	//5.436MeV+...
	double ProbD15 = 0.0869*b;	//5.179MeV+...
	double ProbC  = 1.-b;
//	Nlevel = Nlevel*(ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10+ProbD11+ProbD12+ProbD13+ProbD14+ProbD15+ProbC);

	if(Nlevel<ProbC) Egammas = continuum157();
	else if(Nlevel<ProbC+ProbD1) Egammas = casca1Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2) Egammas = casca2Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3) Egammas = casca3Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4) Egammas = casca4Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5) Egammas = casca5Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6) Egammas = casca6Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7) Egammas = casca7Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8) Egammas = casca8Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9) Egammas = casca9Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10) Egammas = casca10Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10+ProbD11) Egammas = casca11Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10+ProbD11+ProbD12) Egammas = casca12Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10+ProbD11+ProbD12+ProbD13) Egammas = casca13Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10+ProbD11+ProbD12+ProbD13+ProbD14) Egammas = casca14Gd157(); 
	else if(Nlevel<ProbC+ProbD1+ProbD2+ProbD3+ProbD4+ProbD5+ProbD6+ProbD7+ProbD8+ProbD9+ProbD10+ProbD11+ProbD12+ProbD13+ProbD14+ProbD15) Egammas = casca15Gd157(); 

	return Egammas;
}

//====================================//
///////////155Gd cascade////////////////
//====================================//
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca1Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	energy.push_back(8.448);
	energy.push_back(0.089);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca2Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	double cas155_2 = G4UniformRand();
	double cas155_21 = 0.545;
	double cas155_22 = 0.455;

	energy.push_back(7.382);
	if(cas155_2 < cas155_21){
		energy.push_back(1.154);
	}else if(cas155_2 < cas155_21+cas155_22){
		energy.push_back(1.065);
		energy.push_back(0.089);
	}
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca3Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	double cas155_3 = G4UniformRand();
	double cas155_31 = 0.768;
	double cas155_32 = 0.232;

	energy.push_back(7.288);
	if(cas155_3 < cas155_31){
		energy.push_back(1.158);
		energy.push_back(0.089);
	}else if(cas155_3 < cas155_31+cas155_32){
		energy.push_back(0.959);
		energy.push_back(0.199);
		energy.push_back(0.089);
	}
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca4Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	energy.push_back(6.474);
	energy.push_back(1.964);
	energy.push_back(0.098);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca5Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	double cas155_5 = G4UniformRand();
	double cas155_51= 0.639;
	double cas155_52= 0.361;

	energy.push_back(6.430);
	if(cas155_5 < cas155_51){
		energy.push_back(2.017);
		energy.push_back(0.089);
	}else if(cas155_5 < cas155_51+cas155_52){
		energy.push_back(1.818);
		energy.push_back(0.199);
		energy.push_back(0.089);
	}
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca6Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	double cas155_6 = G4UniformRand();
	double cas155_6_1 = G4UniformRand();
	double cas155_61= 0.399;
	double cas155_62= 0.325;
	double cas155_63= 0.276;

	double cas155_61_1= 0.545;
	double cas155_62_1= 0.455;

	energy.push_back(6.348);
	if(cas155_6 < cas155_61){
		energy.push_back(2.188);
	}else if(cas155_6 < cas155_61+cas155_62){
		energy.push_back(2.097);
		energy.push_back(0.089);
	}else if(cas155_6 < cas155_61+cas155_62+cas155_63){
		energy.push_back(1.036);
		if(cas155_6_1 < cas155_61_1){
			energy.push_back(1.154);
		}else if(cas155_6_1 < cas155_61_1+cas155_62_1){
			energy.push_back(1.065);
			energy.push_back(0.089);
		}
	}
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca7Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	energy.push_back(6.319);
	energy.push_back(2.127);
	energy.push_back(0.089);
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca8Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	double cas155_8 = G4UniformRand();
	double cas155_81= 0.686;
	double cas155_82= 0.314;

	energy.push_back(6.034);
	if(cas155_8 < cas155_81){
		energy.push_back(2.412);
		energy.push_back(0.089);
	}else if(cas155_8 < cas155_81+cas155_82){
		energy.push_back(2.213);
		energy.push_back(0.199);
		energy.push_back(0.089);
	}
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca9Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	double cas155_9 = G4UniformRand();
	double cas155_91= 0.518;
	double cas155_92= 0.482;

	energy.push_back(5.885);
	if(cas155_9 < cas155_91){
		energy.push_back(2.563);
		energy.push_back(0.089);
	}else if(cas155_9 < cas155_91+cas155_92){
		energy.push_back(2.364);
		energy.push_back(0.199);
		energy.push_back(0.089);
	}
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca10Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	energy.push_back(5.779);
	energy.push_back(2.672);
	energy.push_back(0.085);
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca11Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	energy.push_back(5.698);
	energy.push_back(2.749);
	energy.push_back(0.089);
	return energy;
}

//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca12Gd155()
	//////////////////////////////////////////////
{
	vector<double> energy;
	energy.push_back(5.661);
	energy.push_back(2.786);
	energy.push_back(0.089);
	return energy;
}

//====================================//
////////////////Gd157 cascade///////////
//====================================//
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca1Gd157()
	//////////////////////////////////////////////
{
	////7937keV (Q-value)/////
	vector<double> energy;
	energy.push_back(7.937);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca2Gd157()
	//////////////////////////////////////////////
{
	////7857keV + 80keV /////
	vector<double> energy;
	energy.push_back(7.857);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca3Gd157()
	//////////////////////////////////////////////
{
	////6960keV  /////
	vector<double> energy;
	energy.push_back(6.960);
	energy.push_back(0.977);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca4Gd157()
	//////////////////////////////////////////////
{
	////6914keV + 944keV + 80keV /////
	vector<double> energy;
	energy.push_back(6.914);
	energy.push_back(0.944);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca5Gd157()
	//////////////////////////////////////////////
{
	////6750keV + 1187 or 1107keV /////
	vector<double> energy;
	double cas157_3 = G4UniformRand();
	double cas157_31 = 0.501;
	double cas157_32 = 0.499;

	energy.push_back(6.750);
	if(cas157_3 < cas157_31){
		energy.push_back(1.187);
	}else if(cas157_3 < cas157_31+cas157_32){
		energy.push_back(1.107);
		energy.push_back(0.080);
	}
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca6Gd157()
	//////////////////////////////////////////////
{
	////6672keV + .../////
	vector<double> energy;
	double cas157_4 = G4UniformRand();
	double cas157_41 = 0.847;
	double cas157_42 = 0.153;

	energy.push_back(6.672);
	if(cas157_4 < cas157_41){
		energy.push_back(1.186);
		energy.push_back(0.080);
	}else if(cas157_4 < cas157_41+cas157_42){
		energy.push_back(1.004);
		energy.push_back(0.182);
		energy.push_back(0.080);
	}
	return energy;
}
///////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca7Gd157()
	//////////////////////////////////////////////
{
	////6420keV + .... /////
	vector<double> energy;
	double cas157_5 = G4UniformRand();
	double cas157_51 = 0.381;
	double cas157_52 = 0.416;
	double cas157_53 = 0.203;

	energy.push_back(6.420);
	if(cas157_5 < cas157_51){
		energy.push_back(1.517);
	}else if(cas157_5 < cas157_51+cas157_52){
		energy.push_back(1.438);
		energy.push_back(0.080);
	}else if(cas157_5 < cas157_51+cas157_52+cas157_53){
		energy.push_back(1.256);
		energy.push_back(0.182);
		energy.push_back(0.080);
	}
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca8Gd157()
	//////////////////////////////////////////////
{
	////6000.5keV (6006,5995 double peaks)/////
	vector<double> energy;
	double cas157_6 = G4UniformRand();
	double cas157_61 = 0.501;
	double cas157_62 = 0.499;

	energy.push_back(6.001);
	energy.push_back(0.749);
	if(cas157_6 < cas157_61){
		energy.push_back(1.187);
	}else if(cas157_6 < cas157_61+cas157_62){
		energy.push_back(1.107);
		energy.push_back(0.080);
	}
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca9Gd157()
	//////////////////////////////////////////////
{
	////5903keV + .... /////
	vector<double> energy;
	double cas157_7 = G4UniformRand();
	double cas157_7_1 = G4UniformRand();
	double cas157_7_2 = G4UniformRand();
	double cas157_71 = 0.393;
	double cas157_72 = 0.254;
	double cas157_73 = 0.222;
	double cas157_74 = 0.131;

	double cas157_73_1 = 0.847;
	double cas157_73_2 = 0.153;
	double cas157_74_1 = 0.755;	//value from Ensdf
	double cas157_74_2 = 0.245;	//value from Ensdf

	energy.push_back(5.903);
	if(cas157_7 < cas157_71){
		energy.push_back(1.010);
		energy.push_back(0.944);
		energy.push_back(0.080);
	}else if(cas157_7 < cas157_71+cas157_72){
		energy.push_back(0.875);
		energy.push_back(0.898);
		energy.push_back(0.182);
		energy.push_back(0.080);
	}else if(cas157_7 < cas157_71+cas157_72+cas157_73){
		energy.push_back(0.769);
		if(cas157_7_1 < cas157_73_1){
			energy.push_back(1.186);
			energy.push_back(0.080);
		}else if(cas157_7_1 < cas157_73_1+cas157_73_2){
			energy.push_back(1.004);
			energy.push_back(0.182);
			energy.push_back(0.080);
		}
	}else if(cas157_7 < cas157_71+cas157_72+cas157_73+cas157_74){
		energy.push_back(0.676);
		if(cas157_7_2 < cas157_74_1){
			energy.push_back(1.097);
			energy.push_back(0.182);
			energy.push_back(0.080);
		}else if(cas157_7_2 < cas157_74_1+cas157_74_2){
			energy.push_back(1.279);
			energy.push_back(0.080);
		}
	}
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca10Gd157()
	//////////////////////////////////////////////
{
	////5784keV /////
	vector<double> energy;
	energy.push_back(5.784);
	energy.push_back(2.073);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca11Gd157()
	//////////////////////////////////////////////
{
	////5669keV (5677,5661 double peaks)/////
	vector<double> energy;
	energy.push_back(5.669);
	energy.push_back(2.188);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca12Gd157()
	//////////////////////////////////////////////
{
	////5595keV (5610,5593,5582 triple peaks)/////
	vector<double> energy;
	energy.push_back(5.595);
	energy.push_back(2.262);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca13Gd157()
	//////////////////////////////////////////////
{
	////5543keV /////
	vector<double> energy;
	energy.push_back(5.543);
	energy.push_back(2.314);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca14Gd157()
	//////////////////////////////////////////////
{
	////5436keV /////
	vector<double> energy;
	energy.push_back(5.436);
	energy.push_back(2.421);
	energy.push_back(0.080);
	return energy;
}
//////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::casca15Gd157()
	//////////////////////////////////////////////
{
	////5167keV (5179,5155 double peaks)/////
	vector<double> energy;
	energy.push_back(5.167);
	energy.push_back(2.690);
	energy.push_back(0.080);
	return energy;
}

///////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::continuum155()
	///////////////////////////////////////////////
{
	// continuum part of gadolinium
	//cross sections
	vector<double> energy;

	double BindingEnergy = 8.54; // MeV, for 155Gd  Fixed(2017.9.21)
	int nbin = 200; // constant for reading table
	double E_max = BindingEnergy; //constant for reading table, maximum value of E

	double E_res = BindingEnergy-0.0001;
	double E_gam = 0;
	int ithbin = 0;
	//	int maxbin=0;
	int ybin = 0;
	int ngam = 0;
	double random = 0;

	while(E_res>0.20){
		random = G4UniformRand();
		E_gam = random*E_res;
		ybin = (int)((BindingEnergy-E_res)/E_max*nbin);
		ithbin = (int)(E_gam/E_max*nbin);

		random=G4UniformRand();
		if (kopecky155_int[ithbin][ybin]>random){
			energy.push_back(E_gam);
			E_res=E_res-E_gam;
			ngam=ngam+1;
		}
	}
	energy.push_back(E_res);
	return energy;
}

///////////////////////////////////////////////
vector<double>  GdCaptureGammas_ggarnet::continuum157()
	///////////////////////////////////////////////
{
	// continuum part of gadolinium
	//cross sections
	vector<double> energy;

	double BindingEnergy = 7.937; // MeV, for 157Gd  Fixed(2017.9.21)
	int nbin = 200; // constant for reading table
	double E_max = BindingEnergy; //constant for reading table, maximum value of E

	double E_res = BindingEnergy-0.0001;
	double E_gam = 0;
	int ithbin = 0;
	//	int maxbin=0;
	int ybin = 0;
	int ngam = 0;
	double random = 0;

	double E182 = G4UniformRand();
	double add182 = 0.153;

	while(E_res>0.20){
		random = G4UniformRand();
		E_gam = random*E_res;
		ybin = (int)((BindingEnergy-E_res)/E_max*nbin);
		ithbin = (int)(E_gam/E_max*nbin);

		random = G4UniformRand();
		if (kopecky157_int[ithbin][ybin]>random){
			energy.push_back(E_gam);
			E_res = E_res-E_gam;
			ngam = ngam+1;
		}
	}

	if(E182<add182&&E_res>0.182){	//Fixed(2017.9.21)
		energy.push_back(0.182);
		E_res = E_res-0.182;
		energy.push_back(E_res);
	}else{
		energy.push_back(E_res);
	}
	return energy;
}

/////////////////////////////
void  GdCaptureGammas_ggarnet::intc()
	/////////////////////////////
{
	//   Prepare a table and kopecky_int(200,200)
	// i : E_res-E_gam
	// j : E_gam

	//	ifstream ifs155("Gd155.dat");
	//	ifstream ifs157("Gd157_Simple.dat");

	ifstream ifs155(Gd155_File.c_str());
	ifstream ifs157(Gd157_File.c_str());

	int nbin=200;
	double prob_int;
	for (int i=0;i<nbin;i++){
		for (int j=0;j<nbin;j++){
			if(ifs155>>prob_int){
				kopecky155_int[i][j]=prob_int;
			}else {
				printf("error while reading Gd155.dat");
			}

			if(ifs157>>prob_int){
				kopecky157_int[i][j]=prob_int;
			}else {
				printf("error while reading Gd157.dat");
			}
		}
	}
}
