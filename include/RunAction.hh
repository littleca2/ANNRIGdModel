// ====================================================================
//   RunAction.hh
//
// ====================================================================
#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include "G4UserRunAction.hh"

using namespace std;
class G4Run;
class TH1D;
class TNtuple;
class TTree;

class RunAction : public G4UserRunAction {
private:
  TTree* tree;
  TTree* tre;

public:
  RunAction();
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);
};

#endif
