
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
#include "G4HadronElastic.hh"

// Juan David Cortes, June 27, 2025, This should replace the previous low energy models for baryons and mesons (protons, neutrons, pions, kaons) 
#include "G4CascadeInterface.hh"

// Juan David Cortes, June 27, 2025, This should replace the previous low energy models for deuterons and light ions(deuterons, tritons and alphas)
#include "G4BinaryLightIonReaction.hh"
  


// High-energy Models: >20 GeV

// Juan David Cortes, June 30, 2025, It is necessary to include this in order to properly use "G4FTFModel.hh"
#include "G4TheoFSGenerator.hh" 
#include "G4LundStringFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4PreCompoundModel.hh"
#include "G4GeneratorPrecompoundInterface.hh"

// Juan David Cortes, June 27, 2025, This should replace the previous high energy models
#include "G4FTFModel.hh"
 

// Neutron high-precision models: <20 MeV

// Juan David Cortes, 27 June, 2025, To use instead of the "G4NeutronHPorLElastic.hh" file
#include "G4NeutronHPElastic.hh"
 
#include "G4NeutronHPElasticData.hh"
#include "GdNeutronHPCapture.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelasticData.hh"

// Juan David Cortes, 30 June, 2025, For some reason, this wasn't included before:
#include "G4NeutronHPInelastic.hh"


// Juan David Cortes, 27 June, 2025, To use instead of the "G4LCapture.hh" file
#include "G4NeutronHPCapture.hh" 

// Stopping processes

