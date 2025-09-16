#include "PMDetectorConstruction.hh"
#include "PMSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

PMDetectorConstruction::PMDetectorConstruction() {}

PMDetectorConstruction::~PMDetectorConstruction() {}

G4VPhysicalVolume* PMDetectorConstruction::Construct() {
    G4double worldSize = 50.0 * cm;
    G4Box* worldBox = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldBox, air, "World");
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, G4ThreeVector(),
                                                     worldLV, "World", nullptr, false, 0);

    G4Material* scintMaterial = CreateScintillatorMaterial();
    G4Material* aluminiumMaterial = CreateAluminiumMaterial(); 

    G4double scintX = 10.0 * cm;
    G4double scintY = 10.0 * cm;
    G4double scintZ = 3.0 * cm;
    G4Box* scintBox = new G4Box("Scintillator", scintX/2, scintY/2, scintZ/2);
    scintillatorLogical = new G4LogicalVolume(scintBox, scintMaterial, "Scintillator");

    G4double aluminiumX = 15. * cm;
    G4double aluminiumY = 15. * cm;
    G4double aluminiumZ = 0.01 * cm;  // Thin barrier
    G4Box* aluminiumBox = new G4Box("AluminiumBarrier", aluminiumX/2, aluminiumY/2, aluminiumZ/2);
    barrierLogical = new G4LogicalVolume(aluminiumBox, aluminiumMaterial, "AluminiumBarrier");

    G4VPhysicalVolume* scintillatorPhys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0*cm),
                                                            scintillatorLogical, "ScintillatorPhys", worldLV, false, 0);
    
    // Place Aluminium Barrier (above the scintillator)
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 2.*cm),
                     barrierLogical, "AluminiumBarrierPhys", worldLV, false, 0);

    G4OpticalSurface* scintSurface = new G4OpticalSurface("ScintSurface");
    scintSurface->SetType(dielectric_dielectric);
    scintSurface->SetFinish(polished);
    scintSurface->SetModel(unified);

    G4MaterialPropertiesTable* surfaceProps = new G4MaterialPropertiesTable();
    std::vector<G4double> energy = {1.0*eV, 6.0*eV};
    std::vector<G4double> reflectivity = {0.95, 0.95};
    surfaceProps->AddProperty("REFLECTIVITY", energy.data(), reflectivity.data(), 2);
    scintSurface->SetMaterialPropertiesTable(surfaceProps);

    new G4LogicalBorderSurface("ScintBorderSurface", scintillatorPhys, worldPhys, scintSurface);

    G4VisAttributes* scintVis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0)); // Yellow
    G4VisAttributes* aluminiumVis = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));  // Orange 
    
    scintillatorLogical->SetVisAttributes(scintVis);
    barrierLogical->SetVisAttributes(aluminiumVis); 

    return worldPhys;
}

void PMDetectorConstruction::ConstructSDandField() {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    PMSensitiveDetector* scintSD = new PMSensitiveDetector("ScintillatorSD");
    sdManager->AddNewDetector(scintSD);
    scintillatorLogical->SetSensitiveDetector(scintSD);
}

G4Material* PMDetectorConstruction::CreateScintillatorMaterial() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    G4MaterialPropertiesTable* scintMPT = new G4MaterialPropertiesTable();

    std::vector<G4double> photonEnergies = {
        2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV, 2.177*eV, 2.216*eV,
        2.256*eV, 2.298*eV, 2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV
    };

    // Refractive Index
    std::vector<G4double> rindex(photonEnergies.size(), 1.85);
    scintMPT->AddProperty("RINDEX", photonEnergies.data(), rindex.data(), photonEnergies.size());

    // Scintillation Spectrum
    std::vector<G4double> scintil = { 0.02, 0.07, 0.15, 0.32, 0.57, 0.85, 1.00, 0.85, 0.57, 0.32, 0.15, 0.07 };
    scintMPT->AddProperty("FASTCOMPONENT", photonEnergies.data(), scintil.data(), photonEnergies.size(), true);

    // Absorption Length
    std::vector<G4double> absorption(photonEnergies.size(), 380.*cm);
    scintMPT->AddProperty("ABSLENGTH", photonEnergies.data(), absorption.data(), photonEnergies.size());

    // Scintillation Parameters
    scintMPT->AddConstProperty("SCINTILLATIONYIELD", 40000. / MeV, true); 
    scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.0, true);
    scintMPT->AddConstProperty("FASTTIMECONSTANT", 250.*ns, true);
    scintMPT->AddConstProperty("YIELDRATIO", 1.0, true);

    NaI->SetMaterialPropertiesTable(scintMPT);
    return NaI;
}

G4Material* PMDetectorConstruction::CreateAluminiumMaterial() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* Al = nist->FindOrBuildMaterial("G4_Al");

    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    std::vector<G4double> energy = {1.0*eV, 6.0*eV};
    std::vector<G4double> rindex = {1.39, 1.39}; 
    std::vector<G4double> absorption = {1.*mm, 1.*mm};

    mpt->AddProperty("RINDEX", energy.data(), rindex.data(), 2);
    mpt->AddProperty("ABSLENGTH", energy.data(), absorption.data(), 2);

    Al->SetMaterialPropertiesTable(mpt);
    return Al;
}
