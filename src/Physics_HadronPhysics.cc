
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
// 130505 : Moved to G4NeutronHPorLElastic or G4NeutronHPorLEOthers. T.Yano.
#include "Physics_HadronPhysics.hh"
#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   

#define File_name "Physics_HadronPhysics.cc"
extern G4int MODEL; 


//////////////////////////////////////////////////////////////////
	Physics_HadronPhysics::Physics_HadronPhysics(const G4String& name)
:  G4VPhysicsConstructor(name)
	//////////////////////////////////////////////////////////////////
{
}

///////////////////////////////////////////////
Physics_HadronPhysics::~Physics_HadronPhysics()
	///////////////////////////////////////////////
{
}

#include "G4HadronCaptureProcess.hh" 

// Elastic processes: 
#include "G4HadronElasticProcess.hh"

// Inelastic processes:
#include "G4PionPlusInelasticProcess.hh" 
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh" 
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh" 
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh" 
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh" 
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh" 
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh" 

// Low-energy Models: < 20GeV
#include "G4LElastic.hh" 
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh" 
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh" 
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh" 
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh" 
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh" 
#include "G4LEDeuteronInelastic.hh" 
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh" 

// High-energy Models: >20 GeV 
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh" 
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh" 
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh" 
#include "G4HEProtonInelastic.hh" 
#include "G4HEAntiProtonInelastic.hh"
#include "G4HENeutronInelastic.hh" 
#include "G4HEAntiNeutronInelastic.hh"

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPorLElastic.hh" 
#include "G4NeutronHPElasticData.hh"
#include "GdNeutronHPCapture.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPorLEInelastic.hh" 
#include "G4NeutronHPInelasticData.hh" 
#include "G4LCapture.hh"

// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh" 
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh" 
#include "G4AntiNeutronAnnihilationAtRest.hh"

#include "G4ProcessManager.hh"

///////////////////////////////////////////////
void Physics_HadronPhysics::ConstructParticle()
	///////////////////////////////////////////////
{
	// mesons
	G4PionPlus::PionPlusDefinition();
	G4PionMinus::PionMinusDefinition();
	G4PionZero::PionZeroDefinition();
	G4Eta::EtaDefinition();
	G4EtaPrime::EtaPrimeDefinition();
	// G4RhoZero::RhoZeroDefinition();
	G4KaonPlus::KaonPlusDefinition();
	G4KaonMinus::KaonMinusDefinition();
	G4KaonZero::KaonZeroDefinition();
	G4AntiKaonZero::AntiKaonZeroDefinition();
	G4KaonZeroLong::KaonZeroLongDefinition();
	G4KaonZeroShort::KaonZeroShortDefinition();

	G4AntiLambda::AntiLambdaDefinition();
	G4AntiLambdacPlus::AntiLambdacPlusDefinition();
	G4AntiNeutron::AntiNeutronDefinition();
	G4AntiOmegaMinus::AntiOmegaMinusDefinition();
	G4AntiOmegacZero::AntiOmegacZeroDefinition();
	G4AntiProton::AntiProtonDefinition();
	G4AntiSigmaMinus::AntiSigmaMinusDefinition();
	G4AntiSigmaPlus::AntiSigmaPlusDefinition();
	G4AntiSigmaZero::AntiSigmaZeroDefinition();
	G4AntiSigmacPlus::AntiSigmacPlusDefinition();
	G4AntiSigmacPlusPlus::AntiSigmacPlusPlusDefinition();
	G4AntiSigmacZero::AntiSigmacZeroDefinition();
	G4AntiXiMinus::AntiXiMinusDefinition();
	G4AntiXiZero::AntiXiZeroDefinition();
	G4AntiXicPlus::AntiXicPlusDefinition();
	G4AntiXicZero::AntiXicZeroDefinition();
	//  G4BaryonConstructor::BaryonConstructorDefinition();
	G4Lambda::LambdaDefinition();
	G4LambdacPlus::LambdacPlusDefinition();
	G4Neutron::NeutronDefinition();
	G4OmegaMinus::OmegaMinusDefinition();
	G4OmegacZero::OmegacZeroDefinition();
	G4Proton::ProtonDefinition();
	G4SigmaMinus::SigmaMinusDefinition();
	G4SigmaPlus::SigmaPlusDefinition();
	G4SigmaZero::SigmaZeroDefinition();
	G4SigmacPlus::SigmacPlusDefinition();
	G4SigmacPlusPlus::SigmacPlusPlusDefinition();
	G4SigmacZero::SigmacZeroDefinition();
	G4XiMinus::XiMinusDefinition();
	G4XiZero::XiZeroDefinition();
	G4XicPlus::XicPlusDefinition();
	G4XicZero::XicZeroDefinition();

	// ions
	//  const G4IonTable *theIonTable =
	//    G4ParticleTable::GetParticleTable()->GetIonTable(); // make sure it exists113a121

	G4Alpha::AlphaDefinition();
	G4Deuteron::DeuteronDefinition();
	G4GenericIon::GenericIonDefinition();
	G4He3::He3Definition();
	G4Triton::TritonDefinition();

}

