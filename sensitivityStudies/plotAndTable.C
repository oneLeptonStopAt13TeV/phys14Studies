#include "common.h"
#include "babyTupleFormat.h"

// #########################################################################
//                              Main function
// #########################################################################

bool dummySelector() { return true; }

int main (int argc, char *argv[])
{

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
     SonicScrewdriver s;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     s.AddVariable("numberOfSelectedLeptons",  "# of selected leptons",  "",        4,0,3,    &(myEvent.numberOfSelectedLeptons),  "");
     s.AddVariable("numberOfSelectedJets",     "# of selected jets",     "",        6,3,8,    &(myEvent.numberOfSelectedJets),     "logY");
     s.AddVariable("numberOfBTaggedJets",      "# of b-tagged jets",     "",        4,0,3,    &(myEvent.numberOfBTaggedJets),      "logY");
     s.AddVariable("leadingLeptonPt",          "lead. lepton p_{T}",     "GeV",  20,0,500,    &(myEvent.leadingLeptonPt),          "logY");
     s.AddVariable("leadingJetPt",             "lead. jet p_{T}",        "GeV",  20,0,500,    &(myEvent.jetsPt[0]),                "");
     s.AddVariable("ETmiss",                   "E_{T}^{miss}",           "GeV", 20,100,500,   &(myEvent.ETmiss),                   "logY");
     s.AddVariable("MT",                       "M_{T}",                  "GeV",  20,0,400,    &(myEvent.MT),                       "logY");
     s.AddVariable("MT2W",                     "M_{T2}^{W}",             "GeV",  20,0,400,    &(myEvent.MT2W),                     "");
     float relIso;
     s.AddVariable("leadingLeptonIso",         "reliso(lepton)",          "",     30,0,0.3,    &(relIso),                           "logY");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("1ltop",         "1ltop",                    "background", kRed-7);
            s.AddDataset("ttbar-madgraph",        "1ltop",  0, 0);
            s.AddDataset("singleTop_s",           "1ltop",  0, 0);
            s.AddDataset("singleTop_t",           "1ltop",  0, 0);
            s.AddDataset("singleTopbar_s",        "1ltop",  0, 0);
            s.AddDataset("singleTopbar_t",        "1ltop",  0, 0);

     s.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);

     s.AddProcessClass("Wjets",   "W+jets",                          "background", kOrange-2);
             s.AddDataset("Wjets",    "Wjets", 0, 0);

     s.AddProcessClass("rare",     "rare",                           "background", kMagenta-5);
             s.AddDataset("ttW",   "rare", 0, 0);
             s.AddDataset("ttZ",   "rare", 0, 0);
             s.AddDataset("WZ",    "rare", 0, 0);
             s.AddDataset("ZZ",    "rare", 0, 0);
             s.AddDataset("DY",    "rare", 0, 0);

     s.AddProcessClass("T2tt_850_100",   "T2tt (850/100)",            "signal",     kSpring-1);
             s.AddDataset("T2tt_850_100",   "T2tt_850_100", 0, 0);

     s.AddProcessClass("T2tt_650_325",   "T2tt (650/325)",            "signal",     kAzure-2);
             s.AddDataset("T2tt_650_325",   "T2tt_650_325", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     vector<Cut> preselection = {
                                  Cut("numberOfSelectedLeptons", '=', 1  ),
                                  Cut("leadingLeptonPt",         '>', 30 ),
                                  Cut("numberOfSelectedJets",    '>', 3  ),
                                  Cut("numberOfBTaggedJets",     '>', 0  ),
                                  Cut("ETmiss",                  '>', 100),
                                };

     vector<Cut> MTtail       = {
                                  Cut("MT",  '>', 120)
                                };

     vector<Cut> signalRegion1 = {
                                  Cut("MT",     '>', 150),
                                  Cut("ETmiss", '>', 250),
                                  Cut("MT2W",   '>', 180)
                                 };

     vector<Cut> signalRegion2 = {
                                   Cut("MT",     '>', 150),
                                   Cut("ETmiss", '>', 300),
                                   Cut("MT2W",   '>', 190)
                                 };

     vector<Cut> signalRegion3 = {
                                   Cut("MT",     '>', 150),
                                   Cut("ETmiss", '>', 350),
                                   Cut("MT2W",   '>', 200)
                                 };

     s.AddRegion("preselection",          "Preselection",                                         preselection);

        s.AddRegion("preselectionMTtail", "Preselection + M_{T} > 120",    "preselection",        MTtail);

            s.AddRegion("signalRegion1",  "Signal region #1",              "preselectionMTtail",  signalRegion1);
            s.AddRegion("signalRegion2",  "Signal region #2",              "preselectionMTtail",  signalRegion2);
            s.AddRegion("signalRegion3",  "Signal region #3",              "preselectionMTtail",  signalRegion3);

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     s.AddChannel("singleLepton", "e/#mu-channels", dummySelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("1DStack");

     // Config plots

     s.SetGlobalBoolOption  ("1DSuperimposed",    "includeSignal",          true          );
     s.SetGlobalStringOption("1DStack",           "includeSignal",          "superimposed");
     s.SetGlobalFloatOption ("1DStack",           "factorSignal",           1000.0        );

     s.SetLumi(1000);

     s.SetGlobalStringOption("Plot", "infoTopRight", "CMS Work in progress");
     s.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 13 TeV, L = 1 fb^{-1}");

     s.SetGlobalBoolOption("Plot", "exportPdf", true);
     s.SetGlobalBoolOption("Plot", "exportEps", false);
     s.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  s.GetDatasetList(&datasetsList);

  cout << "   > Reading datasets... " << endl;
  cout << endl;

  string sampleName, sampleType;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = s.GetProcessClass(currentDataset);

     sampleName = currentDataset;
     sampleType = s.GetProcessClassType(currentProcessClass);

     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple");

     InitializeBranchesForReading(theTree,&myEvent);

     theTree->GetEntry(0);
     float weightLumi = s.GetLumi() * myEvent.crossSection / myEvent.totalNumberOfInitialEvent;

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          theTree->GetEntry(i);

          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar-madgraph") && (myEvent.numberOfGeneratedLeptons == 2))
              currentProcessClass_ = "ttbar_2l";

          relIso = myEvent.leadingLeptonIso / myEvent.leadingLeptonPt;
          float weight = weightLumi;

          s.AutoFillProcessClass(currentProcessClass_,weight);
      }

      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  s.MakePlots();
  cout << "   > Saving plots..." << endl;
  s.WritePlots("./plots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  vector<string> regionsForTable  = { "preselection", "preselectionMTtail", "signalRegion1", "signalRegion2", "signalRegion3" };
  TableBackgroundSignal(&s,regionsForTable,"singleLepton").Print("yieldTable.tab",2);
  TableBackgroundSignal(&s,regionsForTable,"singleLepton").PrintLatex("yieldTable.tex",0);

  printBoxedMessage("Program done.");
  return (0);
}

