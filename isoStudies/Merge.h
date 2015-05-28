#include "Efficiency.h"
#include "TStyle.h"

/*
 * class Merge{


	Merge();

	void Superpose(vector<Efficiency>);
        void SuperposeDataset();

};
*/

//gStyle->SetOptTitle(0);
//gStyle->SetOptStat(0);

void SuperposeAlgo(TFile* fout, vector<Efficiency> eff){
   string name;
   if(eff.size()>0) name = eff[0].channel+"_"+eff[0].signalName;
   string ctitle = name+"_pt";
   TCanvas* cPt = new TCanvas(ctitle.c_str());
   ctitle = name+"_eta";
   TCanvas* cEta = new TCanvas(ctitle.c_str());
   TLegend* leg = new TLegend(0.8,0.75,0.9,0.9);
   string arg;
   for(unsigned int i=0;i<eff.size();i++){
      if(i==0) arg = "e";
      else arg = "esame";
      cPt->cd();
      eff[i].hEff_Pt->SetLineColor(1+i);
      eff[i].hEff_Pt->SetLineWidth(2);
      eff[i].hEff_Pt->Draw(arg.c_str());
      cEta->cd();
      eff[i].hEff_Eta->SetLineColor(1+i);
      eff[i].hEff_Eta->SetLineWidth(2);
      eff[i].hEff_Eta->Draw(arg.c_str());
      leg->AddEntry(eff[i].hEff_Pt,eff[i].isoMethod.c_str(),"l");
   }
   cPt->cd();
   leg->Draw();
   cEta->cd();
   leg->Draw();
   fout->cd();
   cPt->Write();
   cEta->Write();
   string ofilename = string("plots/")+cPt->GetName()+".png";
   cPt->Print(ofilename.c_str());
   ofilename = string("plots/")+cEta->GetName()+".png";
   cEta->Print(ofilename.c_str());
}

//void Superpose

/*
void Merge::SuperposeDataset(){
	//TGraph* g = new
}
*/
