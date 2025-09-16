#include "PMSteppingAction.hh"
#include "PMEventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"
#include "G4PhysicalConstants.hh"

PMSteppingAction::PMSteppingAction(PMEventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

PMSteppingAction::~PMSteppingAction() {}


void PMSteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4String particleName = track->GetDefinition()->GetParticleName();
    
    if (track->GetDefinition() == G4OpticalPhoton::Definition()) {
        return;
    }

    G4StepPoint* preStep = step->GetPreStepPoint();
    G4StepPoint* postStep = step->GetPostStepPoint();
    G4ThreeVector pos = preStep->GetPosition();
    
    G4double kineticEnergy = preStep->GetKineticEnergy() / MeV;
    G4double momentum = preStep->GetMomentum().mag() / (MeV / c_light);
    G4double depositedEnergy = step->GetTotalEnergyDeposit() / MeV;  // Energy deposition

    G4String processName = "Unknown";
    if (postStep->GetProcessDefinedStep()) {
        processName = postStep->GetProcessDefinedStep()->GetProcessName();
    }

    G4VPhysicalVolume* volume = preStep->GetPhysicalVolume();
    G4String volumeName = (volume) ? volume->GetName() : "Unknown";

    if (depositedEnergy > 0) {
        fEventAction->RecordEnergy(depositedEnergy);
    }

    G4cout << "\n=== Step Information ===\n"
           << "Particle: " << particleName << "\n"
           << "Position (mm): (" << pos.x() / mm << ", " << pos.y() / mm << ", " << pos.z() / mm << ")\n"
           << "Kinetic Energy: " << kineticEnergy << " MeV\n"
           << "Momentum: " << momentum << " MeV/c\n"
           << "Deposited Energy: " << depositedEnergy /MeV << " MeV\n"
           << "Current Volume: " << volumeName << "\n"
           << "Process: " << processName << "\n"
           << "=========================\n" << G4endl;
}
