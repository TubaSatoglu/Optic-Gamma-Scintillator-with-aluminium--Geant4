#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class PMDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
private:
    G4Material* CreateScintillatorMaterial();
    G4Material* CreateAluminiumMaterial(); 

    G4LogicalVolume* BuildWorld();
    G4LogicalVolume* BuildScintillator();
    G4LogicalVolume* BuildBarrier();
    
    G4VPhysicalVolume* PlaceWorld(G4LogicalVolume*);
    G4VPhysicalVolume* PlaceScintillatorInWorld(G4LogicalVolume*, G4LogicalVolume*);
    G4VPhysicalVolume* PlaceBarrierInWorld(G4LogicalVolume*, G4LogicalVolume*);
    
    G4VPhysicalVolume* worldPhys;
    G4VPhysicalVolume* scintillatorPhys;
    G4LogicalVolume* scintillatorLogical;
    G4LogicalVolume* barrierLogical;
};

#endif
