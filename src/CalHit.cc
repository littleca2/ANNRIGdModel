// ====================================================================
//
//   CalHit.cc
//
// ====================================================================
#include "CalHit.hh"
#include "G4SystemOfUnits.hh"

// allocator
G4Allocator<CalHit> CalHitAllocator;

////////////////////
	CalHit::CalHit()
: id(-1), edep(0.)
	////////////////////
{
}

/////////////////////////////////////////
	CalHit::CalHit(G4int aid, G4double aedep)
: id(aid), edep(aedep)
	/////////////////////////////////////////
{
}

/////////////////
CalHit::~CalHit()
	/////////////////
{
}


///////////////////
void CalHit::Draw()
	///////////////////
{
}

////////////////////
void CalHit::Print()
	////////////////////
{
	G4cout << "Cal Hit:" << id << "::" 
		<< edep/MeV << " MeV" << G4endl;
}

