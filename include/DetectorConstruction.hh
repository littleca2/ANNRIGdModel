// ====================================================================
//    DetectorConstruction.hh
//
// ====================================================================
#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "DrawMessage.hh"

// ====================================================================
//
// class definition
//
// ====================================================================
class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {

public:
  DetectorConstruction();
  ~DetectorConstruction(); 

  void DefineMaterials();
  virtual G4VPhysicalVolume* Construct(); 
	
private:
	G4String name_ge[4];
	G4String name_bgo[4];
	
	G4String calName[4][14];
        G4String sBGOname[2][24];
        G4String tBGOname[2][16];

	G4LogicalVolume* calorLogical_al[14];
        G4LogicalVolume* calorLogical_in[14];
        G4LogicalVolume* calorLogical_Ge[14];
        G4LogicalVolume* calorLogical_insens[14];

        G4LogicalVolume* calorLogical_sBGO_insens[24];
        G4LogicalVolume* calorLogical_sBGO[24];
        G4LogicalVolume* calorLogical_tBGO_insens[16];
        G4LogicalVolume* calorLogical_tBGO[16];
	
	G4VPhysicalVolume* calorPhysical_al[14];
        G4VPhysicalVolume* calorPhysical_in[14];
        G4VPhysicalVolume* calorPhysical_Ge[14];
        G4VPhysicalVolume* calorPhysical_insens[14];

        G4VPhysicalVolume* calorPhysical_sBGO_insens[24];
        G4VPhysicalVolume* calorPhysical_sBGO[24];
        G4VPhysicalVolume* calorPhysical_tBGO_insens[16];
        G4VPhysicalVolume* calorPhysical_tBGO[16];
	
	DrawMessage Printing;
};

#endif
