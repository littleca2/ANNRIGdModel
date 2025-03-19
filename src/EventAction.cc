// ====================================================================
//
//   EventAction.cc
//
// ====================================================================
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4Trajectory.hh"

#include "EventAction.hh"
#include "CalorimeterSD.hh"
#include "BGOSD.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TNtuple.h"
#include "TTree.h"

#define File_name "EventAction.cc"
G4int color_for_dat[14];
G4int color_for_sbgo[24];
G4int color_for_tbgo[16];
extern G4int INFO;	
extern G4int VETO;
extern G4double BGO_thr;
extern G4double Ene_thr;
extern G4int NumGamma;
extern G4double GammaEnergies[15];
//////////////////////////
EventAction::EventAction()
	//////////////////////////
{
}

///////////////////////////
EventAction::~EventAction()
	///////////////////////////
{
}

////////////////////////////////////////////////////////////
void EventAction::BeginOfEventAction(const G4Event* anEvent)
	////////////////////////////////////////////////////////////
{
	for(G4int i=0;i<14;i++) color_for_dat[i]=0;
	for(G4int j=0;j<24;j++) color_for_sbgo[j]=0;
	for(G4int j=0;j<16;j++) color_for_tbgo[j]=0;
	
//Normarization
NumGamma = 0;
for(G4int i=0;i<15;i++) GammaEnergies[i]=0;

	if((anEvent->GetEventID())==0){
		G4cout<<"=========Start Simulating========="<<G4endl;
	}
}

