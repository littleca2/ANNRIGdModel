// ====================================================================
//   EventAction.hh
//
// ====================================================================
#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H 

#include "G4UserEventAction.hh"
#include "G4Trajectory.hh"
#include "DrawMessage.hh"

class G4Event;

class EventAction : public G4UserEventAction {
	public:
		EventAction();
		virtual ~EventAction();

		virtual void BeginOfEventAction(const G4Event* anEvent);
		virtual void EndOfEventAction(const G4Event* anEvent);
	private:
		void PrintMain(const G4Event* event);
		void PrintTrajectory(const G4Trajectory* trj);
		void PrintHitCrystal(const G4double* data);
		void PrintHitBGO(const G4double* BGOs, const G4double* BGOt);
		
		DrawMessage Printing;
};

#endif
