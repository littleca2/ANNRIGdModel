// ====================================================================
//    PrimaryGeneratorAction.hh
// ====================================================================

#ifndef PRIMARY_GENERATOR_ACTION_H
#define PRIMARY_GENERATOR_ACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"		//Added by Yano. 20150901 No.4
#include "DrawMessage.hh"

class G4ParticleGun;

// ====================================================================
// class definition
// ====================================================================

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
	private:
		////for G4ParticleGun ////
		G4ParticleGun* particleGun;	//original No.3
		////for GPS ////
		G4GeneralParticleSource* particleGPS;	//use GPS Added by Yano. 20150901 No.5
	public:
		PrimaryGeneratorAction();
		~PrimaryGeneratorAction();

		virtual void GeneratePrimaries(G4Event* anEvent);
		
		DrawMessage Printing;
};

#endif
