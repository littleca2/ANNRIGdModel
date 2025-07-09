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
#include "Physics_RadioactiveDecayPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   

/////////////////////////////////////////////////////////////////////////////////////
Physics_RadioactiveDecayPhysics::Physics_RadioactiveDecayPhysics(const G4String& name)
                   :  G4VPhysicsConstructor(name)
/////////////////////////////////////////////////////////////////////////////////////
{
}

///////////////////////////////////////////////////////////////////
Physics_RadioactiveDecayPhysics::~Physics_RadioactiveDecayPhysics()
///////////////////////////////////////////////////////////////////
{
}

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Decay.hh"
#include "G4RadioactiveDecayPhysics.hh"

///////////////////////////////////////////////////////////////////
void Physics_RadioactiveDecayPhysics::ConstructParticle()
///////////////////////////////////////////////////////////////////
{
  //  G4Alpha::AlphaDefinition();
  //  G4Deuteron::DeuteronDefinition();
  //  G4GenericIon::GenericIonDefinition();
  //  G4He3::He3Definition();
  //  G4Triton::TritonDefinition();

}

#include "G4ProcessManager.hh"

///////////////////////////////////////////////////////////////////
void Physics_RadioactiveDecayPhysics::ConstructProcess()
///////////////////////////////////////////////////////////////////
{
  G4RadioactiveDecayPhysics* theRadioactiveDecayProcess = new G4RadioactiveDecayPhysics(); // -0316
  theRadioactiveDecayProcess->ConstructProcess(); // -0316
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();  

  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();

  theParticleIterator->reset();

  while( (*theParticleIterator)() ){     
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();     
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);       // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    } 
  }
}