// Juan David Cortes, 27 June, 2025, To use instead of "G4PiMinusAbsorbtionAtRest.hh", "G4KaonMinusAbsorptionAtRest.hh"
// I don't think that G4AntiProtonAnnihilationAtRest.hh" should be used
#include "G4PiMinusAbsorptionBertini.hh"
#include "G4KaonMinusAbsorptionBertini.hh"
  
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

	// Juan David Cortes, 27 June 2025, This should properly define theParticleIterator 
	G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();

	G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess();
	G4HadronElastic* theElasticModel = new G4HadronElastic();   
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
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();
		        theInelasticProcess->RegisterMe(theLEInelasticModel);	


			G4TheoFSGenerator * theHEInelasticModel = 
				new G4TheoFSGenerator("FTFP");
			G4FTFModel * theHEStringModel = 
				new G4FTFModel();
			G4LundStringFragmentation * theHELund = 
				new G4LundStringFragmentation();
			G4ExcitedStringDecay * theHEStringDecay = 
				new G4ExcitedStringDecay(theHELund);
			G4VPreCompoundModel * theHEDeExcitation = 
				new G4PreCompoundModel();
			G4GeneratorPrecompoundInterface * theHETransport = 
				new G4GeneratorPrecompoundInterface(theHEDeExcitation);

			theHEStringModel->SetFragmentationModel(theHEStringDecay);

			theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
			theHEInelasticModel->SetTransport(theHETransport);

			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		
 
		}

		else if (particleName == "pi-") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4PionMinusInelasticProcess* theInelasticProcess =
				new G4PionMinusInelasticProcess("inelastic");    
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();           
			theInelasticProcess->RegisterMe(theLEInelasticModel);


			G4TheoFSGenerator * theHEInelasticModel =
				new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);

			G4String prcNam; 
			pmanager->AddRestProcess(new G4PiMinusAbsorptionBertini(), ordDefault);


		} 

		else if (particleName == "kaon+") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonPlusInelasticProcess* theInelasticProcess =
				new G4KaonPlusInelasticProcess("inelastic");
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);

			
			G4TheoFSGenerator * theHEInelasticModel =
				new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);


		} 

		else if (particleName == "kaon0S") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonZeroSInelasticProcess* theInelasticProcess =  
				new G4KaonZeroSInelasticProcess("inelastic");
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);    
		

			G4TheoFSGenerator * theHEInelasticModel =
				new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);

	
		} 

		else if (particleName == "kaon0L") {
			pmanager->AddDiscreteProcess(theElasticProcess); 
			G4KaonZeroLInelasticProcess* theInelasticProcess =
				new G4KaonZeroLInelasticProcess("inelastic");
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);
	

			G4TheoFSGenerator * theHEInelasticModel =
                                new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);

	
		} 

		else if (particleName == "kaon-") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonMinusInelasticProcess* theInelasticProcess =
				new G4KaonMinusInelasticProcess("inelastic");
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);
	
			
			G4TheoFSGenerator * theHEInelasticModel =
                                new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);
	

			pmanager->AddRestProcess(new G4KaonMinusAbsorptionBertini, ordDefault);
		}

		else if (particleName == "proton") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4ProtonInelasticProcess* theInelasticProcess =
				new G4ProtonInelasticProcess("inelastic");
			G4CascadeInterface* theLEInelasticModel = new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);


			G4TheoFSGenerator * theHEInelasticModel =
                                new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);


		}

		else if (particleName == "anti_proton") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AntiProtonInelasticProcess* theInelasticProcess =
				new G4AntiProtonInelasticProcess("inelastic");
			G4CascadeInterface* theLEInelasticModel = 
				new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);


			G4TheoFSGenerator * theHEInelasticModel =
                                new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);
		
		}

		else if (particleName == "neutron") {
			// elastic scattering
			G4HadronElasticProcess* theNeutronElasticProcess =
				new G4HadronElasticProcess;
			G4NeutronHPElastic * theElasticNeutron = new G4NeutronHPElastic();
			theNeutronElasticProcess->RegisterMe(theElasticNeutron);
			G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData();
			theNeutronElasticProcess->AddDataSet(theNeutronData);
			pmanager->AddDiscreteProcess(theNeutronElasticProcess);

			// inelastic scattering
			G4NeutronInelasticProcess* theInelasticProcess = 
				new G4NeutronInelasticProcess("inelastic");
			G4NeutronHPInelastic* theInelasticModel = new G4NeutronHPInelastic();
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
			G4CascadeInterface* theLEInelasticModel =
				new G4CascadeInterface();
			theInelasticProcess->RegisterMe(theLEInelasticModel);


			G4TheoFSGenerator * theHEInelasticModel =
                                new G4TheoFSGenerator("FTFP");
                        G4FTFModel * theHEStringModel =
                                new G4FTFModel();
                        G4LundStringFragmentation * theHELund =
                                new G4LundStringFragmentation();
                        G4ExcitedStringDecay * theHEStringDecay =
                                new G4ExcitedStringDecay(theHELund);
                        G4VPreCompoundModel * theHEDeExcitation =
                                new G4PreCompoundModel();
                        G4GeneratorPrecompoundInterface * theHETransport =
                                new G4GeneratorPrecompoundInterface(theHEDeExcitation);

                        theHEStringModel->SetFragmentationModel(theHEStringDecay);

                        theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
                        theHEInelasticModel->SetTransport(theHETransport);

                        theInelasticProcess->RegisterMe(theHEInelasticModel);
                        pmanager->AddDiscreteProcess(theInelasticProcess);

		
		}

		else if (particleName == "deuteron") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4DeuteronInelasticProcess* theInelasticProcess =
				new G4DeuteronInelasticProcess("inelastic");
			G4BinaryLightIonReaction* theLEInelasticModel =
				new G4BinaryLightIonReaction();
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "triton") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4TritonInelasticProcess* theInelasticProcess =
				new G4TritonInelasticProcess("inelastic");
			G4BinaryLightIonReaction* theLEInelasticModel =
				new G4BinaryLightIonReaction();
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if (particleName == "alpha") {
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AlphaInelasticProcess* theInelasticProcess =
				new G4AlphaInelasticProcess("inelastic");
			G4BinaryLightIonReaction* theLEInelasticModel =
				new G4BinaryLightIonReaction();
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


