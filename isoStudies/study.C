#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"


#include <iostream>
#include <vector>
#include <string>

#include "babyReader.h"
#include "Efficiency.h"
#include "common.h"
#include "Merge.h"

using namespace std;


babyEvent myEvent;


int main(){
  vector<string> datasets = {"T2tt_850_100"};//,"",""}

  //vector<Effiency> eff;

  TFile* fout = new TFile("isoResults.root","RECREATE");

  //read files
  for(unsigned int d=0;d<datasets.size();d++){
  	string filename = string(FOLDER_BABYTUPLES)+ datasets[d] + ".root";
  	TFile* f  = TFile::Open(filename.c_str(),"READ");
        TTree* theTree = (TTree*) f->Get("babyTuple");
	cout<<"Tree loaded "<<endl;
	InitializeBranchesForReading(theTree,&myEvent);
  	cout<<"Branches inialized"<<endl;
        //Fil efficiency vector
        Efficiency eff_mu_miniIso("miniIso",datasets[d], "muon", 0.1);
        Efficiency eff_mu_relIsoDB("relIsoDB",datasets[d], "muon", 0.15);
        Efficiency eff_el_miniIso("miniIso",datasets[d], "electron", 0.2);
        Efficiency eff_el_relIsoDB("relIsoDB",datasets[d], "electron", 0.15);
        Efficiency eff_el_relIsoEA("relIsoEA",datasets[d], "electron", 0.15);
	
	/* //useless
	eff.push_back(eff_mu_miniIso);
	eff.push_back(eff_mu_relIsoDB):
	eff.push_back(eff_el_miniIso);
	eff.push_back(eff_el_relIsoDB);
	eff.push_back(eff_el_relIsoEA);
	*/
	//loop over events
        int nEntries = theTree->GetEntries();
	cout<<"will run over "<<nEntries<<" events"<<endl;
        for (int i = 0 ; i < nEntries ; i++)
        {
           if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,datasets[d]);

           // Get the i-th entry
           ReadEvent(theTree,i,&myEvent);
	   
	   //selection
           if (myEvent.numberOfSelectedJets<=3) continue;
           if (myEvent.numberOfBTaggedJets==0) continue;
           if (myEvent.ETmiss<80) continue;

	   eff_mu_miniIso.Fill(myEvent.mu_miniIso[0],myEvent.mu_pt[0],myEvent.mu_eta[0]);
	   eff_mu_relIsoDB.Fill(myEvent.mu_relIsoDB[0],myEvent.mu_pt[0],myEvent.mu_eta[0]);
	   eff_el_miniIso.Fill(myEvent.el_miniIso[0],myEvent.el_pt[0],myEvent.el_eta[0]);
	   eff_el_relIsoDB.Fill(myEvent.el_relIsoDB[0],myEvent.el_pt[0],myEvent.el_eta[0]);
	   eff_el_relIsoEA.Fill(myEvent.el_relIsoEA[0],myEvent.el_pt[0],myEvent.el_eta[0]);
	
	}
	 eff_mu_miniIso.Draw(fout);
         eff_mu_relIsoDB.Draw(fout);
         eff_el_miniIso.Draw(fout);
         eff_el_relIsoDB.Draw(fout);
         eff_el_relIsoEA.Draw(fout);
          
	 vector<Efficiency> eff_mu;
	 eff_mu.push_back(eff_mu_miniIso);
	 eff_mu.push_back(eff_mu_relIsoDB);
	 vector<Efficiency> eff_el;
	 eff_el.push_back(eff_el_miniIso);
	 eff_el.push_back(eff_el_relIsoDB);
	 eff_el.push_back(eff_el_relIsoEA);
         SuperposeAlgo(fout, eff_mu);
         SuperposeAlgo(fout, eff_el);
         

  }
  fout->Close();

  return 0;
}


