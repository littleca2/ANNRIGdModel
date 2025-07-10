//////////////////////////////////////////////////////////////////////////////
//                   Spectrum of radiative neutron capture by Gadolinium           
//                                    version 1.0.0                                
//                                    (Sep.09.2005)                               

//                Author : karim.zbiri@subatech.in2p3.fr                  

//Modified class from original G4NeutronHPCaptureFS class to deexcite and
//add correctly the secondary to the hadronic final state

// Karim Zbiri, Aug, 2005
///////////////////////////////////////////////////////////////////////////////


#include "GdNeutronHPCaptureFS.hh"
#include "G4Gamma.hh"
#include "G4ReactionProduct.hh"
#include "G4Nucleus.hh"
#include "G4PhotonEvaporation.hh"
#include "G4Fragment.hh"
#include "G4ParticleTable.hh" 
#include "G4NeutronHPDataUsed.hh"
#include "ANNRIGd_GdNCaptureGammaGenerator.hh"
#include "ANNRIGd_GeneratorConfigurator.hh"
#include "ANNRIGd_OutputConverter.hh"

#include "G4SystemOfUnits.hh"

// Juan David Cortes, June 30, 2025, This might solve the theResult problem:

#include "G4Cache.hh"


// Juan David Cortes, June 30, 2025, Because FindIon() was moved
#include "G4IonTable.hh"

// Juan David Cortes, July 2, 2025, This will, hopefully, get Gd into the IonTables
#include "G4NuclideTable.hh"

// Juan David Cortes, July 7, 2025, Just for the sake of knowing the current MC iteration

#include <cmath>
int eventCount = 0;
int totalEventNum = 100;
double percentProgress = 0.0;
double eventsPerSecond = log(static_cast<double>(totalEventNum) / 1.1547) / 2.2163; // Assuming linearly completed neutron capture events and a logarithmic processing relation with the total number of events in the simulation
double elapsedTime = 0.0;
double totalTime = static_cast<double>(totalEventNum) / eventsPerSecond; // In Seconds 
double remainingTime = totalTime; // Also in seconds
int daysElapsed = 0;
int daysRemaining = 0;
int hoursElapsed = 0;
int hoursRemaining = 0;
int minutesElapsed = 0;
int minutesRemaining = 0;
int secondsElapsed = 0;
int secondsRemaining = 0;
int numSecondsPerDay = 86400;
int numSecondsPerHour = 3600;
int numSecondsPerMinute = 60;

namespace AGd = ANNRIGdGammaSpecModel;

#define File_Name "GdNeutronHPCaptireFS.cc"
extern G4int    MODEL;      //1:garnet, 2:glg4sim, 4:ANNRI-Gd
extern G4int    Gd_CAPTURE; //1:natural , 2:enriched 157Gd, 3:enriched 155Gd
extern G4int    Gd_CASCADE; //1:discrete + continuum; 2:discrete, 3:continuum
extern G4String Gd157_ROOTFile;
extern G4String Gd155_ROOTFile;

// use a static generator to avoid the construction and loading of data
// files for each Gd isotope
AGd::ANNRIGd_GdNCaptureGammaGenerator* GdNeutronHPCaptureFS::sAnnriGammaGen = 0;

////////////////////////////////////////////////////////////////////////////////////////
GdNeutronHPCaptureFS::GdNeutronHPCaptureFS() :
////////////////////////////////////////////////////////////////////////////////////////
nucleus(0), theTwo(0), targetMass(0)
{
	hasXsec = false;
	if(MODEL == 4 and not sAnnriGammaGen) InitANNRIGdGenerator();
}

////////////////////////////////////////////////////////////////////////////////////////
GdNeutronHPCaptureFS::~GdNeutronHPCaptureFS()
////////////////////////////////////////////////////////////////////////////////////////
{;}

