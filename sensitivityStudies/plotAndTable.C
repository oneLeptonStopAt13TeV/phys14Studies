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
     s.AddVariable("leadingLeptonPt",          "lead. lepton p_{T}",     "GeV",  20,0,500,    &(myEvent.leadingLeptonPt),          "");
     s.AddVariable("leadingJetPt",             "lead. jet p_{T}",        "GeV",  20,0,500,    &(myEvent.jetsPt[0]),                "");
     s.AddVariable("ETmiss",                   "E_{T}^{miss}",           "GeV", 16,50,530,    &(myEvent.ETmiss),                   "logY");
     s.AddVariable("MT",                       "M_{T}",                  "GeV",  20,0,400,    &(myEvent.MT),                       "logY");
     s.AddVariable("MT2W",                     "M_{T2}^W",               "GeV",  20,0,400,    &(myEvent.MT2W),                     "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("SMtop", "SM top",                             "background", kRed-7);
            s.AddDataset("ttbar",                "SMtop",  245000, 800);
            //s.AddDataset("singleT-s",            "top",  0, 0);
            //s.AddDataset("singleT-t",            "top",  0, 0);
            //s.AddDataset("singleTbar-s",         "top",  0, 0);
            //s.AddDataset("singleTbar-t",         "top",  0, 0);

     s.AddProcessClass("W+jets",   "W+jets",                          "background", kOrange-2);
             s.AddDataset("Wjets",    "W+jets", 238000, 61500);

     s.AddProcessClass("rare",   "rare",                              "background", kMagenta-5);
             s.AddDataset("ttW",   "rare", 178000, 0.700);
             s.AddDataset("ttZ",   "rare", 115000, 0.620);

     s.AddProcessClass("T2tt_850_100",   "T2tt (850/100)",            "signal",     kSpring-1);
             s.AddDataset("T2tt_850_100",   "T2tt_850_100", 102000, 0.019);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     vector<Cut> preselection = {
                                  Cut("numberOfSelectedLeptons", '=', 1 ),
                                  Cut("numberOfSelectedJets",    '>', 3 ),
                                  Cut("numberOfBTaggedJets",     '>', 0 ),
                                  Cut("ETmiss",                     '>', 50)
                                };

     vector<Cut> MTtail       = {
                                    Cut("MT",  '>', 120)
                                };

     vector<Cut> signalRegion = {
                                  Cut("ETmiss",  '>', 300),
                                  Cut("MT2W",    '>', 180)
                                };

     s.AddRegion("preselection",       "preselection",                                      preselection);
     s.AddRegion("preselectionMTtail", "preselection + M_{T} > 120", "preselection",        MTtail);
     s.AddRegion("signalRegion",       "signalRegion",               "preselectionMTtail",  signalRegion);



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

     s.SetGlobalBoolOption  ("1DSuperimposed",    "includeSignal",          true   );
     s.SetGlobalStringOption("1DStack",           "includeSignal",          "stack");
     s.SetGlobalFloatOption ("1DStack",           "factorSignal",           1.0    );

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

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          theTree->GetEntry(i);

          float weight = s.GetDatasetLumiWeight(currentDataset);

          s.AutoFillProcessClass(currentProcessClass,weight);
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

  vector<string> regionsForTable  = { "preselection", "preselectionMTtail", "signalRegion" };
  TableBackgroundSignal(&s,regionsForTable,"singleLepton").Print("yieldTable.tab",2);

  printBoxedMessage("Program done.");
  return (0);
}

