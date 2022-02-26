// -----------------------------------------------------------------------------
//  G4_QPIX | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"
#include "TrackingSD.h"
#include "DetectorMessenger.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"


DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction(), fTargetMater(0), fLogicTarget(0)
{
  fTargetLength      = 15*cm;
  fTargetRadius      = 10*cm;
  fShieldLength      = fTargetLength;
  fShieldThickness      = 2*cm;
  fVacuumLength      = fTargetLength;
  fVacuumThickness      = 5*cm;
  fWallLength      = fTargetLength;
  fWallThickness      = 2*cm;

  fWorldLength = fWallLength + (5.0*cm);
  fWorldRadius = fTargetRadius + fShieldThickness + fVacuumThickness + fWallThickness + (1.0*cm);

  DefineMaterials();

  fDetectorMessenger = new DetectorMessenger(this);
}

void DetectorConstruction::DefineMaterials()
{
  // target density
  targetPressure=15;
  densityXe = ((targetPressure*molarMassXe)/(0.0821*273.15))/1E3;
  densityXe136 = 80.3; //kg/m3

  // build materials
  //
  mXenon = new G4Material("Xenon", 54, molarMassXe*g/mole, densityXe*g/cm3, kStateGas, 273.15*kelvin, targetPressure=15*atmosphere);
  mXenon136 = new G4Material("Xenon136", 54, molarMassXe136*g/mole, densityXe136*kg/m3, kStateGas, 275.*kelvin, targetPressure=15*atmosphere);

  G4Element* N  = new G4Element("Nitrogen", "N", 7, 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen",   "O", 8, 15.9994*g/mole);
  //
  G4int ncomponents; G4double fractionmass;
  G4Material* Air20 = new G4Material("Air", 1.205*mg/cm3, ncomponents=2,
                      kStateGas, 293.*kelvin, 1.*atmosphere);
    Air20->AddElement(N, fractionmass=0.7);
    Air20->AddElement(O, fractionmass=0.3);
  //
  fWorldMater = Air20;

  // fShieldMater Stainless steel
  G4Element* Cr = new G4Element("Chromium", "Cr", 24, 51.996*g/mole);
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* Mn = new G4Element("Manganese", "Mn", 25, 54.938*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", 14, 28.085*g/mole);
  G4Element* P = new G4Element("Phosphorous", "P", 15, 30.974*g/mole);
  G4Element* S = new G4Element("Sulphur", "S", 16, 32.06*g/mole);
  G4Element* Ni = new G4Element("Nickel", "Ni", 28, 58.693*g/mole);
  G4Element* Fe = new G4Element("Iron", "Fe", 26, 55.845*g/mole);


  fTargetMater = man->FindOrBuildMaterial("G4_lAr");
  fShieldMater = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  fVacuumMater = man->FindOrBuildMaterial("G4_Galactic");
  fWallMater = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

 ///G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


DetectorConstruction::~DetectorConstruction()
{delete fDetectorMessenger;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD /////////////////////////////////////////////////

  fWorldLength = fTargetLength + (5.0*cm);
  fWorldRadius = fTargetRadius + (1.0*cm);

  G4Tubs*
  sWorld = new G4Tubs("World",                                 //name
                 0.,fWorldRadius, 0.5*fWorldLength, 0.,twopi); //dimensions

  G4LogicalVolume*
  lWorld = new G4LogicalVolume(sWorld,                  //shape
                             fWorldMater,               //material
                             "World");                  //name

  fPhysiWorld = new G4PVPlacement(0,                    //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            lWorld,                     //logical volume
                            "World",                    //name
                            0,                          //mother volume
                            false,                      //no boolean operation
                            0);                         //copy number

  // Target //////////////////////////////////////////////

  G4Tubs*
  sTarget = new G4Tubs("Target",                                   //name
                  0., fTargetRadius, 0.5*fTargetLength, 0.,twopi); //dimensions


  fLogicTarget = new G4LogicalVolume(sTarget,           //shape
                             fTargetMater,              //material
                             "Target");                 //name

           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           fLogicTarget,                //logical volume
                           "Target",                    //name
                           lWorld,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number

  //////////////////////////////////////////////////////////
  // Shield
  //
  G4Tubs* 
  sShield = new G4Tubs("Shield",  
                fTargetRadius, fTargetRadius+fShieldThickness, 0.5*fShieldLength, 0.,twopi);


  fLogicShield = new G4LogicalVolume(sShield,       //shape
                             fShieldMater,            //material
                             "Shield");               //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           fLogicShield,              //logical volume
                           "Shield",                  //name
                           lWorld,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number



  // Vacuum

  G4Tubs* 
  sVacuum = new G4Tubs("Vacuum",  
                fTargetRadius+fVacuumThickness, fTargetRadius+fShieldThickness+fVacuumThickness, 0.5*fVacuumLength, 0.,twopi);


  fLogicVacuum = new G4LogicalVolume(sVacuum,       //shape
                             fVacuumMater,            //material
                             "Vacuum");               //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           fLogicVacuum,              //logical volume
                           "Vacuum",                  //name
                           lWorld,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number


// Wall

  G4Tubs* 
  sWall = new G4Tubs("Wall",  
                fTargetRadius+fShieldThickness+fVacuumThickness, fTargetRadius+fShieldThickness+fVacuumThickness+fWallThickness, 0.5*fWallLength, 0.,twopi);


  fLogicWall = new G4LogicalVolume(sWall,       //shape
                             fWallMater,            //material
                             "Wall");               //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           fLogicWall,              //logical volume
                           "Wall",                  //name
                           lWorld,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number


  PrintParameters();


  return fPhysiWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  // SENSITIVE DETECTOR ////////////////////////////////////

  TrackingSD* tracking_sd = new TrackingSD("/G4QPIX/TRACKING", "TrackingHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracking_sd);

  G4LogicalVolume* detector_logic_vol =
    G4LogicalVolumeStore::GetInstance()->GetVolume("Target");

  SetSensitiveDetector(fLogicTarget, tracking_sd);

  //////////////////////////////////////////////////////////
}

void DetectorConstruction::PrintParameters()
{

  G4cout << "\n Target : Length = " << G4BestUnit(fTargetLength,"Length")
         << " Radius = " << G4BestUnit(fTargetRadius,"Length")
         << " Material = " << fTargetMater->GetName();
  G4cout << "\n Shield : Length = " << G4BestUnit(fShieldLength,"Length")
         << " Thickness = " << G4BestUnit(fShieldThickness,"Length")
         << " Material = " << fShieldMater->GetName();
  G4cout << "\n Vacuum : Length = " << G4BestUnit(fVacuumLength,"Length")
         << " Thickness = " << G4BestUnit(fVacuumThickness,"Length")
         << " Type = " << fVacuumMater->GetName();
  G4cout << "\n Wall : Length = " << G4BestUnit(fWallLength,"Length")
         << " Thickness = " << G4BestUnit(fWallThickness,"Length")
         << " Material = " << fWallMater->GetName();

  G4cout << "\n" << fTargetMater << "\n" << fShieldMater << "\n" << fVacuumMater << "\n" << fWallMater << G4endl;

}

void DetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial) {
    fTargetMater = pttoMaterial;
    if(fLogicTarget) { fLogicTarget->SetMaterial(fTargetMater); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetTargetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

void DetectorConstruction::SetTargetRadius(G4double value)
{
  fTargetRadius = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetTargetLength(G4double value)
{
  fTargetLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetShieldMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial) {
    fShieldMater = pttoMaterial;
    if(fLogicShield) { fLogicShield->SetMaterial(fShieldMater); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetShieldMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

void DetectorConstruction::SetShieldThickness(G4double value)
{
  fShieldThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetShieldLength(G4double value)
{
  fShieldLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetVacuumMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial) {
    fVacuumMater = pttoMaterial;
    if(fLogicVacuum) { fLogicVacuum->SetMaterial(fVacuumMater); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetVacuumMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

void DetectorConstruction::SetVacuumThickness(G4double value)
{
  fVacuumThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetVacuumLength(G4double value)
{
  fVacuumLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetWallMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial) {
    fWallMater = pttoMaterial;
    if(fLogicWall) { fLogicWall->SetMaterial(fWallMater); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetWallMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

void DetectorConstruction::SetWallThickness(G4double value)
{
  fWallThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetWallLength(G4double value)
{
  fWallLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

G4double DetectorConstruction::GetTargetLength()
{
  return fTargetLength;
}

G4double DetectorConstruction::GetTargetRadius()
{
  return fTargetRadius;
}

G4Material* DetectorConstruction::GetTargetMaterial()
{
  return fTargetMater;
}

G4LogicalVolume* DetectorConstruction::GetLogicTarget()
{
  return fLogicTarget;
}

G4double DetectorConstruction::GetShieldLength()
{
  return fShieldLength;
}

G4double DetectorConstruction::GetShieldThickness()
{
  return fShieldRadius;
}

G4Material* DetectorConstruction::GetShieldMaterial()
{
  return fShieldMater;
}

G4LogicalVolume* DetectorConstruction::GetLogicShield()
{
  return fLogicShield;
}

G4double DetectorConstruction::GetVacuumLength()
{
  return fVacuumLength;
}

G4double DetectorConstruction::GetVacuumThickness()
{
  return fVacuumThickness;
}

G4Material* DetectorConstruction::GetVacuumMaterial()
{
  return fVacuumMater;
}

G4LogicalVolume* DetectorConstruction::GetLogicVacuum()
{
  return fLogicVacuum;
}

G4double DetectorConstruction::GetWallLength()
{
  return fWallLength;
}

G4double DetectorConstruction::GetWallThickness()
{
  return fWallThickness;
}

G4Material* DetectorConstruction::GetWallMaterial()
{
  return fWallMater;
}

G4LogicalVolume* DetectorConstruction::GetLogicWall()
{
  return fLogicWall;
}