//////////////////////////////////////////////////////////
void EventAction::EndOfEventAction(const G4Event* anEvent)
	//////////////////////////////////////////////////////////
{	
	G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	G4int GdParentID = -99;
	G4Trajectory* trj = NULL;
	G4int NGammaOfEvent=0;
	G4int NGammaOfEvent_above_2MeV = 0;

	if (trajectoryContainer){
		n_trajectories = trajectoryContainer->entries();
	}	

	// Find the residual Gd nuclei after Gd(n,g) reaction.
	for (G4int i=0; i<n_trajectories; i++){
		trj = (G4Trajectory*) ((*(anEvent->GetTrajectoryContainer()))[i]);
		if (strstr(trj->GetParticleName(),"Gd15")){
			GdParentID = trj->GetParentID();
		}
	}
	// Find the Gamma after Gd(n,g) reaction.
	for (G4int i=0; i<n_trajectories; i++){
		trj = (G4Trajectory*) ((*(anEvent->GetTrajectoryContainer()))[i]);
		if ( trj->GetParticleName()=="gamma" && trj->GetParentID()==GdParentID){
			NGammaOfEvent++;
			if (trj->GetInitialKineticEnergy()>0.2){
				NGammaOfEvent_above_2MeV++;
			}
		}
	}

	// get "Hit Collection of This Event"
	G4int idcal = -1;
	G4int idbgo = -1;
	G4int idtop = -1;
	G4HCofThisEvent* HCTE = anEvent->GetHCofThisEvent();
	if(!HCTE) return;

	G4SDManager* SDManager= G4SDManager::GetSDMpointer();
	if( idcal<0 )  idcal = SDManager-> GetCollectionID("calorimeter");
	if( idbgo<0 )  idbgo = SDManager-> GetCollectionID("BGOside");
	if( idtop<0 )  idtop = SDManager-> GetCollectionID("BGOtop");

	CalHitsCollection* hccal= (CalHitsCollection*)HCTE-> GetHC(idcal);
	BGOHitsCollection* hcbgo= (BGOHitsCollection*)HCTE-> GetHC(idbgo);
	BGOHitsCollection* hcbgotop= (BGOHitsCollection*)HCTE-> GetHC(idtop);
//	if (!hccal && !hcbgo && !hcbgotop) return; // no hit collection
//	if (!hccal) return; // no hit collection


	//create array
	G4double data[14] = {0.0};
	G4double BGOs[24] = {0.0};
	G4double BGOt[16] = {0.0};
	G4double up = 0.0;
	G4double dw = 0.0;
	G4int bit = 0;
G4int NGammaOri = 0;
G4double GammaEneOri[15] = {0.0};

	//set Branch address
	TTree* tree = (TTree*)gROOT->FindObject("tree");
	tree->SetBranchAddress("Ge",data);
	tree->SetBranchAddress("BGOs",BGOs);
	tree->SetBranchAddress("BGOt",BGOt);
	tree->SetBranchAddress("up",&up);
	tree->SetBranchAddress("dw",&dw);
	tree->SetBranchAddress("bit",&bit);
	tree->SetBranchAddress("NGamma",&NGammaOfEvent);
	tree->SetBranchAddress("NGamma_above_2MeV",&NGammaOfEvent_above_2MeV);
tree->SetBranchAddress("NGammaOri",&NGammaOri);
tree->SetBranchAddress("GammaEneOri",GammaEneOri);

TTree* tre = (TTree*)gROOT->FindObject("tre");
tre->SetBranchAddress("NGammaOri",&NGammaOri);
tre->SetBranchAddress("GammaEneOri",GammaEneOri);

NGammaOri = NumGamma;
for (G4int i=0;i<15;i++) GammaEneOri[i] = GammaEnergies[i];

tre->Fill();

	//====================================
	//////////Side BGO Detector///////////
	//====================================
	//Initialization
	G4double sbgo_edep = 0.0;
	G4int sbgo_ich = 0;
	G4int sbgo_flag_up = 0;
	G4int sbgo_flag_low = 0;

	G4int sbgo_nhits = hcbgo-> entries();
	for(G4int idx=0; idx<sbgo_nhits; idx++) {
		sbgo_edep = 0.;

		//Get information of BGO detectors
		sbgo_ich= (*hcbgo)[idx]-> GetID();
		sbgo_edep= (*hcbgo)[idx]-> GetEdep();

		//Input the data
		BGOs[sbgo_ich] = sbgo_edep*1000.;
		if(sbgo_edep*1000.>BGO_thr){
			color_for_sbgo[sbgo_ich]=31;
			if(sbgo_ich<12) sbgo_flag_up++;
			else sbgo_flag_low++;
		}
	}
	//====================================
	////////////Top BGO Detector//////////
	//====================================
	//Initialization
	G4double tbgo_edep = 0.0;
	G4int tbgo_ich = 0;
	G4int tbgo_flag_up = 0;
	G4int tbgo_flag_low = 0;

	G4int tbgo_nhits = hcbgotop-> entries();
	for(G4int idx=0; idx<tbgo_nhits; idx++) {
		tbgo_edep = 0.;

		//Get information of BGO detectors
		tbgo_ich= (*hcbgotop)[idx]-> GetID();
		tbgo_edep= (*hcbgotop)[idx]-> GetEdep();

		//Input the data
		BGOt[tbgo_ich] = tbgo_edep*1000.;
		if(tbgo_edep*1000.>BGO_thr){
			color_for_tbgo[tbgo_ich]=31;
			if(tbgo_ich<8) tbgo_flag_up++;
			else tbgo_flag_low++;
		}
	}


	//====================================
	////////////Ge Calorometer////////////
	//====================================
	//Initialization
	G4double ge_edep = 0.0;
	G4int ge_ich = 0;
	G4int ge_flag_up = 0;
	G4int ge_flag_low = 0;

	G4int ge_nhits = hccal-> entries();
	for(G4int idx=0; idx<ge_nhits; idx++) {
		ge_edep = 0.;
		//Get information of Ge crystals
		ge_ich= (*hccal)[idx]->GetID();
		ge_edep= (*hccal)[idx]->GetEdep();

		//Input the data
		if(ge_edep*1000>Ene_thr){
			if(VETO==1){
				if(ge_ich<7){
					if(sbgo_flag_up>0||tbgo_flag_up>0){
						data[ge_ich]=0;
					}else{
						data[ge_ich] = ge_edep*1000;
					}	
				}else if(ge_ich>=7){
					if(sbgo_flag_low>0||tbgo_flag_low>0){
						data[ge_ich]=0;
					}else{
						data[ge_ich] = ge_edep*1000;
					}
				}
			}else if(VETO==2){
				data[ge_ich] = ge_edep*1000;
			}else{
				Printing.Error(File_name,"VETO");
			}
		}

		if(data[ge_ich]>Ene_thr){
			color_for_dat[ge_ich]=31;
			bit += pow(2,13-ge_ich);
			if(ge_ich<7){
				ge_flag_up++;
				up += data[ge_ich];
			}else{
				ge_flag_low++;
				dw += data[ge_ich];
			}
		}
	}

	//fill
	if(ge_flag_up>0||ge_flag_low>0){
		tree->Fill();
	}

	switch(INFO){
		case 1:
			G4cout<<"====================INFORMATION 1========================="<<G4endl;
			G4cout<<"######## Information of Particles ########"<<G4endl;
			PrintMain(anEvent);
			for(G4int i=0; i<n_trajectories; i++){
				trj = (G4Trajectory*) ((*(anEvent->GetTrajectoryContainer()))[i]);
				PrintTrajectory(trj);
			}
			PrintHitCrystal(data);
			PrintHitBGO(BGOs,BGOt);
			break;

		case 2:
			G4cout<<"====================INFORMATION 2========================="<<G4endl;
			G4cout<<"######## Information of Particles ########"<<G4endl;
			for(G4int i=0; i<n_trajectories; i++){
				trj = (G4Trajectory*) ((*(anEvent->GetTrajectoryContainer()))[i]);
				PrintTrajectory(trj);
			}
			break;

		case 3:
			G4cout<<"====================INFORMATION 3========================="<<G4endl;
			PrintHitCrystal(data);
			PrintHitBGO(BGOs, BGOt);
			break;

		case 4:
			if(((anEvent->GetEventID())%10000)==0){
				std::cerr <<" Current Event No. is "<< anEvent->GetEventID()<< "\r";
			}
			break;

		default: 
			Printing.Error(File_name,"INFO");
			break;
	}
}

