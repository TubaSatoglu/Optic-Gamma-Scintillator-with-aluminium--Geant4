#ifndef PMSTEPPINGACTION_HH
#define PMSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4Step;
class PMEventAction;

class PMSteppingAction : public G4UserSteppingAction {
public:
    PMSteppingAction(PMEventAction* eventAction);
    virtual ~PMSteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

private:
    PMEventAction* fEventAction;
};

#endif
