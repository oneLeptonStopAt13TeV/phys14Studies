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

     s.AddProcessClass("1ltopMG",         "1ltop (madgraph)",                    "background", kRed-7);
            s.AddDataset("ttbar-madgraph",        "1ltopMG",  0, 0);
     s.AddProcessClass("ttbar_2lMG", "t#bar{t} #rightarrow l^{+}l^{-} (madgraph)", "background",kCyan-3);

     s.AddProcessClass("1ltopPY",         "1ltop (pythia)",                    "background", kRed);
            s.AddDataset("ttbar-pythia8",        "1ltopPY",  0, 0);
     s.AddProcessClass("ttbar_2lPY", "t#bar{t} #rightarrow l^{+}l^{-} (pythia)", "background",kCyan);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     vector<Cut> preselection = {
                                  Cut("numberOfSelectedLeptons", '=', 1 ),
                                  Cut("leadingLeptonPt",         '>', 30),
                                  Cut("numberOfSelectedJets",    '>', 3 ),
                                  Cut("numberOfBTaggedJets",     '>', 0 ),
                                  Cut("ETmiss",                  '>', 50)
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

     s.AddRegion("preselection",       "preselection",                                      preselection);
     s.AddRegion("preselectionMTtail", "preselection + M_{T} > 120", "preselection",        MTtail);

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

          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar-pythia8") && (myEvent.numberOfGeneratedLeptons == 2))
              currentProcessClass_ = "ttbar_2lPY";
          if ((currentDataset == "ttbar-madgraph") && (myEvent.numberOfGeneratedLeptons == 2))
              currentProcessClass_ = "ttbar_2lMG";

          // Get the i-th entry
          theTree->GetEntry(i);

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

  vector<string> regionsForTable  = { "preselection", "preselectionMTtail" };
  TableBackgroundSignal(&s,regionsForTable,"singleLepton").Print("yieldTable.tab",2);
  TableBackgroundSignal(&s,regionsForTable,"singleLepton").PrintLatex("yieldTable.tex",0);

  printBoxedMessage("Program done.");
  return (0);
}

