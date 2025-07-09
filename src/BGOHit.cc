// ====================================================================
//   BGOHit.cc
//
// ====================================================================
#include "BGOHit.hh"
#include "G4SystemOfUnits.hh"

// allocator
G4Allocator<BGOHit> BGOHitAllocator;

////////////////////
	BGOHit::BGOHit()
: BGOid(-1), BGOedep(0.)
	////////////////////
{
}

/////////////////////////////////////////
	BGOHit::BGOHit(G4int aid,G4double aedep)
: BGOid(aid), BGOedep(aedep)
	/////////////////////////////////////////
{
}

/////////////////
BGOHit::~BGOHit()
	/////////////////
{
}


///////////////////
void BGOHit::Draw()
	///////////////////
{
}

////////////////////
void BGOHit::Print()
	////////////////////
{
	G4cout << "BGO Hit:" << BGOid << "::" 
		<< BGOedep/MeV << " MeV" << G4endl;
}

