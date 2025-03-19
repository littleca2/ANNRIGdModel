// ====================================================================
//   BGOSD.cc
//
// ====================================================================
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "BGOSD.hh"
#include "BGOHit.hh"


//////////////////////////////////////////////////
	BGOSD::BGOSD(const G4String& name)
: G4VSensitiveDetector(name)
	//////////////////////////////////////////////////
{
	collectionName.insert(name);
}

///////////////////////////////
BGOSD::~BGOSD()
	///////////////////////////////
{
}

/////////////////////////////////////////////////////
void BGOSD::Initialize(G4HCofThisEvent* BGOHCTE)
	/////////////////////////////////////////////////////
{
	// create hit collection(s)
	BGOhitsCollection= new BGOHitsCollection(SensitiveDetectorName,collectionName[0]); 

	// push H.C. to "Hit Collection of This Event"
	G4int BGOid= GetCollectionID(0);
	BGOHCTE-> AddHitsCollection(BGOid, BGOhitsCollection);

	// clear energy deposit buffer
	for (G4int i=0; i<CHANNEL; i++) BGOedep[i]=0.;
}

///////////////////////////////////////////////////////////////
G4bool BGOSD::ProcessHits(G4Step* astep, G4TouchableHistory* )
	///////////////////////////////////////////////////////////////
{
	// get step information from "PreStepPoint"
	const G4StepPoint* preStepPoint= astep-> GetPreStepPoint();
	G4TouchableHistory* touchable=
		(G4TouchableHistory*)(preStepPoint-> GetTouchable());

	// accumulate energy deposit in each scintillator
	G4int BGOid= touchable-> GetCopyNumber();
	BGOedep[BGOid]+= astep-> GetTotalEnergyDeposit();

	return true;
}

/////////////////////////////////////////////////
void BGOSD::EndOfEvent(G4HCofThisEvent* )
	/////////////////////////////////////////////////
{
	// make hits and push them to "Hit Coleltion"
	for (G4int BGOid=0; BGOid< CHANNEL; BGOid++) {
		if(BGOedep[BGOid] > 0. ) {
			BGOHit* ahit= new BGOHit(BGOid, BGOedep[BGOid]);
			BGOhitsCollection-> insert(ahit);
		}
	}
}

/////////////////////////////
void BGOSD::DrawAll()
	/////////////////////////////
{
}

//////////////////////////////
void BGOSD::PrintAll()
	//////////////////////////////
{
	BGOhitsCollection-> PrintAllHits();
}

