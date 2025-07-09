// ====================================================================
//  DetectorConstruction.cc
// ====================================================================
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Cons.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4UnitsTable.hh"
#include "G4Colour.hh"
#include "G4String.hh"
#include "G4SDManager.hh"

// Juan David Cortes, June 27, 2025, this should solve the units issues
#include "G4SystemOfUnits.hh"

// Juan David Cortes, June 27, 2025, this should solve the STP_Temperature issue
#include "G4PhysicalConstants.hh"

#include "DetectorConstruction.hh"
#include "CalorimeterSD.hh"
#include "BGOSD.hh"
using namespace std;

//========================================================
////////////////////// Define Structure //////////////////
//========================================================
#define File_name "DetectorConstruction.cc"
extern G4int TARGET;	
extern G4int BEAM_DUCT;		
extern G4int COLOR;		
extern G4double SOURCE_POSX;	
extern G4double SOURCE_POSY;	
extern G4double SOURCE_POSZ;	
extern G4double HOLDER_POSX;	
extern G4double HOLDER_POSY;	
extern G4double HOLDER_POSZ;
extern G4int Gd_CAPTURE;
//========================================================
//////////////////// Define Structure fin/////////////////
//========================================================

// ====================================================================
//
// class description
//
// ====================================================================
////////////////////////////////////////////
DetectorConstruction::DetectorConstruction()
////////////////////////////////////////////
{
//========================================================
//////////////////// Definition of Name /////////////////
//========================================================
	//Ge detectors
	name_ge[0] = "dete_al";
	name_ge[1] = "dete_in";
	name_ge[2] = "dete_Ge";
	name_ge[3] = "dete_insens";
	for(size_t i=0;i<14;i++){
		calorLogical_al[i] = 0;
		calorLogical_in[i] = 0;
		calorLogical_Ge[i] = 0;
		calorLogical_insens[i] = 0;
		calorPhysical_al[i] = 0;
		calorPhysical_in[i] = 0;
		calorPhysical_Ge[i] = 0;
		calorPhysical_insens[i] = 0;
	}
	for(G4int i=0;i<14;i++){
		for(G4int num=0;num<4;num++){
			name_ge[num].append(i);
			calName[num][i] = name_ge[num].c_str();
		}
	}
	
	//BGO detectors
	name_bgo[0] = "sBGO_insens";
	name_bgo[1] = "sBGO";
	name_bgo[2] = "tBGO_insens";
	name_bgo[3] = "tBGO";
	//Side BGO detectors
	for(G4int i=0;i<24;i++){
		for(G4int num=0;num<2;num++){
		name_bgo[num].append(i);
		sBGOname[num][i] = name_bgo[num].c_str();
		}
	}
	for(size_t i=0;i<24;i++){
		calorLogical_sBGO_insens[i] = 0;
		calorLogical_sBGO[i] = 0;
		calorPhysical_sBGO_insens[i] = 0;
		calorPhysical_sBGO[i] = 0;
	}

	//Top BGO detectors
	for(G4int i=0;i<16;i++){
		for(G4int num=0;num<2;num++){
		name_bgo[num+2].append(i);
		tBGOname[num][i] = name_bgo[num+2].c_str();
		}
	}
	for(size_t i=0;i<16;i++){
		calorLogical_tBGO_insens[i] = 0;
		calorLogical_tBGO[i] = 0;
		calorPhysical_tBGO_insens[i] = 0;
		calorPhysical_tBGO[i] = 0;
	}
}


/////////////////////////////////////////////
DetectorConstruction::~DetectorConstruction()
/////////////////////////////////////////////
{
}

