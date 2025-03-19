// ====================================================================
//   BGOHit.hh
//
// ====================================================================
#ifndef BGO_HIT_H
#define BGO_HIT_H
 
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class BGOHit : public G4VHit {
private:
  G4int BGOid;
  G4double BGOedep;

public:
  BGOHit();
  BGOHit(G4int aid, G4double aedep);
  virtual ~BGOHit();

  // copy constructor & assignment operator
  BGOHit(const BGOHit& right);
  const BGOHit& operator=(const BGOHit& right);
  G4int operator==(const BGOHit& right) const;
  
  // new/delete operators
  void* operator new(size_t);
  void operator delete(void* aHit);
  
  // set/get functions
  void SetID(G4int aid) { BGOid = aid; }
  G4int GetID() const { return BGOid; }

  void SetEdep(G4double aedep) { BGOedep = aedep; }
  G4double GetEdep() const { return BGOedep; }

  // methods
  virtual void Draw();
  virtual void Print();
};

// ====================================================================
// inline functions
// ====================================================================
inline BGOHit::BGOHit(const BGOHit& right)
  : G4VHit()
{
  BGOid= right.BGOid;
  BGOedep= right.BGOedep;
}

inline const BGOHit& BGOHit::operator=(const BGOHit& right)
{
  BGOid= right.BGOid;
  BGOedep= right.BGOedep;
  return *this;
}

inline G4int BGOHit::operator==(const BGOHit& right) const 
{
   return (this==&right) ? 1 : 0; 
}

typedef G4THitsCollection<BGOHit> BGOHitsCollection;
extern G4Allocator<BGOHit> BGOHitAllocator; 

inline void* BGOHit::operator new(size_t)
{
  void* aHit= (void*)BGOHitAllocator.MallocSingle();
  return aHit;
}

inline void BGOHit::operator delete(void* aHit)
{
  BGOHitAllocator.FreeSingle((BGOHit*) aHit);
}

#endif
