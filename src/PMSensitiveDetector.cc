#include "PMSensitiveDetector.hh"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4TrackStatus.hh"
#include "CLHEP/Units/PhysicalConstants.h"

PMSensitiveDetector::PMSensitiveDetector(G4String name)
    : G4VSensitiveDetector(name),
      fScintillationCount(0),
      fOpticalPhotonCount(0),
      fPhotonsAtAluminiumBarrier(0) {}

PMSensitiveDetector::~PMSensitiveDetector() {}

void PMSensitiveDetector::Initialize(G4HCofThisEvent*) {
    fScintillationCount = 0;
    fOpticalPhotonCount = 0;
    fPhotonsAtAluminiumBarrier = 0;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4Track* track = step->GetTrack();
    
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
        return false;
    }

    G4StepPoint* preStep = step->GetPreStepPoint();
    G4String volName = preStep->GetPhysicalVolume()->GetName();
    
    if (volName == "AluminiumBarrierPhys" && preStep->GetStepStatus() == fGeomBoundary) {
        fPhotonsAtAluminiumBarrier++;
    }

    fOpticalPhotonCount++;

    G4double energy = track->GetKineticEnergy();
    G4double wavelength = (CLHEP::h_Planck * CLHEP::c_light / energy);
    G4ThreeVector position = preStep->GetPosition();
    G4double time = preStep->GetGlobalTime();
    G4double depositedEnergy = step->GetTotalEnergyDeposit();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (fPhotonsAtBarrier > 0) {
        analysisManager->FillH1(1, fPhotonsAtBarrier);
    }

    if (fOpticalPhotonCount > 0) {
        analysisManager->FillNtupleIColumn(0, G4RunManager::GetRunManager()
            ->GetCurrentEvent()->GetEventID());
        analysisManager->FillNtupleDColumn(1, position.x()/mm);
        analysisManager->FillNtupleDColumn(2, position.y()/mm);
        analysisManager->FillNtupleDColumn(3, position.z()/mm);
        analysisManager->FillNtupleDColumn(4, time/ns);
        analysisManager->FillNtupleDColumn(5, wavelength/nm);
        analysisManager->FillNtupleDColumn(6, depositedEnergy / MeV);
        analysisManager->AddNtupleRow();
    }

    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
    if (fOpticalPhotonCount > 0) {
        G4cout << "\n======= Event Summary =======" << G4endl;
        G4cout << "🔹 Event ID: " << G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << G4endl;
        G4cout << "🔹 Total Optical Photons: " << fOpticalPhotonCount << G4endl;
        G4cout << "🔹 Photons at Lead Barrier: " << fPhotonsAtBarrier << G4endl;
        G4cout << "🔹 Scintillation Photons: " << fScintillationCount << G4endl;
        
        G4double detectionEfficiency = (fOpticalPhotonCount > 0) ?
            (100.0 * fPhotonsAtBarrier / fOpticalPhotonCount) : 0.0;
        G4cout << "🔹 Detection Efficiency: " << detectionEfficiency << " %" << G4endl;
        G4cout << "=====================================\n" << G4endl;
    }
}