////////////////////////////////////////////
void DetectorConstruction::DefineMaterials()
	////////////////////////////////////////////
{	
	G4NistManager* manager=G4NistManager::Instance();
	G4String symbol;             //a=mass of a mole;
	G4double a,density;      //z=mean number of protons;  
	G4int iz,n;
	G4int ncomponents, natoms;
	G4double abundance,fractionmass;

	//===================================================
	//////////////////// define Elements/////////////////
	// ==================================================
	G4Isotope* Gd152 = new G4Isotope("Gd152",iz= 64,n=152,a=151.919786*g/mole);
	G4Isotope* Gd154 = new G4Isotope("Gd154",iz= 64,n=154,a=153.920861*g/mole);
	G4Isotope* Gd155 = new G4Isotope("Gd155",iz= 64,n=155,a=154.922618*g/mole);
	G4Isotope* Gd156 = new G4Isotope("Gd156",iz= 64,n=156,a=155.922118*g/mole);
	G4Isotope* Gd157 = new G4Isotope("Gd157",iz= 64,n=157,a=156.923956*g/mole);
	G4Isotope* Gd158 = new G4Isotope("Gd158",iz= 64,n=158,a=157.924099*g/mole);
	G4Isotope* Gd160 = new G4Isotope("Gd160",iz= 64,n=160,a=159.927049*g/mole);

	G4Element* H = manager->FindOrBuildElement("H");
	G4Element* C = manager->FindOrBuildElement("C");
	G4Element* O = manager->FindOrBuildElement("O");
	G4Element* Mg = manager->FindOrBuildElement("Mg");
	G4Element* Al = manager->FindOrBuildElement("Al");
	G4Element* Si = manager->FindOrBuildElement("Si");
	G4Element* Cl = manager->FindOrBuildElement("Cl");
	G4Element* Ti = manager->FindOrBuildElement("Ti");
	G4Element* Cr = manager->FindOrBuildElement("Cr");
	G4Element* Mn = manager->FindOrBuildElement("Mn");
	G4Element* Fe = manager->FindOrBuildElement("Fe");
	G4Element* Cu = manager->FindOrBuildElement("Cu");
	G4Element* Zn = manager->FindOrBuildElement("Zn");
	G4Element* Ge = manager->FindOrBuildElement("Ge");
	G4Element* Zr = manager->FindOrBuildElement("Zr");
//	G4Element* Gd = manager->FindOrBuildElement("Gd");

	G4Element* Gd = new G4Element("Gadolinium", "Gd", ncomponents=7);
	Gd->AddIsotope(Gd152, abundance= 0.20*perCent);
	Gd->AddIsotope(Gd154, abundance= 2.18*perCent);
	Gd->AddIsotope(Gd155, abundance=14.80*perCent);
	Gd->AddIsotope(Gd156, abundance=20.47*perCent);
	Gd->AddIsotope(Gd157, abundance=15.65*perCent);
	Gd->AddIsotope(Gd158, abundance=24.84*perCent);
	Gd->AddIsotope(Gd160, abundance=21.86*perCent);

	G4Element* eGd155 = new G4Element("eGd155", "eGd155", ncomponents=6);
	eGd155->AddIsotope(Gd154, abundance= 0.50*perCent);
	eGd155->AddIsotope(Gd155, abundance=91.90*perCent);
	eGd155->AddIsotope(Gd156, abundance= 5.87*perCent);
	eGd155->AddIsotope(Gd157, abundance= 0.81*perCent);
	eGd155->AddIsotope(Gd158, abundance= 0.65*perCent);
	eGd155->AddIsotope(Gd160, abundance= 0.27*perCent);

	G4Element* eGd157 = new G4Element("eGd157", "eGd157", ncomponents=6);
	eGd157->AddIsotope(Gd154, abundance= 0.05*perCent);
	eGd157->AddIsotope(Gd155, abundance= 0.30*perCent);
	eGd157->AddIsotope(Gd156, abundance= 1.63*perCent);
	eGd157->AddIsotope(Gd157, abundance= 88.4*perCent);
	eGd157->AddIsotope(Gd158, abundance= 9.02*perCent);
	eGd157->AddIsotope(Gd160, abundance= 0.60*perCent);

	G4Material* matAl = manager->FindOrBuildMaterial("G4_Al");
	G4Material* matPb = manager->FindOrBuildMaterial("G4_Pb");
	G4Material* matCu = manager->FindOrBuildMaterial("G4_Cu");
	G4Material* matLi = manager->FindOrBuildMaterial("G4_Li");
	
	G4Material* Teflon = manager->FindOrBuildMaterial("G4_TEFLON");
	G4Material* BGO = manager->FindOrBuildMaterial("G4_BGO");
	G4Material* Air = manager->FindOrBuildMaterial("G4_AIR");
	G4Material* LiF = manager->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
	G4Material* LiH = manager->FindOrBuildMaterial("G4_LITHIUM_HYDRIDE");

	G4Material* LiF80 = new G4Material("LiF80", 2.108*g/cm3, ncomponents=1);
	LiF80->AddMaterial(LiF, abundance= 100*perCent);

	G4Material* LiH80 = new G4Material("LiH80", 0.656*g/cm3, ncomponents=1);
	LiH80->AddMaterial(LiH, abundance= 100*perCent);

/*	G4Material* LiF80 = new G4Material("LiF80", 2.108*g/cm3, ncomponents=2);
	LiF80->AddMaterial(LiF, abundance= 80*perCent);
	LiF80->AddMaterial(Air, abundance= 20*perCent);

	G4Material* LiH80 = new G4Material("LiH80", 0.656*g/cm3, ncomponents=2);
	LiH80->AddMaterial(LiH, abundance= 80*perCent);
	LiH80->AddMaterial(Air, abundance= 20*perCent);
*/
/*	G4Material* LiF80 = new G4Material("LiF80", 2.635*g/cm3, ncomponents=1);
	LiF80->AddMaterial(LiF, abundance= 100*perCent);

	G4Material* LiH80 = new G4Material("LiH80", 0.82*g/cm3, ncomponents=1);
	LiH80->AddMaterial(LiH, abundance= 100*perCent);
*/
/*	G4Material* LiF80 = new G4Material("LiF80", 0.001205*g/cm3, ncomponents=1);
	LiF80->AddMaterial(Air, abundance= 100*perCent);

	G4Material* LiH80 = new G4Material("LiH80", 0.001205*g/cm3, ncomponents=1);
	LiH80->AddMaterial(Air, abundance= 100*perCent);
*/
	matAl->SetName("Aluminium");
	matPb->SetName("lead");
	matCu->SetName("Copper");
	matLi->SetName("Lithium");
	Teflon->SetName("Teflon");
	BGO->SetName("BGO");
	Air->SetName("Air");
	LiF->SetName("LiF");
	LiH->SetName("LiH");

	G4Material* Germanium = new G4Material("Germanium", density=5.323*g/cm3, ncomponents=1);
	Germanium->AddElement(Ge, natoms=1);

	G4Material* Acryl = new G4Material("Acryl",density=1.18*g/cm3,ncomponents=3);
	Acryl->AddElement(C,natoms=5);
	Acryl->AddElement(O,natoms=2);
	Acryl->AddElement(H,natoms=8);

	G4Material* Duralumin = new G4Material("Duralumin",density=2.79*g/cm3,ncomponents=10);
	Duralumin->AddElement(Al,fractionmass=92.95*perCent);
	Duralumin->AddElement(Si,fractionmass= 0.50*perCent);
	Duralumin->AddElement(Fe,fractionmass= 0.70*perCent);
	Duralumin->AddElement(Cu,fractionmass= 4.00*perCent);
	Duralumin->AddElement(Mn,fractionmass= 0.70*perCent);
	Duralumin->AddElement(Mg,fractionmass= 0.60*perCent);
	Duralumin->AddElement(Zn,fractionmass= 0.25*perCent);
	Duralumin->AddElement(Cr,fractionmass= 0.10*perCent);
	Duralumin->AddElement(Zr,fractionmass= 0.10*perCent);
	Duralumin->AddElement(Ti,fractionmass= 0.10*perCent);

	G4Material* Gdfoil = new G4Material("Gdfoil",density= 7.900*g/cm3, ncomponents=1);
	Gdfoil->AddElement(Gd, natoms=1);
	
	G4Material* eGd155foil = new G4Material("eGd155foil",density= 7.407*g/cm3, ncomponents=2);
	eGd155foil->AddElement(eGd155, natoms=2);
	eGd155foil->AddElement(O, natoms=3);
	
	G4Material* eGd157foil = new G4Material("eGd157foil",density= 7.407*g/cm3, ncomponents=2);
	eGd157foil->AddElement(eGd157, natoms=2);
	eGd157foil->AddElement(O, natoms=3);

//	G4Material* Clfoil = new G4Material("Clfoil",density= 3200*g/cm3, ncomponents=1);
	G4Material* Clfoil = new G4Material("Clfoil",density= 3.200*g/cm3, ncomponents=1);
	Clfoil->AddElement(Cl, natoms=1);

	G4Material* Beam = new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,kStateGas, STP_Temperature, 2.e-2*bar);
	Beam->AddMaterial(Air, fractionmass=1.);

	//	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

