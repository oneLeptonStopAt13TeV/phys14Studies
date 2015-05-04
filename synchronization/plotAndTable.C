#include "common.h"
#include "babyReader.h"
#include <fstream>

// #########################################################################
//                              Main function
// #########################################################################

bool dummySelector() { return true; }

babyEvent myEvent;

int main (int argc, char *argv[])
{

  ofstream dump_T2tt_850_100_el("T2tt_850_100_el_Strasbourg.dump",ofstream::out);
  ofstream dump_T2tt_850_100_mu("T2tt_850_100_mu_Strasbourg.dump",ofstream::out);
  ofstream dump_T2tt_650_325_el("T2tt_650_325_el_Strasbourg.dump",ofstream::out);
  ofstream dump_T2tt_650_325_mu("T2tt_650_325_mu_Strasbourg.dump",ofstream::out);

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

     s.AddProcessClass("T2tt_850_100",   "T2tt (850/100)",            "background",     kSpring-1);
             s.AddDataset("T2tt_850_100",   "T2tt_850_100", 0, 0);

     s.AddProcessClass("T2tt_650_325",   "T2tt (650/325)",            "background",     kAzure-2);
               s.AddDataset("T2tt_650_325",   "T2tt_650_325", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     vector<Cut> baselineCuts = {
                                  Cut("numberOfSelectedLeptons", '=', 1  ),
                                  Cut("numberOfSelectedJets",    '>', 3  ),
                                  Cut("numberOfBTaggedJets",     '>', 0  ),
                                  Cut("ETmiss",                  '>', 80 ),
                                };

     s.AddRegion("baseline", "Baseline selection", baselineCuts);

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
          ReadEvent(theTree,i,&myEvent);

          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar-madgraph") && (myEvent.numberOfGeneratedLeptons == 2))
              currentProcessClass_ = "ttbar_2l";

          relIso = myEvent.leadingLeptonIso / myEvent.leadingLeptonPt;
          float weight = weightLumi;

          s.AutoFillProcessClass(currentProcessClass_,weight);

          if ((myEvent.numberOfSelectedLeptons == 1 )
           && (myEvent.numberOfSelectedJets    >= 4 )
           && (myEvent.numberOfBTaggedJets     >= 1 )
           && (myEvent.ETmiss                  >= 80))
          {
              ofstream* outputFile;

                   if ((currentDataset == "T2tt_850_100") && (abs(myEvent.leadingLeptonId) == 11)) outputFile = &dump_T2tt_850_100_el;
              else if ((currentDataset == "T2tt_850_100") && (abs(myEvent.leadingLeptonId) == 13)) outputFile = &dump_T2tt_850_100_mu;
              else if ((currentDataset == "T2tt_650_325") && (abs(myEvent.leadingLeptonId) == 11)) outputFile = &dump_T2tt_650_325_el;
              else if ((currentDataset == "T2tt_650_325") && (abs(myEvent.leadingLeptonId) == 13)) outputFile = &dump_T2tt_650_325_mu;

              (*outputFile) << setprecision(2)              << fixed
                            << myEvent.runId                << " "
                            << myEvent.lumiId               << " "
                            << myEvent.eventId              << " "
                            << myEvent.leadingLeptonPt      << " "
                            << myEvent.leadingLeptonId      << " "
                            << myEvent.ETmiss               << " "
                            << myEvent.MT                   << " "
                            << myEvent.numberOfSelectedJets << " "
                            << myEvent.numberOfBTaggedJets  << " "
                            << myEvent.HT
                            << endl;
          }
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

  vector<string> regionsForTable  = { "baseline" };
  TableBackgroundSignal(&s,regionsForTable,"singleLepton").Print("yieldTable.tab",2);

  printBoxedMessage("Program done.");


  dump_T2tt_850_100_el.close();
  dump_T2tt_850_100_mu.close();
  dump_T2tt_650_325_el.close();
  dump_T2tt_650_325_mu.close();

  return (0);
}