////////////////////////////////////////////////////////////////////////////////////////
G4HadFinalState * GdNeutronHPCaptureFS::ApplyYourself(const G4HadProjectile & theTrack)
///////////////////////////////////////////////////////////////////////////////////////
{
	G4int i;

	// Juan David Cortes, June 30, 2025, It is necessary to get the cache and then clear it. This will also prove radically important when dealing
	// with AddSecondary(), as you will see below
	auto * theFinalState = theResult.Get();

	if (!theFinalState) {
		theResult.Put(new G4HadFinalState);
		theFinalState = theResult.Get();		
		theFinalState->Clear();
		cout << "theFinalState was null" << endl;
	}

	////////////////prepare neutron//////////////////////
	G4double eKinetic = theTrack.GetKineticEnergy();
	const G4HadProjectile *incidentParticle = &theTrack;
	//theNeutron = const_cast<G4ParticleDefinition *>(incidentParticle->GetDefinition()) ;
	// Juan David Cortes, June 30, 2025,  G4ReactionProduct theNeutron( const_cast<G4ParticleDefinition *>(incidentParticle->GetDefinition()) );
	// Juan David Cortes, June 30, 2025, I think that it would just be better to work with the this* variables instead of just shadowing (I don't see any
	// benefit to shadowing in this very specific case)
	auto theNeutron = G4ReactionProduct( const_cast<G4ParticleDefinition *>(incidentParticle->GetDefinition()) );
	theNeutron.SetMomentum( incidentParticle->Get4Momentum().vect() );
	theNeutron.SetKineticEnergy( eKinetic );

	/////////////////prepare target////////////////////
	// Juan David Cortes, June 30, 2025, Again, because I don't think that shadowing should be necessary in this case
	G4ReactionProduct theTarget; 
	G4Nucleus aNucleus;
	G4double eps = 0.0001;
	if(targetMass<500*MeV)
		//targetMass = ( G4NucleiPropertiesTable::GetNuclearMass(static_cast<G4int>(theBaseZ+eps), static_cast<G4int>(theBaseA+eps))) /
		targetMass = ( G4NucleiProperties::GetNuclearMass( static_cast<G4int>(theBaseA+eps) , static_cast<G4int>(theBaseZ+eps) )) /
			G4Neutron::Neutron()->GetPDGMass();
	G4ThreeVector neutronVelocity = 1./G4Neutron::Neutron()->GetPDGMass()*theNeutron.GetMomentum();
	G4double temperature = theTrack.GetMaterial()->GetTemperature();
	theTarget = aNucleus.GetBiasedThermalNucleus(targetMass, neutronVelocity, temperature);

	//////////////go to nucleus rest system////////////////
	theNeutron.Lorentz(theNeutron, -1*theTarget);
	eKinetic = theNeutron.GetKineticEnergy();

	///////////////////dice the photons////////////////////
	G4ReactionProductVector * thePhotons = NULL;    
	if(MODEL==1){
		thePhotons = theFinalgammas_ggarnet.GetGammas();
	}else if(MODEL==2){
		thePhotons = theFinalgammas_glg4sim.GetGammas();
	}else if(MODEL==4){
		thePhotons = GenerateWithANNRIGdGenerator();
	}else{
		Printing.Error(File_Name,"MODEL");
	}

	////////////////////update the nucleus/////////////////
	G4ThreeVector aCMSMomentum = theNeutron.GetMomentum()+theTarget.GetMomentum();
	G4LorentzVector p4(aCMSMomentum, theTarget.GetTotalEnergy() + theNeutron.GetTotalEnergy());
	nucleus = new G4Fragment(static_cast<G4int>(theBaseA+1), static_cast<G4int>(theBaseZ) ,p4);

	G4int nPhotons = 0;
	if(thePhotons!=NULL) nPhotons=thePhotons->size();
	for(i=0;i<nPhotons;i++){
		G4Fragment * theOne;
		G4ThreeVector pGamma(thePhotons->operator[](i)->GetMomentum());
		G4LorentzVector gamma(pGamma,thePhotons->operator[](i)->GetTotalEnergy());
		theOne= new G4Fragment(gamma,G4Gamma::GammaDefinition());
		UpdateNucleus(theOne,thePhotons->operator[](i)->GetTotalEnergy());
	}
	theTwo = new G4DynamicParticle;

	// Juan David Cortes, June 30, 2025, I consider that this is necessary in order to properly use the FindIon() function
	// theTwo->SetDefinition(G4ParticleTable::GetParticleTable()->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0, static_cast<G4int>(theBaseZ)));
	G4ParticleTable* theTable1 = G4ParticleTable::GetParticleTable();
	G4IonTable* tableOfIons1 = theTable1->GetIonTable();
	tableOfIons1->CreateAllIon();

	// Juan David Cortes, July 10, 2025, I consider that this is necessary to properly initialize the IonTable
	auto* nuclideTable1 = G4NuclideTable::GetNuclideTable();
	nuclideTable1->SetThresholdOfHalfLife(0.0);
	nuclideTable1->GenerateNuclide();
	size_t nIso1 = nuclideTable1->entries();
	for (size_t p = 0; p < nIso1; ++p) {
		auto* prop = nuclideTable1->GetIsotopeByIndex(p);
		tableOfIons1->GetIon(prop->GetAtomicNumber(), prop->GetAtomicMass(), prop->GetIsomerLevel());
	}


	//cout << "Number of Ions in tableOfIons1: " << tableOfIons1->Entries() << endl;
	//theTwo->SetDefinition(tableOfIons1->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0, static_cast<G4int>(theBaseZ)));

	auto* tempIonTheTwo = tableOfIons1->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0);


	//cout << "Ions in tableOfIons1" << endl;
	G4int ionsInTableOfIons1 = tableOfIons1->Entries();
	for (G4int n = 0; n < ionsInTableOfIons1; ++n) {
		G4ParticleDefinition* ion = tableOfIons1->GetParticle(n);
		G4int Z  = ion->GetAtomicNumber();
		G4double A = ion->GetAtomicMass();
		//cout << ion->GetParticleName() << "  Z=" << Z << "  A=" << A << endl;
	}

	//cout << "For tempIonTheTwo: "<< "Z = " << static_cast<G4int>(theBaseZ) << "A = " << static_cast<G4int>(theBaseA+1) << endl;


	if (!tempIonTheTwo) {

		cout << "tempIonTheTwo is null" << endl;

	}

	theTwo->SetDefinition(tempIonTheTwo);


	//if (tempIonTheTwo) {

		//theTwo->SetDefinition(tempIonTheTwo);

	//}

	//cout << "tempIonTheTwo is null" << endl;


	//theTwo->SetDefinition(tableOfIons1->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0));

	theTwo->SetMomentum(nucleus->GetMomentum());

	/////////////add them to the final state///////////////
	G4int nParticles = nPhotons;
	if(1==nPhotons) nParticles = 2;

	/////////////////back to lab system///////////////////
	for(i=0; i<nPhotons; i++){
		thePhotons->operator[](i)->Lorentz(*(thePhotons->operator[](i)), theTarget);
	}

	//////////////Recoil, if only one gamma//////////////
	if (1==nPhotons){
		G4DynamicParticle * theOne = new G4DynamicParticle;

		// Juan David Cortes, June 30, 2025, I consider that this is necessary in order to properly use the FindIon() function
		// G4ParticleDefinition * aRecoil = G4ParticleTable::GetParticleTable()->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0, static_cast<G4int>(theBaseZ));
		G4ParticleTable* theTable2 = G4ParticleTable::GetParticleTable();
		G4IonTable* tableOfIons2 = theTable2->GetIonTable();

		// Juan David Cortes, July 10, 2025, Doing this to get Gd on the IonTable
		tableOfIons2->CreateAllIon();


		// Juan David Cortes, July 10, 2025, I consider that this is necessary to properly initialize the IonTable
		auto* nuclideTable2 = G4NuclideTable::GetNuclideTable();
		nuclideTable2->SetThresholdOfHalfLife(0.0);
		nuclideTable2->GenerateNuclide();
		size_t nIso2 = nuclideTable2->entries();
		for (size_t r = 0; r < nIso2; ++r) {
			auto* prop = nuclideTable2->GetIsotopeByIndex(r);
			tableOfIons2->GetIon(prop->GetAtomicNumber(), prop->GetAtomicMass(), prop->GetIsomerLevel());
		}


		//cout << "Number of Ions in tableOfIons2: " << tableOfIons2->Entries() << endl;
		//G4ParticleDefinition * aRecoil = tableOfIons2->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0, static_cast<G4int>(theBaseZ));

		G4ParticleDefinition * aRecoil = tableOfIons2->FindIon(static_cast<G4int>(theBaseZ), static_cast<G4int>(theBaseA+1), 0);

		theOne->SetDefinition(aRecoil);
		// Now energy; 
		// Can be done slightly better @
		G4ThreeVector aMomentum =  theTrack.Get4Momentum().vect()
			+theTarget.GetMomentum()
			-thePhotons->operator[](0)->GetMomentum();

		G4ThreeVector theMomUnit = aMomentum.unit();
		G4double aKinEnergy =  theTrack.GetKineticEnergy()
			+theTarget.GetKineticEnergy(); // gammas come from Q-value
		G4double theResMass = aRecoil->GetPDGMass();
		G4double theResE = aRecoil->GetPDGMass()+aKinEnergy;
		G4double theAbsMom = std::sqrt(theResE*theResE - theResMass*theResMass);
		G4ThreeVector theMomentum = theAbsMom*theMomUnit;
		theOne->SetMomentum(theMomentum);

		// Juan David Cortes, June 30, 2025, theFinalState is used instead of theResult		
		theFinalState->AddSecondary(theOne);
	}

	//////////////Now fill in the gammas.////////////////////
	for(i=0; i<nPhotons; i++){
		// back to lab system
		G4DynamicParticle * theOne = new G4DynamicParticle;
		theOne->SetDefinition(thePhotons->operator[](i)->GetDefinition());
		theOne->SetMomentum(thePhotons->operator[](i)->GetMomentum());
		// G4cout << "PID: gamma is generated by Gd. The momentum is:" << thePhotons->operator[](i)->GetMomentum() << G4endl;//Added by Yano, to see the function working correctly.

		// Juan David Cortes, June 30, 2025, theFinalState is used instead of theResult		
		theFinalState->AddSecondary(theOne);
		delete thePhotons->operator[](i);
	}
	delete thePhotons; 

	// Juan David Cortes, June 30, 2025, Again, theFinalState is used instead of theResult
	///////////////ADD deexcited nucleus////////////////////
	theFinalState->AddSecondary(theTwo);




	// Juan David Cortes, July 7, 2025, Just for the sake of knowing our current iteration
	eventCount = eventCount + 1;
	elapsedTime = static_cast<double>(eventCount) / eventsPerSecond;

	// Juan David Cortes, July 7, 2025, This should legibly express elapsedTime in days, hours, minutes and seconds 
	daysElapsed = elapsedTime / numSecondsPerDay;
	hoursElapsed = (static_cast<int>(elapsedTime) % numSecondsPerDay) / numSecondsPerHour;
	minutesElapsed = (static_cast<int>(elapsedTime) % numSecondsPerHour) / numSecondsPerMinute;
	secondsElapsed = static_cast<int>(elapsedTime) % numSecondsPerMinute;


	remainingTime = totalTime - elapsedTime; 

	// Juan David Cortes, July 7, 2025, This should legibly express remainingTime in days, hours, minutes and seconds 
	daysRemaining = remainingTime / numSecondsPerDay;
	hoursRemaining = (static_cast<int>(remainingTime) % numSecondsPerDay) / numSecondsPerHour;
	minutesRemaining = (static_cast<int>(remainingTime) % numSecondsPerHour) / numSecondsPerMinute;
	secondsRemaining = static_cast<int>(remainingTime) % numSecondsPerMinute; 


	percentProgress = (static_cast<double>(eventCount) / static_cast<double>(totalEventNum)) * 100.0;
	cout << "Event number " <<  eventCount << " of " << totalEventNum << endl;
	cout << "Estimated Elapsed time: " << daysElapsed << " days, " << hoursElapsed << " hours, " << minutesElapsed << " minutes, " << secondsElapsed <<  " seconds" << endl;
	cout << "Estimated Time Remaining: " << daysRemaining << " days, " << hoursRemaining << " hours, " << minutesRemaining << " minutes, " << secondsRemaining <<  " seconds" << endl;
	cout << "Current Progress: " << percentProgress << "%" << endl << endl << endl;



	/////////////clean up the primary neutron///////////////
	theFinalState->SetStatusChange(stopAndKill);
	return theFinalState;
}

