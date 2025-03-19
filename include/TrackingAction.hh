////////////////////////////////////////////////////////////
// $Id: TrackingAction.hh,v 1.0 2015/02/03 TYano $
//
#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
class G4Track;

class TrackingAction : public G4UserTrackingAction
{
  public:
    TrackingAction();
    virtual ~TrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
};
#endif
