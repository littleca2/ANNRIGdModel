// ====================================================================
//
//   RunAction.cc
//
// ====================================================================
#include <stdio.h>
#include <string.h>

#include "G4Run.hh"
#include "RunAction.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "fstream"

// Juan David Cortes, July 2, 2025, I'm doing this to see if IonTable() types will finally be filled with Gd
#include "G4IonTable.hh"

extern std::ofstream outf;
extern G4String RECREATE_FILE_NAME;
//////////////////////
RunAction::RunAction()
	//////////////////////
{
	// initialize root
	//gROOT-> Reset();

	//define 
	tree = new TTree("tree","");
	Double_t Ge[14];
	Double_t BGOside[24];
	Double_t BGOtop[16];
	Double_t up,dw;
	Int_t bit;
	Int_t NGamma;
	Int_t NGamma_above_2MeV;
Int_t NGammaOri;
Double_t GammaEneOri[15];
	tree -> Branch("Ge",Ge,"Ge[14]/D");
	tree -> Branch("BGOs",BGOside,"BGOside[24]/D");
	tree -> Branch("BGOt",BGOtop,"BGOtop[16]/D");
	tree -> Branch("up",&up,"up/D");
	tree -> Branch("dw",&dw,"dw/D");
	tree -> Branch("bit",&bit,"bit/I");
	tree -> Branch("NGamma",&NGamma,"NGamma/I");
	tree -> Branch("NGamma_above_2MeV",&NGamma_above_2MeV,"NGamma_above_2MeV/I");
tree -> Branch("NGammaOri",&NGammaOri,"NGammaOri/I");
tree -> Branch("GammaEneOri",GammaEneOri,"GammaEneOri[15]/D");

tre = new TTree("tre","");
tre->Branch("NGammaOri",&NGammaOri,"NGammaOri/I");
tre->Branch("GammaEneOri",GammaEneOri,"GammaEneOri[15]/D");
}

///////////////////////
RunAction::~RunAction()
	///////////////////////
{
	delete tree;
delete tre;
}

///////////////////////////////////////////////////
//void RunAction::BeginOfRunAction(const G4Run* aRun)
void RunAction::BeginOfRunAction(const G4Run*)
	///////////////////////////////////////////////////
{

// Juan David Cortes, July 2, 2025, I'm doing this to see if IonTable() types will finally be filled with Gd
G4IonTable::GetIonTable()->CreateAllIon();

//	outf.open("test.dat");
	tree-> Reset();
tre->Reset();
}

/////////////////////////////////////////////////
//void RunAction::EndOfRunAction(const G4Run* aRun)
void RunAction::EndOfRunAction(const G4Run*)
	/////////////////////////////////////////////////
{
	TFile* file = new TFile(RECREATE_FILE_NAME.c_str(),"RECREATE", "Geant4 ROOT analysis");
	file->SetCompressionLevel(2);
	tree->Write();
	file-> Close();
	delete file;
//	outf.close();

TFile* f2 = new TFile("OriginalGamma.root","RECREATE");
f2->SetCompressionLevel(2);
tre->Write();
f2->Close();
delete f2;
}
