#ifndef PMSENSITIVEDETECTOR_HH
#define PMSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "CLHEP/Units/PhysicalConstants.h"

class PMSensitiveDetector : public G4VSensitiveDetector {
public:
    PMSensitiveDetector(G4String name);
    virtual ~PMSensitiveDetector();

    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);
    virtual void Initialize(G4HCofThisEvent*);
    virtual void EndOfEvent(G4HCofThisEvent*);

private:
    G4int fScintillationCount;     
    G4int fOpticalPhotonCount;     
    G4int fPhotonsAtBarrier;       
    G4double fTotalEnergyDeposit;  
    G4int fPhotonsAtAluminiumBarrier;
};

#endif // PMSENSITIVEDETECTOR_HH
