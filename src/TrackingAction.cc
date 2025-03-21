////////////////////////////////////////////////////////////
// $Id: TrackingAction.cc,v 1.0 2015/02/03  T.Yano
//

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "TrackingAction.hh"

#include "G4Trajectory.hh"

/////////////////////////////////
TrackingAction::TrackingAction()
/////////////////////////////////
{
}

/////////////////////////////////
TrackingAction::~TrackingAction()
////////////////////////////////
{
}

/////////////////////////////////////////////////////////////////
void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
/////////////////////////////////////////////////////////////////
{
    G4VTrajectory* traj = new G4Trajectory(aTrack);
    fpTrackingManager->SetTrajectory(traj);
    fpTrackingManager->SetStoreTrajectory(true);
}

