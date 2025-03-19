// ====================================================================
//   BGOSD.hh
//
// ====================================================================
#ifndef BGO_SD_H
#define BGO_SD_H
 
#include "G4VSensitiveDetector.hh"
#include "BGOHit.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

enum { CHANNEL=50 };

class BGOSD : public G4VSensitiveDetector {
private:
  BGOHitsCollection* BGOhitsCollection;
  G4double BGOedep[CHANNEL]; // buffer for energy deposit

public:
  BGOSD(const G4String& name);
  virtual ~BGOSD();

  // virtual methods
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void Initialize(G4HCofThisEvent* BGOHCTE);
  virtual void EndOfEvent(G4HCofThisEvent* BGOHCTE);

  virtual void DrawAll();
  virtual void PrintAll(); 
 
};

#endif
