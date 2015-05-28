#ifndef efficiency_h
#define efficiency_h

#include <string>
#include <math.h>

#include "TH1F.h"
#include "TH2F.h"

#include "babyReader.h"


#define ISOMAXEFF 0.85

using namespace std;

class Efficiency{

  public:
      string isoMethod;
      string signalName;
      string channel;

      float isocut;

      float pt_max;
      float pt_bins;
      float eta_max;
      float eta_bins;
      float isoCut;
      float iso_max;
      float iso_bins;

      float eff;
      int count;

      TH1F* hIso;
      TH2F* hIsoPt;
      TH2F* hIsoEta;

      TH1F* hEff_Ptref;
      TH1F* hEff_Etaref;
      TH1F* hEff_Pt;
      TH1F* hEff_Eta;


      //methods
      Efficiency(string isoM, string sigN, string channel, float isoCut);
      void Fill(float iso, float pt, float eta);
      void Draw(TFile* fout);


};

Efficiency::Efficiency(string isoM, string sigN, string Channel, float isoCut){
	isoMethod = isoM;
	signalName = sigN;
	channel = Channel; 
	isocut = isoCut;

	eff = 0;
	count = 0;

	//defaut values
        pt_max = 800;
        pt_bins = 20;
        eta_max  = 2.1;
        eta_bins = 10;
	iso_max = 1;
	iso_bins = 20;

	//create histograms
	string hname_default = "h_"+isoMethod+"_"+channel+"_"+signalName;
	string title_default = isoMethod+" "+channel+"-channel - "+signalName;
	string hname;
	string title;
	hname = hname_default+"_iso"; 
	title = title_default+" IsoVar";
        hIso = new TH1F(hname.c_str(),title.c_str(),iso_bins,0,iso_max);
	hIso->GetXaxis()->SetTitle(isoMethod.c_str());
        
	hname = hname_default+"_isoVsPt"; 
	title = title_default+" IsoVar vs Pt";
	hIsoPt = new TH2F(hname.c_str(),title.c_str(),pt_bins,0,pt_max,iso_bins,0,iso_max);
	hIsoPt->GetXaxis()->SetTitle("#p_T [GeV]");
	hIsoPt->GetYaxis()->SetTitle(isoMethod.c_str());
        

	hname = hname_default+"_isoVsEta"; 
	title = title_default+" IsoVar vs Eta";
	hIsoEta = new TH2F(hname.c_str(),title.c_str(),eta_bins,-eta_max,eta_max,iso_bins,0,iso_max);
	hIsoEta->GetXaxis()->SetTitle("#eta");
	hIsoEta->GetYaxis()->SetTitle(isoMethod.c_str());
                  

	hname = hname_default+"_effVsPt_ref"; 
	title = title_default+" Eff vs PT";
        hEff_Ptref = new TH1F(hname.c_str(),title.c_str(),pt_bins,0,pt_max);
	hEff_Ptref->GetXaxis()->SetTitle("#p_T [GeV]");
        hEff_Ptref->Sumw2();
        
	hname = hname_default+"_effVsEta_ref"; 
	title = title_default+" Eff vs Eta";
	hEff_Etaref = new TH1F(hname.c_str(),title.c_str(),eta_bins,-eta_max,eta_max);
	hEff_Etaref->GetXaxis()->SetTitle("#eta");
        hEff_Etaref->Sumw2();
        
	hname = hname_default+"_effVsPt"; 
	title = title_default+" Eff vs PT";
	hEff_Pt = new TH1F(hname.c_str(),title.c_str(),pt_bins,0,pt_max);
	hEff_Pt->GetXaxis()->SetTitle("#p_T [GeV]");
	hEff_Pt->GetYaxis()->SetRangeUser(0,ISOMAXEFF);
        hEff_Pt->Sumw2();

	hname = hname_default+"_effVsEta"; 
	title = title_default+" Eff vs Eta";
	hEff_Eta = new TH1F(hname.c_str(),title.c_str(),eta_bins,-eta_max,eta_max);
	hEff_Eta->GetXaxis()->SetTitle("#eta");
	hEff_Eta->GetYaxis()->SetRangeUser(0,ISOMAXEFF);
        hEff_Eta->Sumw2();
}

void Efficiency::Fill(float iso, float pt, float eta){
	if(iso <0 ) return;
	hIso->Fill(iso);
	hIsoPt->Fill(pt,iso*sqrt(pt));
	hIsoEta->Fill(eta,iso*sqrt(pt));
	//values harcoded for relIsoEA - electron (would need to arguments as parameters
	if(isoMethod=="relIsoEA" && channel=="electron"){
	    bool isIso = false;
	    if (fabs(eta) <= 1.479 && iso < 0.097213) isIso = true;
	    if (fabs(eta) > 1.479 && iso < 0.116708) isIso = true; 
	    if(isIso){
		hEff_Pt->Fill(pt);
		hEff_Eta->Fill(eta);
	    	eff+=1;
	    }
	}
	//default case
	else{
	  if(iso<isocut){
		hEff_Pt->Fill(pt);
		hEff_Eta->Fill(eta);
		eff+=1;
  	  }
	}
	count+=1;
	hEff_Ptref->Fill(pt);
	hEff_Etaref->Fill(eta);
}


void Efficiency::Draw(TFile* fout){
	//renormalise efficiency
	hEff_Pt->Divide(hEff_Ptref);
	hEff_Eta->Divide(hEff_Etaref);
	
	
	fout->cd();	
        hIso->Write();
        hIsoPt->Write();
        hIsoEta->Write();
	
	//Plots in Canvas Iso vs Pt/Eta
	TCanvas cIsoPt(hIsoPt->GetName());
	cIsoPt.SetLogy();
	hIsoPt->Draw("COLZ");
	string fname = string("plots/")+cIsoPt.GetName()+".png";
	cIsoPt.Print(fname.c_str());
	cIsoPt.Write();
	
	TCanvas cIsoEta(hIsoEta->GetName());
	cIsoEta.SetLogy();
	hIsoEta->Draw("COLZ");
	fname = string("plots/")+cIsoEta.GetName()+".png";
	cIsoEta.Print(fname.c_str());
	cIsoEta.Write();
	//------------------
	
	//no need to be saved
	//hEff_Ptref->Write();
        //hEff_Etaref->Write();
        hEff_Pt->Write();
        hEff_Eta->Write();
	
}


#endif
