// -----------------------------------------------------------------------------
//  G4_QPIX | DetectorConstruction.h
//
//  Class for the definition of the detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "DetectorMessenger.hh"

class G4Material;


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:

  DetectorConstruction();
  virtual ~DetectorConstruction();

public:
    
  G4double GetTargetLength();
  G4double GetTargetRadius();
  G4Material* GetTargetMaterial();       
  G4LogicalVolume* GetLogicTarget();

  G4double GetShieldLength();
  G4double GetShieldThickness();
  G4Material* GetShieldMaterial();       
  G4LogicalVolume* GetLogicShield();

  G4double GetVacuumLength();
  G4double GetVacuumThickness();
  G4Material* GetVacuumMaterial();       
  G4LogicalVolume* GetLogicVacuum();

  G4double GetWallLength();
  G4double GetWallThickness();
  G4Material* GetWallMaterial();       
  G4LogicalVolume* GetLogicWall();
  
  void PrintParameters();

public:

  void SetTargetLength (G4double value);
  void SetTargetRadius (G4double value);
  void SetTargetMaterial (G4String);

  void SetShieldLength (G4double value);
  void SetShieldThickness (G4double value);
  void SetShieldMaterial (G4String);

  void SetVacuumLength (G4double value);
  void SetVacuumThickness (G4double value);
  void SetVacuumMaterial (G4String);

  void SetWallLength (G4double value);
  void SetWallThickness (G4double value);
  void SetWallMaterial (G4String);

private:

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
          void DefineMaterials();

private:

  G4double           fTargetLength;
  G4double           fTargetRadius;
  G4Material*        fTargetMater;
  G4LogicalVolume*   fLogicTarget;

  G4double           fShieldLength;
  G4double           fShieldThickness;
  G4Material*        fShieldMater;
  G4LogicalVolume*   fLogicShield;

  G4double           fVacuumLength;
  G4double           fVacuumThickness;
  G4Material*        fVacuumMater;
  G4LogicalVolume*   fLogicVacuum;

  G4double           fWallLength;
  G4double           fWallThickness;
  G4Material*        fWallMater;
  G4LogicalVolume*   fLogicWall;

  G4double           fWorldLength;
  G4double           fWorldRadius;
  G4Material*        fWorldMater;
  G4VPhysicalVolume* fPhysiWorld;

  G4Material* mXenon;
  G4Material* mXenon136;
    
  G4double targetPressure;
  G4double gapPressure;
  const G4double molarMassXe = 131.292;
  const G4double molarMassXe136 = 135.91;
  G4double densityXe;
  G4double densityXe136;
                
  DetectorMessenger* fDetectorMessenger;

};

#endif