///////////////////////////////////////////////
// taken from DMX example...
void Physics_HadronPhysics::ConstructProcess()
	///////////////////////////////////////////////
{
	G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess;
	G4LElastic* theElasticModel = new G4LElastic;   
	theElasticProcess->RegisterMe(theElasticModel);
	theParticleIterator->reset();
	while ((*theParticleIterator)()) {
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName(); 

		if (particleName == "pi+") {
			pmanager->AddDiscreteProcess(theElasticProcess);           
			G4PionPlusInelasticProcess* theInelasticProcess =
				new G4PionPlusInelasticProcess("inelastic"); 
			G4LEPionPlusInelastic* theLEInelasticModel = 
				new G4LEPionPlusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEPionPlusInelastic* theHEInelasticModel =
				new G4HEPionPlusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess); 
		}

		else if (particleName == "pi-") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4PionMinusInelasticProcess* theInelasticProcess =
				new G4PionMinusInelasticProcess("inelastic");    
			G4LEPionMinusInelastic* theLEInelasticModel =
				new G4LEPionMinusInelastic;           
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEPionMinusInelastic* theHEInelasticModel = 
				new G4HEPionMinusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
			G4String prcNam; 
			pmanager->AddRestProcess(new G4PiMinusAbsorptionAtRest, ordDefault);
		} 

		else if (particleName == "kaon+") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonPlusInelasticProcess* theInelasticProcess =
				new G4KaonPlusInelasticProcess("inelastic");
			G4LEKaonPlusInelastic* theLEInelasticModel =
				new G4LEKaonPlusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonPlusInelastic* theHEInelasticModel = 
				new G4HEKaonPlusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel); 
			pmanager->AddDiscreteProcess(theInelasticProcess);
		} 

		else if (particleName == "kaon0S") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonZeroSInelasticProcess* theInelasticProcess =  
				new G4KaonZeroSInelasticProcess("inelastic");
			G4LEKaonZeroSInelastic* theLEInelasticModel =
				new G4LEKaonZeroSInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);    
			G4HEKaonZeroInelastic* theHEInelasticModel =
				new G4HEKaonZeroInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel); 
			pmanager->AddDiscreteProcess(theInelasticProcess);
		} 

		else if (particleName == "kaon0L") {
			pmanager->AddDiscreteProcess(theElasticProcess); 
			G4KaonZeroLInelasticProcess* theInelasticProcess =
				new G4KaonZeroLInelasticProcess("inelastic");
			G4LEKaonZeroLInelastic* theLEInelasticModel =
				new G4LEKaonZeroLInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonZeroInelastic* theHEInelasticModel =
				new G4HEKaonZeroInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		} 

		else if (particleName == "kaon-") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonMinusInelasticProcess* theInelasticProcess =
				new G4KaonMinusInelasticProcess("inelastic");
			G4LEKaonMinusInelastic* theLEInelasticModel =
				new G4LEKaonMinusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonMinusInelastic* theHEInelasticModel =
				new G4HEKaonMinusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel); 
			pmanager->AddDiscreteProcess(theInelasticProcess);
			pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest, ordDefault);
		}

		else if (particleName == "proton") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4ProtonInelasticProcess* theInelasticProcess =
				new G4ProtonInelasticProcess("inelastic");
			G4LEProtonInelastic* theLEInelasticModel = new G4LEProtonInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEProtonInelastic* theHEInelasticModel = new G4HEProtonInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "anti_proton") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AntiProtonInelasticProcess* theInelasticProcess =
				new G4AntiProtonInelasticProcess("inelastic");
			G4LEAntiProtonInelastic* theLEInelasticModel = 
				new G4LEAntiProtonInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEAntiProtonInelastic* theHEInelasticModel =
				new G4HEAntiProtonInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel); 
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "neutron") {
			// elastic scattering
			G4HadronElasticProcess* theNeutronElasticProcess =
				new G4HadronElasticProcess;
			G4NeutronHPorLElastic * theElasticNeutron = new G4NeutronHPorLElastic();
			theNeutronElasticProcess->RegisterMe(theElasticNeutron);
			G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData();
			theNeutronElasticProcess->AddDataSet(theNeutronData);
			pmanager->AddDiscreteProcess(theNeutronElasticProcess);

			// inelastic scattering
			G4NeutronInelasticProcess* theInelasticProcess = 
				new G4NeutronInelasticProcess("inelastic");
			G4NeutronHPorLEInelastic* theInelasticModel = new G4NeutronHPorLEInelastic();
			theInelasticProcess->RegisterMe(theInelasticModel);
			G4NeutronHPInelasticData * theNeutronData1 =
				new G4NeutronHPInelasticData;
			theInelasticProcess->AddDataSet(theNeutronData1);
			pmanager->AddDiscreteProcess(theInelasticProcess);

			// capture
			G4HadronCaptureProcess* theCaptureProcess = new G4HadronCaptureProcess;

			GdNeutronHPCapture *GdtheLENeutronCaptureModel;
			G4NeutronHPCapture *G4theLENeutronCaptureModel;
			switch(MODEL){
				case 1:
				case 2:
				case 4:
					GdtheLENeutronCaptureModel = new GdNeutronHPCapture;
					theCaptureProcess->RegisterMe(GdtheLENeutronCaptureModel);
					break;
				case 3:
					G4theLENeutronCaptureModel = new G4NeutronHPCapture;
					theCaptureProcess->RegisterMe(G4theLENeutronCaptureModel);
					break;
				default:
					Printing.Error(File_name,"MODEL");
					break;
			}

			G4NeutronHPCaptureData * theNeutronData3 = new G4NeutronHPCaptureData;
			theCaptureProcess->AddDataSet(theNeutronData3);
			pmanager->AddDiscreteProcess(theCaptureProcess);
			//  G4ProcessManager* pmanager = G4Neutron::Neutron->GetProcessManager();
			//  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
		}

		else if (particleName == "anti_neutron") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AntiNeutronInelasticProcess* theInelasticProcess =
				new G4AntiNeutronInelasticProcess("inelastic");
			G4LEAntiNeutronInelastic* theLEInelasticModel =
				new G4LEAntiNeutronInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEAntiNeutronInelastic* theHEInelasticModel =
				new G4HEAntiNeutronInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "deuteron") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4DeuteronInelasticProcess* theInelasticProcess =
				new G4DeuteronInelasticProcess("inelastic");
			G4LEDeuteronInelastic* theLEInelasticModel =
				new G4LEDeuteronInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "triton") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4TritonInelasticProcess* theInelasticProcess =
				new G4TritonInelasticProcess("inelastic");
			G4LETritonInelastic* theLEInelasticModel =
				new G4LETritonInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "alpha") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AlphaInelasticProcess* theInelasticProcess =
				new G4AlphaInelasticProcess("inelastic");
			G4LEAlphaInelastic* theLEInelasticModel =
				new G4LEAlphaInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}
	}
}