////////////////////////////////////////////////////
G4VPhysicalVolume* DetectorConstruction::Construct()
	////////////////////////////////////////////////////
{	
	//=========================================--=========================	
	/////////////////////////// Initialization ///////////////////////////
	// ==========================================--=======================
	// define materials
	DefineMaterials();
	G4Material* mate;
	G4Material* mateAl = G4Material::GetMaterial("Aluminium");
	G4Material* mateBea = G4Material::GetMaterial("Beam");
	G4Material* mateGe = G4Material::GetMaterial("Germanium");
	G4Material* mateTef = G4Material::GetMaterial("Teflon");
	G4Material* mateBGO = G4Material::GetMaterial("BGO");
	G4Material* mateLiH = G4Material::GetMaterial("LiH80");
	G4Material* mateLiF = G4Material::GetMaterial("LiF80");
	G4Material* matePb = G4Material::GetMaterial("lead");
	G4Material* mateCu = G4Material::GetMaterial("Copper");
	G4Material* mateAcr = G4Material::GetMaterial("Acryl");
	G4Material* mateDur = G4Material::GetMaterial("Duralumin");
	G4Material* mateCl = G4Material::GetMaterial("Clfoil");
	//Zero Vector
	G4ThreeVector zerov = G4ThreeVector(0.*mm,0.*mm,0.*mm);
	G4RotationMatrix zeror = G4RotationMatrix(0.*deg,0.*deg,0.*deg);
	G4Transform3D zerot = G4Transform3D(zeror,zerov);


	//=============================================--=====================	
	/////////////////////////// Visualization  ///////////////////////////
	// ==============================================--===================
	G4bool visual1[12] = {true,true,true,true,true,true,true,true,true,true,true,false};	//defolt
	G4bool visual2[12] = {false,true,true,true,false,false,false,false,false,true,true,false}; //Ge,Tef,Gd,Acr,Dur
	G4bool visual3[12] = {false,true,false,true,true,false,false,false,false,false,false,false}; //Ge,Gd,BGO
	G4bool visual4[12] = {true,false,false,false,false,true,true,false,false,false,false,false}; //Al,Li
	G4double view_degree;
	G4int view_sBGOf,view_tBGOf;
	G4int view_sBGOe,view_tBGOe;
	G4double view_Ducty;
	G4double view_Ductx;


	G4VisAttributes *va[12];	//Al(Red),Ge(Blue),Tef(Blue),Gd(Red),BGO(Green),LiH(Magenta),LiF(Magenta),Pb(Cyan),Cu(Yellow),Acr(Green),Dur(Green),n(non)
	G4ThreeVector Color[12] = {G4Colour::Red(),G4Colour::Blue(),G4Colour::Blue(),G4Colour::Red(),G4Colour::Green(),G4Colour::Magenta(),G4Colour::Magenta(),G4Colour::Cyan(),G4Colour::Yellow(),G4Colour::Green(),G4Colour::Green(),G4Colour::Black()};

	switch(COLOR){
		case 1:
		case 99:
			for(G4int i=0;i<12;i++){
				va[i]=new G4VisAttributes(Color[i]);
				va[i]->SetVisibility(visual1[i]);
			}
			break;

		case 2:
			for(G4int i=0;i<12;i++){
				va[i]=new G4VisAttributes(Color[i]);
				va[i]->SetVisibility(visual2[i]);
			}
			break;
		case 3:
			for(G4int i=0;i<12;i++){
				va[i]=new G4VisAttributes(Color[i]);
				va[i]->SetVisibility(visual3[i]);
			}
			break;
		case 4:
			for(G4int i=0;i<12;i++){
				va[i]=new G4VisAttributes(Color[i]);
				va[i]->SetVisibility(visual4[i]);
			}
			break;
		default:
			Printing.Error(File_name,"COLOR");
			break;
	}
	if(COLOR==99){
		view_degree=180.;
		view_sBGOf=1;
		view_sBGOe=7;
		view_tBGOf=1;
		view_tBGOe=4;
		view_Ducty=108./2.+1.;
		view_Ductx=92./2.+1.;
	}else{
		view_degree=360.;
		view_sBGOf=0;
		view_tBGOf=0;
		view_sBGOe=12;
		view_tBGOe=8;
		view_Ducty=0.001;
		view_Ductx=0.001;
	}

	//==================================================================	
	/////////////////////////// World Volume ///////////////////////////
	// =================================================================
	G4Box* worldSolid = new G4Box("world", 100.*cm, 100.*cm, 100.*cm);
	mate = G4Material::GetMaterial("Air");
	G4LogicalVolume* worldLV = new G4LogicalVolume(worldSolid, mate, "world");

	G4PVPlacement* world = new G4PVPlacement(0, G4ThreeVector(),"world",worldLV, 0, false, 0);
	worldLV->SetVisAttributes(va[11]);

	// ================================================================
	//////////////////////////// Ge Detectors /////////////////////////
	// ================================================================
	/////////////Variable/////////////////////
	G4ThreeVector cutvec;
	G4RotationMatrix cutdeg;
	const G4double dphi = 60.*deg;
	G4double x,y;
	G4double h;
	G4double dia,hei,r,R;
	G4double dhei,l;
	G4double theta;
	G4double dh,dy;

	G4double Al_th = 0.7;   // it must be less than 1.4 (defult value is 0.7)

	/////////////Aluminium Case////////////////
	dia = 61.5;		//Distance of side to side
	hei = 78.0;		//Height 
	R = 70.0;		//Diameter 
	dhei = 12.0;		//Height of cylinder part 
	x = dia/sqrt(3.);	//Length of one side
	y = (R-dia)/2.;		//Width of cut
	h = hei-dhei;		//Height of cut
	theta = 1.*atan(h/y);	//Angle of cut
	dh = 10.0;
	dy = (y/h)*(h+dh)-y;

	G4VSolid* tube = new G4Tubs("tube", 0.*mm, R/2.*mm, hei/2.*mm, 0.*deg, 360.*deg);
	G4VSolid* trd = new G4Trd("trd",x*mm, x*mm, 0.*mm, (y+dy)*mm,(h+dh)/2.*mm);

	cutvec = G4ThreeVector(0.*mm,-1.0*(R/2+dy)*mm,(hei-h-dh)/2*mm);
	cutvec.rotateZ(dphi);
	cutdeg.rotateZ(dphi);
	G4VSolid* crys = new G4SubtractionSolid("crys",tube,trd,G4Transform3D(cutdeg,cutvec));
	for(G4int i=1;i<6;i++){
		cutvec.rotateZ(dphi);
		cutdeg.rotateZ(dphi);
		crys = new G4SubtractionSolid("crys",crys,trd,G4Transform3D(cutdeg,cutvec));
	}

	for(G4int i=0;i<14;i++){
		calorLogical_al[i] = new G4LogicalVolume(crys, mateAl, calName[0][i]);
		calorLogical_al[i]->SetVisAttributes(va[11]);
	}
	///////////Alminium Case Fin//////////////////////

	///////////////Inner Space///////////////////////
	dia = 61.5-Al_th*2;		//Distance of side to side
	hei = 78.0-Al_th*2;		//Height
	R = 70.0-Al_th*2;		//Diameter
	dhei = 12.0-Al_th;	//Height of cylinder part
	x = dia/sqrt(3);	//Length of one side
	y = (R-dia)/2;		//Width of cut
	h = hei-dhei;		//Height of cut
	dh = 5.0-Al_th;
	dy = (y/h)*(h+dh)-y;

	G4VSolid* tube_ispc = new G4Tubs("tube_ispc", 0.*mm, R/2.*mm, hei/2.*mm, 0.*deg, 360.*deg);
	G4VSolid* trd_ispc = new G4Trd("trd_ispc",x*mm, x*mm, 0.*mm, (y+dy)*mm,(h+dh)/2.*mm);

	cutvec = G4ThreeVector(0.*mm,-1.0*(R/2+dy)*mm,(hei-h-dh)/2*mm);
	cutvec.rotateZ(dphi);
	cutdeg.rotateZ(dphi);
	G4VSolid* crys_ispc = new G4SubtractionSolid("crys_ispc",tube_ispc,trd_ispc,G4Transform3D(cutdeg,cutvec));
	for(G4int i=1;i<6;i++){
		cutvec.rotateZ(dphi);
		cutdeg.rotateZ(dphi);
		crys_ispc = new G4SubtractionSolid("crys_ispc",crys_ispc,trd_ispc,G4Transform3D(cutdeg,cutvec));
	}

	for(G4int i=0;i<14;i++){
		calorLogical_in[i] = new G4LogicalVolume(crys_ispc, mateBea, calName[1][i]);
		calorLogical_in[i] -> SetVisAttributes(va[11]);
	}
	///////////////////Inner Space Fin/////////////////
	
	///////////////Insensitive Ge///////////////////////
	dia = 61.5-2.8;		//Distance of side to side
	hei = 78.0-2.8;		//Height
	R = 70.0-2.8;		//Diameter
	dhei = 12.0-1.4;	//Height of cylinder part
	x = dia/sqrt(3);	//Length of one side
	y = (R-dia)/2;		//Width of cut
	h = hei-dhei;		//Height of cut
	dh = 5.0-1.4;
	dy = (y/h)*(h+dh)-y;

	G4VSolid* tube_insens = new G4Tubs("tube_insens", 0.*mm, R/2.*mm, hei/2.*mm, 0.*deg, 360.*deg);
	G4VSolid* trd_insens = new G4Trd("trd_insens",x*mm, x*mm, 0.*mm, (y+dy)*mm,(h+dh)/2.*mm);

	cutvec = G4ThreeVector(0.*mm,-1.0*(R/2+dy)*mm,(hei-h-dh)/2*mm);
	cutvec.rotateZ(dphi);
	cutdeg.rotateZ(dphi);
	G4VSolid* crys_insens = new G4SubtractionSolid("crys_insens",tube_insens,trd_insens,G4Transform3D(cutdeg,cutvec));
	for(G4int i=1;i<6;i++){
		cutvec.rotateZ(dphi);
		cutdeg.rotateZ(dphi);
		crys_insens = new G4SubtractionSolid("crys_insens",crys_insens,trd_insens,G4Transform3D(cutdeg,cutvec));
	}

	for(G4int i=0;i<14;i++){
		calorLogical_insens[i] = new G4LogicalVolume(crys_insens, mateGe, calName[3][i]);
		calorLogical_insens[i] -> SetVisAttributes(va[11]);
	}
	///////////////////Insensitive Fin/////////////////

	///////////////////Ge Detector////////////////////
	dia = 61.5-2.8;  //Distance of side to side
	hei = 78.0-4.8;  //Height
	R = 70.0-2.8;    //Diameter
	dhei = 12.0-1.4; //Height of cylinder part
	x = dia/sqrt(3);     //Length of one side
	y = (R-dia)/2;       //Width of cut
	h = hei-dhei;	     //Height of cut
	dh = 5.0-1.4;
	dy = (y/h)*(h+dh)-y;
	r = 10.0;    //Diameter of cylinder cut out from Ge crystal
	l = 63.0;    //Height of cy;inder cut out from Ge crystal

	G4VSolid* tube_Ge = new G4Tubs("tube_Ge", 0.*mm, R/2.*mm, hei/2.*mm, 0.*deg, 360.*deg);
	G4VSolid* trd_Ge = new G4Trd("trd_Ge",x*mm, x*mm, 0.*mm, (y+dy)*mm,(h+dh)/2.*mm);

	cutvec = G4ThreeVector(0.*mm,-1.0*(R/2+dy)*mm,(hei-h-dh)/2*mm);
	cutvec.rotateZ(dphi);
	cutdeg.rotateZ(dphi);
	G4VSolid* crys_Ge = new G4SubtractionSolid("crys_Ge",tube_Ge,trd_Ge,G4Transform3D(cutdeg,cutvec));
	for(G4int i=0;i<6;i++){
		cutvec.rotateZ(dphi);
		cutdeg.rotateZ(dphi);
		crys_Ge = new G4SubtractionSolid("crys_Ge",crys_Ge,trd_Ge,G4Transform3D(cutdeg,cutvec));
	}

	//electrode hole
	G4VSolid* tube_cut = new G4Tubs("tube_cut", 0.*mm, r/2.*mm, l/2.*mm, 0.*deg, 360.*deg);
	cutvec = G4ThreeVector(0.*mm,0.*mm,((-hei+l)/2.)*mm);
	crys_Ge = new G4SubtractionSolid("crys_Ge",crys_Ge,tube_cut,G4Transform3D(zeror,cutvec));

	for(G4int i=0;i<14;i++){
		calorLogical_Ge[i] = new G4LogicalVolume(crys_Ge, mateGe, calName[2][i]);
		calorLogical_Ge[i] -> SetVisAttributes(va[1]);
	}
	////////////////Ge Detector Fin///////////////

	///////////Placement of Detectors/////////////
	//////////////////Variable////////////////////
	G4ThreeVector vec1;
	G4ThreeVector gevec = G4ThreeVector(0.*mm,0.*mm,-1.*mm);
	G4RotationMatrix rot1;
	G4double del = 2*(90.-(theta*180/M_PI))*deg;
	G4double A,B,C,D,E,F,H,Ly,Lz;
	A = 27.;		//Distance of cut from center
	B = 35.;		//Radius of cylinder
	C = 30.75;		//Distance of hexagonal side to other side
	D = 39.;		//Half the height of cylinder
	E = ((A+D)*C)/(B-C);	//From (A+D+E):B=E:C 
	F = D+E;		//Distance from rotation center to center of crystal
	H = F-173.;		//Distance from origin to center of crystal
	Ly = F*cos(del)-H;	//y-axis
	Lz = F*sin(del);	//z-axis
	

	for(G4int i=0;i<14;i++){
		if(i==0){
			vec1 = G4ThreeVector(0.,Ly*mm,Lz*mm);
			rot1 = G4RotationMatrix(0.,-90.*deg-del,0.);
			vec1.rotateY(120.*deg);
			rot1.rotateY(120.*deg);
		}else if(i<6){
			vec1.rotateY(dphi);
			rot1.rotateY(dphi);
		}else if(i==6){
			vec1 = G4ThreeVector(0.*mm,173.*mm,0.*mm);
			rot1 = G4RotationMatrix(0.,-90.*deg,0.);
		}else if(i==7){
			vec1 = G4ThreeVector(0.,-Ly*mm,Lz*mm);
			rot1 = G4RotationMatrix(0.,90.*deg+del,0.);
			vec1.rotateY(60.*deg);
			rot1.rotateY(60.*deg);
		}else if(i<13){
			vec1.rotateY(-dphi);
			rot1.rotateY(-dphi);
		}else{
			vec1 = G4ThreeVector(0.*mm,-173.*mm,0.*mm);
			rot1 = G4RotationMatrix(0.,90.*deg,0.);
		}

		calorPhysical_al[i] = new G4PVPlacement(G4Transform3D(rot1,vec1),calorLogical_al[i],calName[0][i],worldLV,false,i);
		calorPhysical_in[i] = new G4PVPlacement(zerot,calorLogical_in[i],calName[1][i],calorLogical_al[i],false,i);
		calorPhysical_insens[i] = new G4PVPlacement(zerot,calorLogical_insens[i],calName[3][i],calorLogical_in[i],false,i);
		calorPhysical_Ge[i] = new G4PVPlacement(G4Transform3D(zeror,gevec),calorLogical_Ge[i],calName[2][i],calorLogical_insens[i],false,i);
	}
	//===================================================================
	/////////////////////// Ge Detectors Fin ////////////////////////////
	//===================================================================

	//===================================================================
	////////////////////// BGO Scintillators ////////////////////////////
	// ==================================================================
	// Side BGO VETO Detector
	G4VSolid* BGOside_case = new G4Tubs("BGOside_case", 130.5*mm, 215.*mm, 135.*mm, 0.*deg, view_degree*deg);
	G4VSolid* BGOside_iner = new G4Tubs("BGOside_iner", 132.5*mm, 213.*mm, 133.*mm, 0.*deg, 360.*deg);
	G4VSolid* BGOside_insens = new G4Tubs("BGOside_insens", 134.5*mm, 189.5*mm, 125.*mm,0.*deg,29.*deg);
	G4VSolid* BGOside = new G4Tubs("BGOside", 137.5*mm, 186.5*mm, 122.*mm,1.*deg,27.*deg);	//insens 3mm
	G4VSolid* BGO_sideAl = new G4SubtractionSolid("BGO_sideAl",BGOside_case,BGOside_iner,zerot);
	
	G4LogicalVolume* SIDE_CASE = new G4LogicalVolume(BGO_sideAl, mateAl, "SIDE_CASE");

	for(G4int i=0;i<24;i++){
		calorLogical_sBGO_insens[i] = new G4LogicalVolume(BGOside_insens, mateBGO, sBGOname[0][i]);
		calorLogical_sBGO[i] = new G4LogicalVolume(BGOside, mateBGO, sBGOname[1][i]);
		calorLogical_sBGO_insens[i] -> SetVisAttributes(va[4]);
	}
	SIDE_CASE -> SetVisAttributes(va[0]);
	
	//Placement of Side BGO
	//Definition 
	G4ThreeVector vecSCup,vecSCdw;
	G4ThreeVector vecSBup,vecSBdw;
	G4RotationMatrix rotSCup,rotSCdw;
	G4RotationMatrix rotSBup,rotSBdw;

	vecSCup.setY(240.*mm);
	vecSCdw.setY(-240.*mm);
	rotSCup.rotateX(-90.*deg);
	rotSCdw.rotateX(90.*deg);
	new G4PVPlacement(G4Transform3D(rotSCup,vecSCup), SIDE_CASE, "SIDE_CASEup", worldLV, false,28);
	new G4PVPlacement(G4Transform3D(rotSCdw,vecSCdw), SIDE_CASE, "SIDE_CASEdw", worldLV, false,29);
	
	vecSBup.setY(232.5*mm);
	vecSBdw.setY(-232.5*mm);
	rotSBup.rotateX(90.*deg);
	rotSBdw.rotateX(90.*deg);
	for(G4int i=view_sBGOf;i<view_sBGOe;i++){
		//up
		rotSBup.rotateY(30.*deg);
		calorPhysical_sBGO_insens[i] = new G4PVPlacement(G4Transform3D(rotSBup,vecSBup),calorLogical_sBGO_insens[i], sBGOname[0][i], worldLV, false, i);
		calorPhysical_sBGO[i] = new G4PVPlacement(zerot,calorLogical_sBGO[i], sBGOname[1][i], calorLogical_sBGO_insens[i], false, i);
		//down
		rotSBdw.rotateY(-30.*deg);
		calorPhysical_sBGO_insens[i+12] = new G4PVPlacement(G4Transform3D(rotSBdw,vecSBdw),calorLogical_sBGO_insens[i+12], sBGOname[0][i+12], worldLV, false, i+12);
		calorPhysical_sBGO[i+12] = new G4PVPlacement(zerot,calorLogical_sBGO[i+12], sBGOname[1][i+12], calorLogical_sBGO_insens[i+12], false, i+12);
	}
	
	// Top BGO VETO Detector
	G4VSolid* BGOtop_case = new G4Tubs("BGOtop_case", 35.*mm, 130.*mm, 60.*mm, 0.*deg, view_degree*deg);
	G4VSolid* BGOtop_iner = new G4Tubs("BGOtop_iner", 37.*mm, 128.*mm, 58.*mm, 0.*deg, 360.*deg);
	G4VSolid* BGOtop_insens = new G4Tubs("BGOtop_insens", 37.5*mm, 127.5*mm, 50.*mm,0.*deg,43.5*deg);
	G4VSolid* BGOtop = new G4Tubs("BGOtop", 40.5*mm, 124.5*mm, 47.*mm,2.*deg,39.5*deg);	//insens 3mm
	G4VSolid* BGO_topAl = new G4SubtractionSolid("BGO_topAl",BGOtop_case,BGOtop_iner,zerot);
	
	G4LogicalVolume* TOP_CASE = new G4LogicalVolume(BGO_topAl, mateAl, "TOP_CASE");

	for(G4int i=0;i<16;i++){
		calorLogical_tBGO_insens[i] = new G4LogicalVolume(BGOtop_insens, mateBGO, tBGOname[0][i]);
		calorLogical_tBGO[i] = new G4LogicalVolume(BGOtop, mateBGO, tBGOname[1][i]);
		calorLogical_tBGO_insens[i] -> SetVisAttributes(va[4]);
	}
	TOP_CASE -> SetVisAttributes(va[0]);
	
	//Placement of Top BGO
	//Definition 
	G4ThreeVector vecTCup,vecTCdw;
	G4ThreeVector vecTBup,vecTBdw;
	G4RotationMatrix rotTCup,rotTCdw;
	G4RotationMatrix rotTBup,rotTBdw;

	vecTCup.setY(343.*mm);
	vecTCdw.setY(-343.*mm);
	rotTCup.rotateX(-90.*deg);	
	rotTCdw.rotateX(90.*deg);
	new G4PVPlacement(G4Transform3D(rotTCup,vecTCup),TOP_CASE,"TOP_CASEup", worldLV, false, 24);
	new G4PVPlacement(G4Transform3D(rotTCdw,vecTCdw),TOP_CASE,"TOP_CASEdw", worldLV, false, 25);	
	
	vecTBup.setY(335.*mm);
	vecTBdw.setY(-335.*mm);
	rotTBup.rotateX(-90.*deg);	
	rotTBdw.rotateX(90.*deg);
	for(G4int i=view_tBGOf;i<view_tBGOe;i++){
		//up
		rotTBup.rotateY(45.*deg);
		calorPhysical_tBGO_insens[i] = new G4PVPlacement(G4Transform3D(rotTBup,vecTBup),calorLogical_tBGO_insens[i],tBGOname[0][i], worldLV, false, i);
		new G4PVPlacement(zerot,calorLogical_tBGO[i],tBGOname[1][i], calorLogical_tBGO_insens[i], false, i);
		//down
		rotTBdw.rotateY(-45.*deg);
		new G4PVPlacement(G4Transform3D(rotTBdw,vecTBdw),calorLogical_tBGO_insens[i+8], tBGOname[0][i+8], worldLV, false, i+8);
		new G4PVPlacement(zerot,calorLogical_tBGO[i+8],tBGOname[1][i+8], calorLogical_tBGO_insens[i+8], false, i+8);
	}
	
	//====================================================================
	///////////////////// BGO Scintillators Fin //////////////////////////
	//====================================================================

	//===================================================================
	///////////////////////////// Plate /////////////////////////////////
	//===================================================================
	////////////////Variable/////////////////////
	G4ThreeVector lihv_up,lifv_up,lih2v_up;
	G4ThreeVector lihv_dw,lifv_dw,lih2v_dw;
	G4RotationMatrix lihr_up,lifr_up,lih2r_up;
	G4RotationMatrix lihr_dw,lifr_dw,lih2r_dw;
	G4double posLiH = 115.65;
	G4double posLiF = 99.;
	G4double posLiH2 = 81.5;

	G4ThreeVector alcasev_up,alcasev_dw;
	G4RotationMatrix alcaser_up,alcaser_dw;
	G4double posAlcase = 173.;
	///////////////////LiH Disk///////////////////
	G4VSolid* li_disk1 = new G4Tubs("li_disk1", 0.*mm, 92.4*mm, 8.65*mm,0.*deg,view_degree*deg);
	G4VSolid* al_disk1 = new G4Tubs("al_disk1", 0.*mm, 93.4*mm, 9.65*mm,0.*deg,view_degree*deg);
	G4LogicalVolume* Al_disk1 = new G4LogicalVolume(al_disk1, mateAl, "Al_disk1");
	G4LogicalVolume* Li_disk1 = new G4LogicalVolume(li_disk1, mateLiH, "Li_disk1");
	Al_disk1->SetVisAttributes(va[0]);
	Li_disk1->SetVisAttributes(va[5]);

	lihv_up.setY(posLiH*mm);
	lihv_dw.setY(-1*posLiH*mm);
	lihr_up.rotateX(-90.*deg);
	lihr_dw.rotateX(-270.*deg);
	new G4PVPlacement(G4Transform3D(lihr_up,lihv_up),Al_disk1, "AL_DISK1", worldLV, false, 0);
	new G4PVPlacement(zerot,Li_disk1, "Li_disk1", Al_disk1, false, 0);
	new G4PVPlacement(G4Transform3D(lihr_dw,lihv_dw),Al_disk1, "AL_DISK1", worldLV, false, 1);
		
	///////////////////LiF Disk///////////////////
	G4VSolid* Lidisk = new G4Tubs("Lidisk", 0.*mm, 158.*mm, 5.*mm,0.*deg,view_degree*deg);
	G4LogicalVolume* Li_disk = new G4LogicalVolume(Lidisk, mateLiF, "Li_disk");
	Li_disk-> SetVisAttributes(va[6]);

	lifv_up.setY(posLiF*mm);
	lifv_dw.setY(-1.*posLiF*mm);
	lifr_up.rotateX(-90.*deg);
	lifr_dw.rotateX(-270.*deg);
	new G4PVPlacement(G4Transform3D(lifr_up,lifv_up),Li_disk, "LiF_disk", worldLV, false, 0);
	new G4PVPlacement(G4Transform3D(lifr_dw,lifv_dw),Li_disk, "LiF_disk", worldLV, false, 1);

	///////////////////LiH Disk2//////////////////
	G4VSolid* disk2 = new G4Tubs("disk2", 0.*mm, 91.4*mm, 9.75*mm,0.*deg,view_degree*deg);
	G4VSolid* disk3 = new G4Tubs("disk3", 0.*mm, 92.4*mm, 10.75*mm,0.*deg,view_degree*deg);
	G4VSolid* coll = new G4Tubs("coll", 92.45*mm,160.*mm, 10.75*mm,0.*deg,view_degree*deg);
	G4LogicalVolume* LiHdisk2 = new G4LogicalVolume(disk2, mateLiH, "LiHdisk2");
	G4LogicalVolume* LiHdisk3 = new G4LogicalVolume(disk3, mateAl, "LiHdisk3");
	G4LogicalVolume* collimator1 = new G4LogicalVolume(coll, matePb, "collimator1");
	LiHdisk2-> SetVisAttributes(va[5]);
	LiHdisk3-> SetVisAttributes(va[0]);
	collimator1-> SetVisAttributes(va[7]);

	lih2v_up.setY(posLiH2*mm);
	lih2v_dw.setY(-1*posLiH2*mm);
	lih2r_up.rotateX(-90.*deg);
	lih2r_dw.rotateX(-270.*deg);
	new G4PVPlacement(G4Transform3D(lih2r_up,lih2v_up),collimator1, "collimator1", worldLV, false, 0);
	new G4PVPlacement(G4Transform3D(lih2r_up,lih2v_up),LiHdisk3, "AL", worldLV, false, 0);
	new G4PVPlacement(zerot,LiHdisk2, "LiH", LiHdisk3, false, 0);
	new G4PVPlacement(G4Transform3D(lih2r_dw,lih2v_dw),collimator1, "collimator1", worldLV, false, 1);
	new G4PVPlacement(G4Transform3D(lih2r_dw,lih2v_dw),LiHdisk3, "AL", worldLV, false, 1);

	/////////////////Alminium case////////////////
	G4VSolid* case_un = new G4Cons("case_un",95.*mm,100.*mm,114.3*mm,115.*mm,47.*mm,0.*deg,view_degree*deg);
	G4VSolid* cover = new G4Tubs("cover",0.*mm,100*mm,0.35*mm,0.*deg,view_degree*deg);
	G4VSolid* side = new G4Tubs("side",110.*mm,115.*mm,27.25*mm,0.*deg,view_degree*deg);
	G4VSolid* Alcover = new G4UnionSolid("Alcover",case_un,cover,0,G4ThreeVector(0.*mm,0.*mm,-47.*mm));
	Alcover = new G4UnionSolid("Alcover",Alcover,side,0,G4ThreeVector(0.*mm,0.*mm,(47.+27.25)*mm));
	G4LogicalVolume* Alcase = new G4LogicalVolume(Alcover, mateAl, "Alcase");
	Alcase-> SetVisAttributes(va[0]);

	alcasev_up.setY(posAlcase*mm);
	alcasev_dw.setY(-posAlcase*mm);
	alcaser_up.rotateX(-90.*deg);
	alcaser_dw.rotateX(90.*deg);
	new G4PVPlacement(G4Transform3D(alcaser_up,alcasev_up),Alcase, "Alcover", worldLV, false, 0);
	new G4PVPlacement(G4Transform3D(alcaser_dw,alcasev_dw),Alcase, "Alcover", worldLV, false, 1);
	//===================================================================
	/////////////////////////// Plate Fin ///////////////////////////////
	//===================================================================

	//===================================================================
	////////////////////////// Back Space //////////////////////////////
	//===================================================================
	////////////////Variable/////////////////////
	G4ThreeVector Albackv_up,Albackv_dw;
	G4ThreeVector Cubackv_up,Cubackv_dw;
	G4RotationMatrix Albackr_up,Albackr_dw;
	G4RotationMatrix Cubackr_up,Cubackr_dw;
	G4double posAlback=217.5;
	G4double posCuback=237.;

	//Backspace (Al and Cu)
	G4VSolid* parts1 = new G4Tubs("parts1",0.*mm,108.*mm,2.5*mm,0.*deg,view_degree*deg);
	G4VSolid* parts2 = new G4Tubs("parts2",0.*mm,35.*mm,3.5*mm,0.*deg,view_degree*deg);
	G4VSolid* parts3 = new G4Tubs("parts3",0.*mm,15.25*mm,3.*mm,0.*deg,view_degree*deg);
	G4VSolid* parts4 = new G4Tubs("parts4",31.*mm,35.*mm,3.5*mm,0.*deg,view_degree*deg);
	G4VSolid* parts5 = new G4Tubs("parts5",15.25*mm,115.*mm,3.5*mm,0.*deg,view_degree*deg);
	G4VSolid* parts6 = new G4Tubs("parts6",0.*mm,15.25*mm,4.5*mm,0.*deg,view_degree*deg);
	G4VSolid* parts7 = new G4Tubs("parts7",0.*mm,7.25*mm,40.*mm,0.*deg,view_degree*deg);
	G4VSolid* backAl = new G4UnionSolid("backAl",parts1,parts2,0,G4ThreeVector(0.*mm,0.*mm,6.*mm));
	backAl = new G4UnionSolid("backAl",backAl,parts3,0,G4ThreeVector(0.*mm,0.*mm,12.*mm));
	backAl = new G4UnionSolid("backAl",backAl,parts4,0,G4ThreeVector(0.*mm,0.*mm,46.5*mm));
	backAl = new G4UnionSolid("backAl",backAl,parts5,0,G4ThreeVector(0.*mm,0.*mm,53.5*mm));
	G4VSolid* backCu = new G4UnionSolid("backCu",parts6,parts7,0,G4ThreeVector(0.*mm,0.*mm,39.5*mm));
	
	G4LogicalVolume* back1 = new G4LogicalVolume(backAl,mateAl,"backal");
	G4LogicalVolume* back2 = new G4LogicalVolume(backCu,mateCu,"backcu");
	back2 -> SetVisAttributes(va[8]);
	back1 -> SetVisAttributes(va[0]);
	
	//placement of back space(Al)
	Albackv_up.setY(posAlback*mm);
	Albackv_dw.setY(-posAlback*mm);
	Albackr_up.rotateX(-90.*deg);
	Albackr_dw.rotateX(-270.*deg);
	new G4PVPlacement(G4Transform3D(Albackr_up,Albackv_up),back1,"back1",worldLV,false,0);
	new G4PVPlacement(G4Transform3D(Albackr_dw,Albackv_dw),back1,"back1",worldLV,false,0);
	
	//placement of bak space(Cu)
	Cubackv_up.setY(posCuback*mm);
	Cubackv_dw.setY(-posCuback*mm);
	Cubackr_up.rotateX(-90*deg);
	Cubackr_dw.rotateX(-270.*deg);
	new G4PVPlacement(G4Transform3D(Cubackr_up,Cubackv_up),back2,"back2",worldLV,false,0);
	new G4PVPlacement(G4Transform3D(Cubackr_dw,Cubackv_dw),back2,"back2",worldLV,false,0);

	//===================================================================
	////////////////////// Back Space Fin ///////////////////////////////
	//===================================================================


	//===================================================================
	/////////////////////////// Beam Duct //////////////////////////////
	//===================================================================
	////////////////Variable/////////////////////
	G4String beam_ver;
	G4double length = 215;
	G4double ty = 108/2.;	//yamada ver. 108/2 mm
	G4double tx = 92./2.;	//yamada ver. 92/2 mm
	G4double dt_Al = 3.0;  	//thickness of duct
	G4double dt_Li = 10.0;
//	G4double dt_Al = 2.0;  	//thickness of duct
//	G4double dt_Li = 7.9;
	
	G4VSolid *Cut_Duct;
	G4LogicalVolume *beamduct_Al;
	G4VSolid *outer_Al;
	G4VSolid *inner_Al;
	G4VSolid *duct_Al;
	G4VSolid *dduct_Al;
	G4LogicalVolume *beamduct_Li;
	G4VSolid *outer_Li;
	G4VSolid *inner_Li;
	G4VSolid *duct_Li;
	G4VSolid *dduct_Li;

	switch(BEAM_DUCT){
		case 1:
			/////////////////2013B0025///////////////////
			////////Beam duct (for natural Gd B0025)/////
			/////////////////////////////////////////////
			outer_Al = new G4Box("outer_Al",length*mm,ty*mm,tx*mm);
			Cut_Duct = new G4Box("Cut_Duct",(length+1)*mm,view_Ducty*mm,view_Ductx*mm);
			inner_Al = new G4Box("inner_Al",length*mm,(ty-dt_Al)*mm,(tx-dt_Al)*mm);
			duct_Al = new G4SubtractionSolid("duct_Al",outer_Al,inner_Al,zerot);
			dduct_Al = new G4SubtractionSolid("dduct_Al",duct_Al,Cut_Duct,0,G4ThreeVector(0,0,tx/2.*mm));
			beamduct_Al= new G4LogicalVolume(dduct_Al, mateAl, "beamduct_Al");
			beamduct_Al-> SetVisAttributes(va[0]);

			// placement of the beam duct
			new G4PVPlacement(zerot,beamduct_Al, "beamduct_Al", worldLV, false, 17);
			beam_ver="2013B0025(Al)";
			break;

		case 2:
			////////////////////2014B0124/////////////////////
			/////////new Beam duct (for enriched Gd B0124)////
			//////////////////////////////////////////////////
			outer_Al = new G4Box("outer_Al",length*mm,ty*mm,tx*mm);
			Cut_Duct = new G4Box("Cut_Duct",(length+1)*mm,view_Ducty*mm,view_Ductx*mm);
			inner_Al = new G4Box("inner_Al",length+0.1*mm,(ty-dt_Al)*mm,(tx-dt_Al)*mm);
			duct_Al = new G4SubtractionSolid("duct_Al",outer_Al,inner_Al,zerot);
			dduct_Al = new G4SubtractionSolid("dduct_Al",duct_Al,Cut_Duct,0,G4ThreeVector(0,0,tx/2.*mm));
			outer_Li = new G4Box("outer_Li",length*mm,(ty-dt_Al)*mm,(tx-dt_Al)*mm);
			inner_Li = new G4Box("inner_Li",length+0.1*mm,(ty-dt_Al-dt_Li)*mm,(tx-dt_Al-dt_Li)*mm);
			duct_Li = new G4SubtractionSolid("duct_Li",outer_Li,inner_Li,zerot);
			dduct_Li = new G4SubtractionSolid("dduct_Li",duct_Li,Cut_Duct,0,G4ThreeVector(0,0,tx/2.*mm));
			
			beamduct_Al= new G4LogicalVolume(dduct_Al, mateAl, "beamduct_Al");
			beamduct_Li= new G4LogicalVolume(dduct_Li, mateLiF, "beamduct_Li");
			beamduct_Al-> SetVisAttributes(va[0]);
			beamduct_Li-> SetVisAttributes(va[6]);

			// placement of the beam duct
			new G4PVPlacement(zerot,beamduct_Al, "beamduct_Al", worldLV, false,17);
			new G4PVPlacement(zerot,beamduct_Li, "beamduct_Li", worldLV, false,18);
			beam_ver="2014B0124(Al+LiF)";
			break;

		default:
			Printing.Error(File_name,"BEAM_DUCT");
			break;
	}
	//=======================================================================
	//////////////////////////// Beam Duct Fin //////////////////////////////
	//=======================================================================


	//==============================================================
	///////////////////////////// Target ///////////////////////////
	//==============================================================
	///////target holder//////////
	////////////////Variable/////////////////////
	G4ThreeVector tav = G4ThreeVector(HOLDER_POSX*mm,HOLDER_POSY*mm,HOLDER_POSZ*mm);		//for 2014B0124
	
	G4VSolid* holder1 = new G4Box("holder1", 3.5*mm, 27.*mm, 25.*mm);
	G4VSolid* holder1_1 = new G4Box("holder1_1", 3.6*mm, 22.*mm, 22.*mm);
	G4VSolid* holder2 = new G4Box("holder2", 2.*mm, 3.*mm, 21.*mm);
	G4VSolid* holder3 = new G4Box("holder3", 6.5*mm, 1.*mm, 21.*mm);
	G4VSolid* holder = new G4SubtractionSolid("holder",holder1,holder1_1,zerot);
	holder = new G4UnionSolid("holder",holder,holder2,0,G4ThreeVector(0.*mm,-30.*mm,0.*mm));
	holder = new G4UnionSolid("holder",holder,holder3,0,G4ThreeVector(0.*mm,-34.*mm,0.*mm));
	
	G4LogicalVolume* tholder= new G4LogicalVolume(holder, mateTef, "tholder");
	tholder-> SetVisAttributes(va[2]);
	
	new G4PVPlacement(G4Transform3D(zeror,tav),tholder,"tholder",worldLV,false,0);

	///////////Target///////////
	////////////////Variable/////////////////////
	G4String target_ver;
	G4ThreeVector caseV = G4ThreeVector(SOURCE_POSX*mm,SOURCE_POSY*mm,SOURCE_POSZ*mm);
	G4ThreeVector casv = G4ThreeVector(1.75*mm,0.*mm,0*mm);
	G4RotationMatrix casr = G4RotationMatrix(0.*deg,90.*deg,90.*deg);
	G4VSolid *foilSolid;
	G4VSolid *foilSolid2;
	G4VSolid *foilSolid3;
	G4LogicalVolume *foilLV;
	G4LogicalVolume *foilLV2;
	G4LogicalVolume *foilLV3;

	switch(TARGET){
		case 1:
			//Gd foil
			switch(Gd_CAPTURE){
				case 1:
					mate = G4Material::GetMaterial("Gdfoil");
					break;
				case 2:
					mate = G4Material::GetMaterial("eGd157foil");
					break;
				case 3:
					mate = G4Material::GetMaterial("eGd155foil");
					break;
				default:
					Printing.Error(File_name,"Gd_CAPTURE");
					break;

			}
			foilSolid = new G4Box("foil", .01*mm, .5*cm, .5*cm);
			foilLV = new G4LogicalVolume(foilSolid, mate, "foil");
			foilLV-> SetVisAttributes(va[3]);
		
			new G4PVPlacement(G4Transform3D(zeror,caseV), foilLV, "foil", worldLV, false, 0);
			target_ver="Gd";
			break;

		case 2:
			//Cl foil
			foilSolid = new G4Box("foil", .01*mm, .5*cm, .5*cm);
			foilLV = new G4LogicalVolume(foilSolid, mateCl, "foil");
			foilLV-> SetVisAttributes(va[3]);
			
			new G4PVPlacement(G4Transform3D(zeror,caseV), foilLV, "foil", worldLV, false, 0);
			target_ver="NaCl";
			break;

		case 3:
			//Eu case
			foilSolid = new G4Tubs("foilSolid",10.*mm,12.5*mm,2.*mm,0.*deg,360.*deg);
			foilSolid2 = new G4Tubs("foilSolid2",0.*mm,10.*mm,1.5*mm,0.*deg,360.*deg);
			foilSolid3 = new G4Tubs("foilSolid3",0.*mm,10.*mm,0.25*mm,0.*deg,360.*deg);
			
			foilLV = new G4LogicalVolume(foilSolid,mateDur,"foilLV");
			foilLV2 = new G4LogicalVolume(foilSolid2,mateAcr,"foilLV2");
			foilLV3 = new G4LogicalVolume(foilSolid3,mateDur,"foilLV3");
			foilLV->SetVisAttributes(va[10]);
			foilLV2->SetVisAttributes(va[9]);
			foilLV3->SetVisAttributes(va[10]);
			
			new G4PVPlacement(G4Transform3D(casr,caseV),foilLV,"foilLV",worldLV,false,0);
			new G4PVPlacement(G4Transform3D(casr,caseV),foilLV2,"foilLV2",worldLV,false,0);
			new G4PVPlacement(G4Transform3D(casr,caseV+casv),foilLV3,"foilLV3",worldLV,false,0);
			new G4PVPlacement(G4Transform3D(casr,caseV-casv),foilLV3,"foilLV3",worldLV,false,1);
			target_ver="Eu";
			break;

		case 4:
			//Co,Cs,Na case
			foilSolid = new G4Tubs("soilSolid",0.*mm,12.5*mm,3.*mm,0.*deg,360.*deg);
			foilLV = new G4LogicalVolume(foilSolid,mateAcr,"foilLV");
			foilLV->SetVisAttributes(va[9]);
			
			new G4PVPlacement(G4Transform3D(casr,caseV),foilLV,"foilLV",worldLV,false,0);	
			target_ver="Co_Na_Cs";
			break;

		case 5:
			target_ver = "None";
			break;

		default :
			Printing.Error(File_name,"TARGET");
			break;

	}
	//===================================================================
	/////////////////////////// Target Fin //////////////////////////////
	//===================================================================
	
	//===================================================================
	////////////////// Define Senstive Detector /////////////////////////
	// ==================================================================
	G4SDManager* SDmanager = G4SDManager::GetSDMpointer();
	CalorimeterSD* calsd = new CalorimeterSD("/calorimeter");
	BGOSD* sidesd = new BGOSD("BGOside");
	BGOSD* topsd  = new BGOSD("BGOtop");
	SDmanager-> AddNewDetector(calsd);
	SDmanager-> AddNewDetector(sidesd);
	SDmanager-> AddNewDetector(topsd);
	for(G4int i=0;i<14;i++){
		calorLogical_Ge[i]-> SetSensitiveDetector(calsd);
	}
	for(G4int i=0;i<24;i++){
		calorLogical_sBGO[i]-> SetSensitiveDetector(sidesd);
	}
	for(G4int i=0;i<16;i++){
		calorLogical_tBGO[i]-> SetSensitiveDetector(topsd);
	}
//	SIDE-> SetSensitiveDetector(sidesd);
//	TOP-> SetSensitiveDetector(topsd);
	//=====================================================================
	////////////////// Define Senstive Detector Fin ///////////////////////
	//=====================================================================
	
	return world;

}
