

///////////////////////////////////////////////////////////////////////////////
//                    Spectrum of radiative neutron capture by Gadolinium            
//                                    version 1.0.0                                
//                                    (Sep.09.2005)                               

//                Author : karim.zbiri@subatech.in2p3.fr                  

//Modified class from original G4NeutronHPCaptureFS class to deexcite and
//add correctly the secondary to the hadronic final state

// Karim Zbiri, Aug, 2005
///////////////////////////////////////////////////////////////////////////////



#ifndef GdNeutronHPCaptureFS_h
#define GdNeutronHPCaptureFS_h 1

#include "globals.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"
#include "G4NeutronHPFinalState.hh"
#include "G4ReactionProductVector.hh"
#include "G4NeutronHPNames.hh"
#include "G4NeutronHPPhotonDist.hh"
#include "G4Nucleus.hh"
#include "G4Fragment.hh"

#include "GdCaptureGammas_ggarnet.hh"
#include "GdCaptureGammas_glg4sim.hh"
#include "DrawMessage.hh"

// Forward declaration
namespace ANNRIGdGammaSpecModel { class ANNRIGd_GdNCaptureGammaGenerator; }
namespace AGd = ANNRIGdGammaSpecModel;

class GdNeutronHPCaptureFS : public G4NeutronHPFinalState
{
	public:

		GdNeutronHPCaptureFS();
		~GdNeutronHPCaptureFS();

		void   UpdateNucleus( const G4Fragment* , G4double );
		void Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & aFSType);
		G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
		G4NeutronHPFinalState * New() 
		{
			GdNeutronHPCaptureFS * theNew = new GdNeutronHPCaptureFS;
			return theNew;
		}

	private:

		void InitANNRIGdGenerator();
		G4ReactionProductVector* GenerateWithANNRIGdGenerator();

		G4Fragment * nucleus;

		G4DynamicParticle * theTwo ;
		G4ReactionProduct theTarget; 
		G4Nucleus aNucleus;
		G4ReactionProduct theNeutron;

		G4double targetMass;

		G4NeutronHPPhotonDist theFinalStatePhotons;
		G4NeutronHPNames theNames;
		
		GdCaptureGammas_ggarnet theFinalgammas_ggarnet;
		GdCaptureGammas_glg4sim theFinalgammas_glg4sim;

		G4double theCurrentA;
		G4double theCurrentZ;

		DrawMessage Printing;

	private:

		static AGd::ANNRIGd_GdNCaptureGammaGenerator* sAnnriGammaGen;
};
#endif