//////////////////////////////////////////////////////////////////////////////////////
void GdNeutronHPCaptureFS::UpdateNucleus( const G4Fragment* gamma , G4double eGamma )
	//////////////////////////////////////////////////////////////////////////////////////
{

	G4LorentzVector p4Gamma = gamma->GetMomentum();
	G4ThreeVector pGamma(p4Gamma.vect());

	G4LorentzVector p4Nucleus(nucleus->GetMomentum() );

	G4ParticleTable* theTable = G4ParticleTable::GetParticleTable();

	// Juan David Cortes, June 30, 2025, Again, I will adjust this code for it to work with the appropriate FindIon()
	G4IonTable* tableOfIons = theTable->GetIonTable();

	tableOfIons->CreateAllIon();

	auto* nuclideTable = G4NuclideTable::GetNuclideTable();
	nuclideTable->SetThresholdOfHalfLife(0.0);
	nuclideTable->GenerateNuclide();
	size_t nIso = nuclideTable->entries();
	//cout << "Number of Isotopes in nuclideTable: " << nIso << endl;
	for (size_t k = 0; k < nIso; ++k) {
		auto* prop = nuclideTable->GetIsotopeByIndex(k);
		tableOfIons->GetIon(prop->GetAtomicNumber(), prop->GetAtomicMass(), prop->GetIsomerLevel());
	}

	//cout << "Number of Ions in tableOfIons: " << tableOfIons->Entries() << endl;

	// Juan David Cortes, June 30, 2025, Since the definition of m2 below shadows the defintion of meters squared in G4SystemOfUnits, I will rename m1 and m2 to tempMass1 and tempMass2 in order to avoid
	// any shadowing and maintain the same naming scheme
	//G4double tempMass1 = tableOfIons->FindIon(static_cast<G4int>(nucleus->GetZ()), static_cast<G4int>(nucleus->GetA()), 0, static_cast<G4int>(nucleus->GetZ())) ->GetPDGMass();

//G4double tempMass1 = tableOfIons->FindIon(static_cast<G4int>(nucleus->GetZ()), static_cast<G4int>(nucleus->GetA())) ->GetPDGMass();

	auto* tempIon1 = tableOfIons->FindIon(static_cast<G4int>(nucleus->GetZ()), static_cast<G4int>(nucleus->GetA()), 0);
	//cout << "Ions in tableOfIons" << endl;

	
	G4int ionsInTableOfIons = tableOfIons->Entries();
	for (G4int j = 0; j < ionsInTableOfIons; ++j) {
		G4ParticleDefinition* ion = tableOfIons->GetParticle(j);
		G4int Z  = ion->GetAtomicNumber();
		G4double A = ion->GetAtomicMass();
		//cout << ion->GetParticleName() << "  Z=" << Z << "  A=" << A << endl;
	}

	//cout << "For tempIon1: " << "Z = " << static_cast<G4int>(nucleus->GetZ()) << "A = " << static_cast<G4int>(nucleus->GetA()) << endl;


	G4double tempMass1 = 0.0;

	if (!tempIon1) {
		cout << "tempIon1 is null" << endl;
	}

	tempMass1 = tempIon1->GetPDGMass();

	//if (tempIon1) {
		//tempMass1 = tempIon1->GetPDGMass();
	//}
	//cout << "tempIon1 is null" << endl;

	//G4double m1 = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(static_cast<G4int>(nucleus->GetZ()),
	//static_cast<G4int>(nucleus->GetA()));
	G4double tempMass2 = nucleus->GetZ() *  G4Proton::Proton()->GetPDGMass() + 
		(nucleus->GetA()- nucleus->GetZ())*G4Neutron::Neutron()->GetPDGMass();

	G4double Mass = std::min(tempMass1,tempMass2);

	G4double newExcitation = p4Nucleus.mag() - Mass - eGamma;

	//G4cout<<" Egamma = "<<eGamma<<G4endl;
	//G4cout<<" NEW EXCITATION ENERGY = "<< newExcitation <<G4endl;

	if(newExcitation < 0) newExcitation = 0;

	G4ThreeVector p3Residual(p4Nucleus.vect() - pGamma);
	G4double newEnergy = std::sqrt(p3Residual * p3Residual +
			(Mass + newExcitation) * (Mass + newExcitation));
	G4LorentzVector p4Residual(p3Residual, newEnergy);

	// Update excited nucleus parameters
	nucleus->SetMomentum(p4Residual);

	//  G4cout<<"nucleus excitation energy = "<<nucleus->GetExcitationEnergy() <<G4endl;  

	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//void GdNeutronHPCaptureFS::Init (G4double A, G4double Z, G4String & dirName, G4String & )
#include <sstream> 
//  void GdNeutronHPCaptureFS::Init (G4double A, G4double Z, G4int M, G4String & dirName, G4String & )
void GdNeutronHPCaptureFS::Init (G4double A, G4double Z, G4int ,G4String & dirName, G4String & )
	///////////////////////////////////////////////////////////////////////////////////////////////////
{
	G4String tString = "/FS/";
	G4bool dbool;
	G4NeutronHPDataUsed aFile = theNames.GetName(static_cast<G4int>(A), static_cast<G4int>(Z), dirName, tString, dbool);

	G4String filename = aFile.GetName();
	theBaseA = A;
	theBaseZ = G4int(Z+.5);
	if(!dbool || ( Z<2.5 && ( std::abs(theBaseZ - Z)>0.0001 || std::abs(theBaseA - A)>0.0001)))
	{
		hasAnyData = false;
		hasFSData = false; 
		hasXsec = false;
		return;
	}
	std::ifstream theData(filename, std::ios::in);

	hasFSData = theFinalStatePhotons.InitMean(theData); 
	if(hasFSData)
	{
		targetMass = theFinalStatePhotons.GetTargetMass();
		theFinalStatePhotons.InitAngular(theData); 
		theFinalStatePhotons.InitEnergies(theData); 
	}
	theData.close();
}

////////////////////////////////////////////////////////////////////////////////
void GdNeutronHPCaptureFS::InitANNRIGdGenerator()
////////////////////////////////////////////////////////////////////////////////
{
	if(sAnnriGammaGen) delete sAnnriGammaGen;
	sAnnriGammaGen = new AGd::ANNRIGd_GdNCaptureGammaGenerator();

	AGd::ANNRIGd_GeneratorConfigurator::Configure(*sAnnriGammaGen,
			Gd_CAPTURE, Gd_CASCADE, Gd155_ROOTFile, Gd157_ROOTFile);
}

////////////////////////////////////////////////////////////////////////////////
G4ReactionProductVector* GdNeutronHPCaptureFS::GenerateWithANNRIGdGenerator()
////////////////////////////////////////////////////////////////////////////////
{
	G4ReactionProductVector* theProducts = 0;

	if(sAnnriGammaGen) {
		AGd::ReactionProductVector products;
		if(Gd_CAPTURE == 1)                          // nat. Gd
			products = sAnnriGammaGen->Generate_NatGd();
		else if(Gd_CAPTURE == 2 and Gd_CASCADE == 1)  // 157Gd, discrete + continuum
			products = sAnnriGammaGen->Generate_158Gd();
		else if(Gd_CAPTURE == 2 and Gd_CASCADE == 2)  // 157Gd, discrete
			products = sAnnriGammaGen->Generate_158Gd_Discrete();
		else if(Gd_CAPTURE == 2 and Gd_CASCADE == 3)  // 157Gd, continuum
			products = sAnnriGammaGen->Generate_158Gd_Continuum();
		else if(Gd_CAPTURE == 3 and Gd_CASCADE == 1)  // 155Gd, discrete + continuum
			products = sAnnriGammaGen->Generate_156Gd();
		else if(Gd_CAPTURE == 3 and Gd_CASCADE == 2)  // 155Gd, discrete
			products = sAnnriGammaGen->Generate_156Gd_Discrete();
		else if(Gd_CAPTURE == 3 and Gd_CASCADE == 3)  // 155Gd, continuum
			products = sAnnriGammaGen->Generate_156Gd_Continuum();

		theProducts = AGd::ANNRIGd_OutputConverter::ConvertToG4(products);
	}
	else {
		Printing.Error(File_Name,"sAnnriGammaGen");
		theProducts = new G4ReactionProductVector();
	}

	return theProducts;
}
