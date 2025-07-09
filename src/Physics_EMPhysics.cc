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
#include "Physics_EMPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   


Physics_EMPhysics::Physics_EMPhysics(const G4String& name)
               :  G4VPhysicsConstructor(name)
{
}

Physics_EMPhysics::~Physics_EMPhysics()
{
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"


// Juan David Cortes, June 27, 2025, might actually be necessary to declare this here for theParticleIterator to be properly defined
#include "G4VPhysicsConstructor.hh"

////////////////////////////////////////////
void Physics_EMPhysics::ConstructParticle()
//////////////////////////////////////////
{  // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}


#include "G4ProcessManager.hh"

//////////////////////////////////////////
void Physics_EMPhysics::ConstructProcess()
//////////////////////////////////////////
{

  bool omit_muon_processes = 0;

  // Juan David Corte, June 27, 2025, This might be able to solve the issue with theParticleIterator (hopefully)
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4String particleType = particle->GetParticleType();     
    G4double charge = particle->GetPDGCharge();

    if (particleName == "gamma") {
      // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
      //electron
      // Construct processes for electron
      pmanager->AddProcess(new G4eMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4eIonisation(),-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),-1,-1,3);

    } else if (particleName == "e+") {
      //positron
      // Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4eIonisation(),-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),-1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation(),0,-1,4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
      //muon
      if ( omit_muon_processes != 0.0 ) {
        G4cerr << "Warning, muon processes omitted.\n";
      }
      else {
        // Construct processes for muon
        pmanager->AddProcess(new G4MuMultipleScattering(),-1,1,1);
        pmanager->AddProcess(new G4MuIonisation(),-1,2,2); 
	pmanager->AddProcess(new G4MuIonisation(),-1,2,2);
        pmanager->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
        pmanager->AddProcess(new G4MuPairProduction(),-1,-1,4);
      }
      // below from DMX example...
    } else if (particleName == "proton"     ||
	       particleName == "alpha"      || 
	       particleName == "deuteron"   ||
	       particleName == "triton"     || 
	       particleName == "He3"        ||
	       particleName == "GenericIon" || 
	       (particleType == "nucleus" && charge != 0) ){         
      // OBJECT may be dynamically created as either a GenericIon or nucleus
      // G4Nucleus exists and therefore has particle type nucleus         
      // genericIon:
      G4hMultipleScattering* aMultipleScattering = new G4hMultipleScattering();         
      //hIonisation G4hLowEnergyIonisation* ahadronLowEIon = new G4hLowEnergy\Ionisation(); 
      G4hIonisation* ahadronIon = new G4hIonisation();
      pmanager->AddProcess(aMultipleScattering,-1,1,1);         
      //hIonisation   pmanager->AddProcess(ahadronLowEIon,-1,2,2);
      pmanager->AddProcess(ahadronIon,-1,2,2);         
      // ahadronLowEIon->SetNuclearStoppingOff() ;
      //        ahadronLowEIon->SetNuclearStoppingPowerModel("ICRU_R49") ;         
      //        ahadronLowEIon->SetNuclearStoppingOn() ;
      //fluorescence switch off for hadrons (for now) PIXE:
      //hIonisation        ahadronLowEIon->SetFluorescence(false); 
    } else if ((!particle->IsShortLived()) &&
	       (charge != 0.0) &&              
	       (particle->GetParticleName() != "chargedgeantino")){  
      //all others charged particles except geantino
      G4hMultipleScattering* aMultipleScattering = new G4hMultipleScattering();         
      //hIonisation        G4hLowEnergyIonisation* ahadronLowEIon = new G4hLo\wEnergyIonisation();
      G4hIonisation* ahadronIon = new G4hIonisation();
      pmanager->AddProcess(aMultipleScattering,-1,1,1); 
      //hIonisation   pmanager->AddProcess(ahadronLowEIon,       -1,2,2);    
      pmanager->AddProcess(ahadronIon,       -1,2,2);
      //      pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
    }
  }
}