/*
   void Physics_HadronPhysics::ConstructProcess()
   {

   bool omit_neutron_hp = 0;
   theParticleIterator->reset();
   while( (*theParticleIterator)() ){
   G4ParticleDefinition* particle = theParticleIterator->value();
   G4ProcessManager* pmanager = particle->GetProcessManager();

   if ( particle == G4AntiNeutron::AntiNeutron() ) {
   pmanager->AddRestProcess(new G4AntiNeutronAnnihilationAtRest());
   }

   if ( particle == G4AntiProton::AntiProton() ) {
   pmanager->AddRestProcess(new G4AntiProtonAnnihilationAtRest());
   }

   if ( particle == G4KaonMinus::KaonMinus() ) {
   pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest());
   }

   if ( particle == G4PionMinus::PionMinus() ) {
   pmanager->AddRestProcess(new G4PionMinusAbsorptionAtRest());
   }

   if ( particle == G4MuonMinus::MuonMinus() ) {
   pmanager->AddRestProcess(new G4MuonMinusCaptureAtRest());
   }

   if ( particle == G4Deuteron::Deuteron() ) {
   G4HadronElasticProcess*   theHadronElasticProcess
   = new   G4HadronElasticProcess();

   pmanager->AddDiscreteProcess(theHadronElasticProcess);
   G4DeuteronInelasticProcess* theInelasticProcess =
   new G4DeuteronInelasticProcess("inelastic");
   G4LEDeuteronInelastic* theLEInelasticModel =
   new G4LEDeuteronInelastic;
   theInelasticProcess->RegisterMe(theLEInelasticModel);
   pmanager->AddDiscreteProcess(theInelasticProcess);
   }

   if ( particle == G4Neutron::Neutron() ) {

   G4HadronElasticProcess*   theHadronElasticProcess
   = new   G4HadronElasticProcess();
   G4LElastic* theNeutronLElastic
   = new   G4LElastic();
   if (omit_neutron_hp ) {
   theHadronElasticProcess->RegisterMe( theNeutronLElastic );
   }
   else {
   G4NeutronHPElastic* theNeutronHPElastic
   = new   G4NeutronHPElastic();
   theNeutronHPElastic->SetMaxEnergy( 20.*MeV );
   theNeutronLElastic->SetMinEnergy( 20.*MeV );
   theHadronElasticProcess->RegisterMe( theNeutronHPElastic );
   theHadronElasticProcess->RegisterMe( theNeutronLElastic );
//-1210
AddDataSet(theHadronElasticProcess, new G4NeutronHPElasticData() );
}
pmanager->AddDiscreteProcess( theHadronElasticProcess );
G4NeutronInelasticProcess*   theNeutronInelasticProcess
= new   G4NeutronInelasticProcess();
G4LENeutronInelastic* theNeutronLENeutronInelastic
= new   G4LENeutronInelastic();
G4HENeutronInelastic* theNeutronHENeutronInelastic
= new   G4HENeutronInelastic();
if (omit_neutron_hp) {
theNeutronInelasticProcess->RegisterMe( theNeutronLENeutronInelastic );
theNeutronInelasticProcess->RegisterMe( theNeutronHENeutronInelastic );
}
else {
	G4NeutronHPInelastic* theNeutronHPInelastic
		= new   G4NeutronHPInelastic();
	theNeutronHPInelastic->SetMaxEnergy( 20.*MeV );
	theNeutronLENeutronInelastic->SetMinEnergy( 20.*MeV );
	theNeutronInelasticProcess->RegisterMe( theNeutronHPInelastic );
	theNeutronInelasticProcess->RegisterMe( theNeutronLENeutronInelastic );
	theNeutronInelasticProcess->RegisterMe( theNeutronHENeutronInelastic );
	//-1210
	AddDataSet(theNeutronInelasticProcess, new G4NeutronHPInelasticData() );
}
pmanager->AddDiscreteProcess( theNeutronInelasticProcess );

G4HadronFissionProcess*   theHadronFissionProcess
= new   G4HadronFissionProcess();
G4LFission* theNeutronLFission
= new   G4LFission();
if (omit_neutron_hp) {
	theHadronFissionProcess->RegisterMe( theNeutronLFission );
}
else {
	G4NeutronHPFission* theNeutronHPFission
		= new   G4NeutronHPFission();
	theNeutronHPFission->SetMaxEnergy( 20.*MeV );
	theNeutronLFission->SetMinEnergy( 20.*MeV );
	theHadronFissionProcess->RegisterMe( theNeutronHPFission );
	theHadronFissionProcess->RegisterMe( theNeutronLFission );
	//-1210
	AddDataSet(theHadronFissionProcess, new G4NeutronHPFissionData() );
}
pmanager->AddDiscreteProcess( theHadronFissionProcess );

G4HadronCaptureProcess*   theCaptureProcess
= new   G4HadronCaptureProcess();
G4LCapture* theNeutronLCapture
= new   G4LCapture();
if (omit_neutron_hp) {
	theCaptureProcess->RegisterMe( theNeutronLCapture );
}
else {
	G4NeutronHPCapture* theNeutronHPCapture
		= new   G4NeutronHPCapture();
	theNeutronHPCapture->SetMaxEnergy( 20.*MeV );
	theNeutronLCapture->SetMinEnergy( 20.*MeV );
	theCaptureProcess->RegisterMe( theNeutronHPCapture );
	theCaptureProcess->RegisterMe( theNeutronLCapture );
	//-1210
	AddDataSet(theCaptureProcess, new G4NeutronHPCaptureData() );
}
pmanager->AddDiscreteProcess( theCaptureProcess );

// special Genericland process that works with GLG4PrimaryGeneratorAction
// to stack particles from neutron capture after diffusion
// deactivated by default because it is unexpected by new users
//      GLG4NeutronDiffusionAndCapture*   theNeutronDiffusion
//	= new   GLG4NeutronDiffusionAndCapture();
//      pmanager->AddDiscreteProcess( theNeutronDiffusion );
//      pmanager->SetProcessActivation( theNeutronDiffusion, false );

// ( end neutron )
}

}
}
*/


