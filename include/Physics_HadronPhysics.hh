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
#ifndef Physics_HadronPhysics_h
#define Physics_HadronPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"
#include "DrawMessage.hh"

class Physics_HadronPhysics : public G4VPhysicsConstructor
{
	public: 
		Physics_HadronPhysics(const G4String& name = "hadron");
		virtual ~Physics_HadronPhysics();

	public: 
		// This method will be invoked in the Construct() method. 
		// each particle type will be instantiated
		virtual void ConstructParticle();

		// This method will be invoked in the Construct() method.
		// each physics process will be instantiated and
		// registered to the process manager of each particle type 
		virtual void ConstructProcess();

		DrawMessage Printing;

	protected:

};


#endif








