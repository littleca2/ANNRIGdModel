// ====================================================================
//   PrimaryGeneratorAction.cc
// ====================================================================
#include "G4GeneralParticleSource.hh" // Added By Yano. 20150901 No.1
#include "G4ParticleGun.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "PrimaryGeneratorAction.hh"

#define File_name "PrimaryGeneratorAction.cc"
extern G4int PARTICLE;	
extern G4double ENERGY_1; 	 
extern G4double ENERGY_2; 	
extern G4double EMIT_POSX;
extern G4double EMIT_POSY; 
extern G4double EMIT_POSZ; 

using namespace CLHEP;
// ====================================================================
// class description
// ====================================================================

////////////////////////////////////////////////
PrimaryGeneratorAction::PrimaryGeneratorAction()
	////////////////////////////////////////////////
{	
	G4String particle_ver;
	G4String emit_gamma;

	switch(PARTICLE){
		case 11:
		case 12:
			////for G4ParticleGun ////
			particleGun = new G4ParticleGun;	//original No.1
			particle_ver = "G4ParticleGun";
			if(PARTICLE==11) emit_gamma = "1 gamma emission";
			if(PARTICLE==12) emit_gamma = "2 gamma emission";
			break;

		case 2:
			////for GPS ////
			particleGPS = new G4GeneralParticleSource(); // Added By Yano. 20150901 No.2
			particle_ver = "G4GeneralParticleSource(GPS)";
			emit_gamma = "";
			break;
		default:
			Printing.Error(File_name,"PARTICLE");
			break;
	}
}
/////////////////////////////////////////////////
PrimaryGeneratorAction::~PrimaryGeneratorAction()
	/////////////////////////////////////////////////
{	
	switch(PARTICLE){
		case 11:
		case 12:
			delete particleGun;
			break;
		case 2:
			delete particleGPS;
			break;
	}
}

////////////////////////////////////////////////////////////////
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
	////////////////////////////////////////////////////////////////
{
	G4ThreeVector pos;
	G4ThreeVector mom1,mom2;

	
	switch(PARTICLE){
		case 11:
			//G4ParticleGun : 1 gamma emission//	
			pos = G4ThreeVector(EMIT_POSX*mm,EMIT_POSY*mm,EMIT_POSZ*mm);
			mom1 = G4RandomDirection();

			particleGun->SetParticlePosition(pos);
			particleGun->SetParticleMomentumDirection(mom1);
			particleGun->GeneratePrimaryVertex(anEvent);
			break;

		case 12:
			//G4ParticleGun : 2 gamma emission//	
			pos = G4ThreeVector(EMIT_POSX*mm,EMIT_POSY*mm,EMIT_POSZ*mm);
			mom1 = G4RandomDirection();
			mom2 = G4RandomDirection();

			particleGun->SetParticleEnergy(ENERGY_1*MeV);
			particleGun->SetParticlePosition(pos);
			particleGun->SetParticleMomentumDirection(mom1);
			particleGun->GeneratePrimaryVertex(anEvent);

			particleGun->SetParticleEnergy(ENERGY_2*MeV);
			particleGun->SetParticlePosition(pos);
			particleGun->SetParticleMomentumDirection(mom2);
			particleGun->GeneratePrimaryVertex(anEvent);
			break;

		case 2:
			//for GPS sources //
			particleGPS->GeneratePrimaryVertex(anEvent); // Added By Yano. 20150901 No.3
			break;
	}

}

