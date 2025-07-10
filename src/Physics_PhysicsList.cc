//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "Physics_PhysicsList.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4FastSimulationManagerProcess.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>   

//#include "Physics_GeneralPhysics.hh"
//#include "Physics_EMPhysics.hh"
//#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
//#include "G4EmStandardPhysics_option4.hh"
//#include "Physics_MuonPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "Physics_HadronPhysics.hh"
//#include "Physics_RadioactiveDecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

#include "G4SystemOfUnits.hh"

//Juan David Cortes, July 2, 2025, I'm doing this to see if IonTable can finally get Gd
#include "G4IonPhysics.hh"
#include "G4IonTable.hh"
#include "G4NuclideTable.hh"

///////////////////////////////////////////////////////////////////
Physics_PhysicsList::Physics_PhysicsList():  G4VModularPhysicsList()
///////////////////////////////////////////////////////////////////
{

	// default cut value  (1.0mm) 
	 //defaultCutValue = 0.1*mm;
		defaultCutValue = 1.*mm;
	//  SetVerboseLevel(1);

	// General Physics
	// RegisterPhysics( new Physics_GeneralPhysics("general") );

	// EM Physics
	//RegisterPhysics( new Physics_EMPhysics("standard EM"));
	//RegisterPhysics( new G4EmLivermorePhysics(1,"Libermore EM"));
	//RegisterPhysics( new G4EmStandardPhysics_option4(1,"Option4"));
	RegisterPhysics( new G4EmPenelopePhysics(1,"Penelope"));

	// Muon Physics
	//RegisterPhysics( new Physics_MuonPhysics("muon"));

	// Optical Physics
	RegisterPhysics( new G4OpticalPhysics(0,"optical"));

	// Hadron Physics
	RegisterPhysics( new Physics_HadronPhysics("hadron"));

	// RadioactiveDecay Physics
	//RegisterPhysics( new Physics_RadioactiveDecayPhysics("radioactive decay"));
	RegisterPhysics( new G4RadioactiveDecayPhysics("radioactive decay"));
	RegisterPhysics( new G4DecayPhysics("decay"));

	//Juan David Cortes, July 2, 2025, I'm doing this to see if IonTable can finally get Gd
	//RegisterPhysics(new G4IonPhysics()); 

}

///////////////////////////////////////////////////////////////////
Physics_PhysicsList::~Physics_PhysicsList()
///////////////////////////////////////////////////////////////////
{
}

// Juan David Cortes, July 2, 2025, Override of ConstructParticle() in order to, hopefully, get the IonTables to have GD
void Physics_PhysicsList::ConstructParticle()
{

	G4VModularPhysicsList::ConstructParticle();
	G4GenericIon::GenericIonDefinition();
	G4IonTable::GetIonTable()->CreateAllIon();

	auto* nuclideTab = G4NuclideTable::GetNuclideTable();
	nuclideTab->SetThresholdOfHalfLife(0.0);
	nuclideTab->GenerateNuclide();
	auto* ionTab = G4IonTable::GetIonTable();
	size_t nIso = nuclideTab->entries();
	for ( size_t i = 0; i < nIso; ++i ) {
		auto* prop = nuclideTab->GetIsotopeByIndex(i);
		ionTab->GetIon(prop->GetAtomicNumber(),
				prop->GetAtomicMass(),
				prop->GetIsomerLevel());
	}

}

///////////////////////////////////////////////////////////////////
void Physics_PhysicsList::SetCuts()
///////////////////////////////////////////////////////////////////
{
	//  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
	//   the default cut value for all particle types 
	SetCutsWithDefault();   
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1*GeV);
}
