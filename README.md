# Optic-Gamma-Scintillator-with-aluminium-GEANT4


## Gamma Scintillator with Aluminium Coating – Geant4 Simulation

This project implements a Geant4 simulation of a **gamma source interacting with an aluminium-coated scintillator**.  
Focuses on aluminium’s reflective and absorptive properties.  

---

## Geometry Definition
Implemented in **PMDetectorConstruction.cc/hh**.  
- Builds a scintillator crystal.  
- Surrounds it with **aluminium barriers**.  
- Defines optical surfaces for gamma–detector interactions.  

---

## Physics List
Defined in **PMPhysicsList.cc/hh**.  
- Uses **FTFP_BERT_HP** physics list.  
- Includes electromagnetic + optical processes.  

---

## Action Initialization
Defined in **PMActionInitialization.cc/hh**.  
- Handles RunAction, EventAction, SteppingAction, SensitiveDetector.  

---

## Primary Generator Action
Defined in **PMPrimaryGenerator.cc/hh**.  
- Generates **gamma particles** incident on the scintillator.  
- Energy can be configured.  

---

## ▶️ Build Instructions
Same as other projects:

```bash
mkdir build && cd build
cmake ..
make
./sim run.mac
