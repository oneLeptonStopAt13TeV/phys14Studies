// ############################################################
// # Usage                                                    #
// #                                                          #
// # - Include this header in your code                       #
// # - Create an instance of babyEvent, for instance event :  #
// #      babyEvent event;                                    #
// # - Open your tree, for example :                          #
// #      TTree* theTree = f.Get("babyTuple");                #
// # - Init branches by calling :                             #
// #      InitializeBranchesForReading(theTree,&event);       #
// # - To read the i-ish event, call :                        #
// #      ReadEvent(theTree,i,&event);                        #
// # - Get the value of your branch by acessing               #
// #      event.branchName;                                   #
// ############################################################

#ifndef babyFormat
#define babyFormat

// ##########################
// #  Baby event structure  #
// ##########################

typedef struct
{
    float jetsPhi[6];
    int   numberOfGeneratedLeptons;
    float HT;
    float MT2W;
    float jetsCSV[6];
    float secondLeptonPt;
    float ETmiss;
    float secondLeptonIso;
    int   eventId;
    int   leadingLeptonId;
    int   numberOfSelectedLeptons;
    float leadingLeptonPt;
    int   lumiId;
    float leadingLeptonEta;
    float jetsPt[6];
    float leadingLeptonIso;
    float jetsPUid[6];
    int   numberOfSelectedJets;
    float leadingLeptonPhi;
    float ETmissPhi;
    int   numberOfBTaggedJets;
    float crossSection;
    int   totalNumberOfInitialEvent;
    float jetsCSVv2[6];
    int   secondLeptonId;
    float MT;
    float jetsEta[6];
    int   runId;
    float secondLeptonPhi;
    float secondLeptonEta;

    // Intermediate pointers for special types
    // Yes, this shit is needed because ROOT is crap.

}
babyEvent;

// #############################
// #  Branches initialization  #
// #############################

void InitializeBranchesForReading(TTree* theTree, babyEvent* myEvent)
{

    theTree->SetBranchAddress("jetsPhi",                 &(myEvent->jetsPhi));
    theTree->SetBranchAddress("numberOfGeneratedLeptons", &(myEvent->numberOfGeneratedLeptons));
    theTree->SetBranchAddress("HT",                      &(myEvent->HT));
    theTree->SetBranchAddress("MT2W",                    &(myEvent->MT2W));
    theTree->SetBranchAddress("jetsCSV",                 &(myEvent->jetsCSV));
    theTree->SetBranchAddress("secondLeptonPt",          &(myEvent->secondLeptonPt));
    theTree->SetBranchAddress("ETmiss",                  &(myEvent->ETmiss));
    theTree->SetBranchAddress("secondLeptonIso",         &(myEvent->secondLeptonIso));
    theTree->SetBranchAddress("eventId",                 &(myEvent->eventId));
    theTree->SetBranchAddress("leadingLeptonId",         &(myEvent->leadingLeptonId));
    theTree->SetBranchAddress("numberOfSelectedLeptons", &(myEvent->numberOfSelectedLeptons));
    theTree->SetBranchAddress("leadingLeptonPt",         &(myEvent->leadingLeptonPt));
    theTree->SetBranchAddress("lumiId",                  &(myEvent->lumiId));
    theTree->SetBranchAddress("leadingLeptonEta",        &(myEvent->leadingLeptonEta));
    theTree->SetBranchAddress("jetsPt",                  &(myEvent->jetsPt));
    theTree->SetBranchAddress("leadingLeptonIso",        &(myEvent->leadingLeptonIso));
    theTree->SetBranchAddress("jetsPUid",                &(myEvent->jetsPUid));
    theTree->SetBranchAddress("numberOfSelectedJets",    &(myEvent->numberOfSelectedJets));
    theTree->SetBranchAddress("leadingLeptonPhi",        &(myEvent->leadingLeptonPhi));
    theTree->SetBranchAddress("ETmissPhi",               &(myEvent->ETmissPhi));
    theTree->SetBranchAddress("numberOfBTaggedJets",     &(myEvent->numberOfBTaggedJets));
    theTree->SetBranchAddress("crossSection",            &(myEvent->crossSection));
    theTree->SetBranchAddress("totalNumberOfInitialEvent", &(myEvent->totalNumberOfInitialEvent));
    theTree->SetBranchAddress("jetsCSVv2",               &(myEvent->jetsCSVv2));
    theTree->SetBranchAddress("secondLeptonId",          &(myEvent->secondLeptonId));
    theTree->SetBranchAddress("MT",                      &(myEvent->MT));
    theTree->SetBranchAddress("jetsEta",                 &(myEvent->jetsEta));
    theTree->SetBranchAddress("runId",                   &(myEvent->runId));
    theTree->SetBranchAddress("secondLeptonPhi",         &(myEvent->secondLeptonPhi));
    theTree->SetBranchAddress("secondLeptonEta",         &(myEvent->secondLeptonEta));
}

// ################################
// #  Function to read one event  #
// ################################

void ReadEvent(TTree* theTree, long int i, babyEvent* myEvent)
{
    theTree->GetEntry(i);

    // Put actual content of special type branches where they should be...
}

#endif