//////////////////////////////////////////////////////////
// Print out the main information.
void EventAction::PrintMain(const G4Event* event)
	//////////////////////////////////////////////////////////
{
	G4PrimaryVertex* pVertex = event->GetPrimaryVertex();
	G4cout<<" Current Event No. is "<< event->GetEventID()<<G4endl;
	G4cout<<"** This event have "<< (pVertex->GetNumberOfParticle()) << " primary vertex." << G4endl;
}

//////////////////////////////////////////////////////////
// Print out the information of given trajectory.
void EventAction::PrintTrajectory(const G4Trajectory* trj)
	//////////////////////////////////////////////////////////
{	
	G4cout<< "ID: "<<setfill('0')<<setw(2)<<trj->GetTrackID();
	G4cout<< ", Name: "<<setfill('*')<<setw(10)<<trj->GetParticleName();
	G4cout<< ", PID: "<<setfill('0')<<setw(2)<<trj->GetParentID();
	G4cout<< ", P = " <<setprecision(2)<<setiosflags(ios::fixed)<<setw(3)<<right<<trj->GetInitialMomentum().mag()<<"[MeV]";
	G4cout<< ", X = "<<setprecision(1)<<setiosflags(ios::fixed)<<right<<trj->GetPoint(0)->GetPosition();
	G4cout<< G4endl;
}

//////////////////////////////////////////////////////////
// Print out the information of hit crystals.
void EventAction::PrintHitCrystal(const G4double* data)
	//////////////////////////////////////////////////////////
{
	G4cout<<"######## Information of hit crystals ########"<<G4endl;
	G4cout<<"////Ge crystals////"<<G4endl;
	G4cout<<"upper side : ";
	for(G4int i=0;i<7;i++){
		G4cout<<Form("\x1b[%dm",color_for_dat[i])<<setprecision(0)<<setiosflags(ios::fixed)<<setfill('0')<<setw(4)<<data[i]<<":";
	}
	G4cout<<"\x1b[0m"<<" [keV]"<<G4endl;
	G4cout<<"lower side : ";
	for(G4int i=7;i<14;i++){
		G4cout<<Form("\x1b[%dm",color_for_dat[i])<<setprecision(0)<<setiosflags(ios::fixed)<<setfill('0')<<setw(4)<<data[i]<<":";
	}
	G4cout<<"\x1b[0m"<<" [keV]"<<G4endl;
}

//////////////////////////////////////////////////////////
// Print out the information of hit BGO.
void EventAction::PrintHitBGO(const G4double* BGOs, const G4double* BGOt)
	//////////////////////////////////////////////////////////
{
	G4cout<<"////BGO crystals////"<<G4endl;
	G4cout<<"BGO upper side : ";
	for(G4int i=0;i<12;i++){
		G4cout<<Form("\x1b[%dm",color_for_sbgo[i])<<setprecision(0)<<setiosflags(ios::fixed)<<setfill('0')<<setw(4)<<BGOs[i]<<":";
	}
	G4cout<<"\x1b[0m"<<" [keV]"<<G4endl;
	G4cout<<"BGO lower side : ";
	for(G4int i=12;i<24;i++){
		G4cout<<Form("\x1b[%dm",color_for_sbgo[i])<<setprecision(0)<<setiosflags(ios::fixed)<<setfill('0')<<setw(4)<<BGOs[i]<<":";
	}
	G4cout<<"\x1b[0m"<<" [keV]"<<G4endl;
	G4cout<<"BGO upper top : ";
	for(G4int i=0;i<8;i++){
		G4cout<<Form("\x1b[%dm",color_for_tbgo[i])<<setprecision(0)<<setiosflags(ios::fixed)<<setfill('0')<<setw(4)<<BGOt[i]<<":";
	}
	G4cout<<"\x1b[0m"<<" [keV]"<<G4endl;
	G4cout<<"BGO lower top : ";
	for(G4int i=8;i<16;i++){
		G4cout<<Form("\x1b[%dm",color_for_tbgo[i])<<setprecision(0)<<setiosflags(ios::fixed)<<setfill('0')<<setw(4)<<BGOt[i]<<":";
	}
	G4cout<<"\x1b[0m"<<" [keV]"<<G4endl;
}
