// ====================================================================
//   CalorimeterSD.cc
//
// ====================================================================
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "CalorimeterSD.hh"
#include "CalHit.hh"


//////////////////////////////////////////////////
	CalorimeterSD::CalorimeterSD(const G4String& name)
:G4VSensitiveDetector(name)
	//////////////////////////////////////////////////
{
	collectionName.insert("calorimeter");
}

///////////////////////////////
CalorimeterSD::~CalorimeterSD()
	///////////////////////////////
{
}

/////////////////////////////////////////////////////
void CalorimeterSD::Initialize(G4HCofThisEvent* HCTE)
	/////////////////////////////////////////////////////
{
	// create hit collection(s)
	hitsCollection = new CalHitsCollection(SensitiveDetectorName,collectionName[0]); 

	// push H.C. to "Hit Collection of This Event"
	G4int hcid = GetCollectionID(0);
	HCTE->AddHitsCollection(hcid,hitsCollection);

	// clear energy deposit buffer
	for(G4int i=0;i<NCHANNEL;i++) edepbuf[i]=0.;
}

/////////////////////////////////////////////////////////////////////
G4bool CalorimeterSD::ProcessHits(G4Step* astep,G4TouchableHistory* )
	/////////////////////////////////////////////////////////////////////
{
	// get step information from "PreStepPoint"
	const G4StepPoint* preStepPoint = astep-> GetPreStepPoint();
	G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint-> GetTouchable());

	// accumulate energy deposit in each scintillator
	G4int id = touchable->GetCopyNumber();
	edepbuf[id] += astep->GetTotalEnergyDeposit();

	return true;
}

/////////////////////////////////////////////////
void CalorimeterSD::EndOfEvent(G4HCofThisEvent* )
	/////////////////////////////////////////////////
{
	// make hits and push them to "Hit Coleltion"
	for(G4int id=0;id<NCHANNEL;id++){
		if(edepbuf[id]>0.){
			CalHit* ahit = new CalHit(id, edepbuf[id]);
			hitsCollection->insert(ahit);
		}
	}
}

/////////////////////////////
void CalorimeterSD::DrawAll()
	/////////////////////////////
{
}

//////////////////////////////
void CalorimeterSD::PrintAll()
	//////////////////////////////
{
	hitsCollection->PrintAllHits();
}

