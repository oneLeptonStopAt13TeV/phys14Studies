
#define FOLDER_BABYTUPLES "../../store/babyTuples/"

typedef struct
{
    int   eventId;
    int   numberOfSelectedLeptons;
    int   leadingLeptonId;
    float leadingLeptonPt;
    float leadingLeptonPhi;
    float leadingLeptonEta;
    float leadingLeptonIso;
    int   secondLeptonId;
    float secondLeptonPt;
    float secondLeptonPhi;
    float secondLeptonEta;
    float secondLeptonIso;
    int   numberOfSelectedJets;
    int   numberOfBTaggedJets;
    float jetsPt[6];
    float jetsPhi[6];
    float jetsEta[6];
    float jetsCSV[6];
    float jetsCSVv2[6];
    float jetsPUid[6];
    float ETmiss;
    float ETmissPhi;
    float MT;
    float MT2W;

    int   numberOfGeneratedLeptons;
    float crossSection;
    int   totalNumberOfInitialEvent;

} babyEvent;

babyEvent myEvent;

void InitializeBranchesForReading(TTree* theTree, babyEvent* event)
{
    theTree->SetBranchAddress("eventId"                  , &(event->eventId                  ));
    theTree->SetBranchAddress("numberOfSelectedLeptons"  , &(event->numberOfSelectedLeptons  ));
    theTree->SetBranchAddress("leadingLeptonId"          , &(event->leadingLeptonId          ));
    theTree->SetBranchAddress("leadingLeptonPt"          , &(event->leadingLeptonPt          ));
    theTree->SetBranchAddress("leadingLeptonPhi"         , &(event->leadingLeptonPhi         ));
    theTree->SetBranchAddress("leadingLeptonEta"         , &(event->leadingLeptonEta         ));
    theTree->SetBranchAddress("leadingLeptonIso"         , &(event->leadingLeptonIso         ));
    theTree->SetBranchAddress("secondLeptonId"           , &(event->secondLeptonId           ));
    theTree->SetBranchAddress("secondLeptonPt"           , &(event->secondLeptonPt           ));
    theTree->SetBranchAddress("secondLeptonPhi"          , &(event->secondLeptonPhi          ));
    theTree->SetBranchAddress("secondLeptonEta"          , &(event->secondLeptonEta          ));
    theTree->SetBranchAddress("secondLeptonIso"          , &(event->secondLeptonIso          ));
    theTree->SetBranchAddress("numberOfSelectedJets"     , &(event->numberOfSelectedJets     ));
    theTree->SetBranchAddress("numberOfBTaggedJets"      , &(event->numberOfBTaggedJets      ));
    theTree->SetBranchAddress("jetsPt"                   , &(event->jetsPt                   ));
    theTree->SetBranchAddress("jetsPhi"                  , &(event->jetsPhi                  ));
    theTree->SetBranchAddress("jetsEta"                  , &(event->jetsEta                  ));
    theTree->SetBranchAddress("jetsCSV"                  , &(event->jetsCSV                  ));
    theTree->SetBranchAddress("jetsCSVv2"                , &(event->jetsCSVv2                ));
    theTree->SetBranchAddress("jetsPUid"                 , &(event->jetsPUid                 ));
    theTree->SetBranchAddress("ETmiss"                   , &(event->ETmiss                   ));
    theTree->SetBranchAddress("ETmissPhi"                , &(event->ETmissPhi                ));
    theTree->SetBranchAddress("MT"                       , &(event->MT                       ));
    theTree->SetBranchAddress("MT2W"                     , &(event->MT2W                     ));

    theTree->SetBranchAddress("numberOfGeneratedLeptons" , &(event->numberOfGeneratedLeptons ));
    theTree->SetBranchAddress("crossSection"             , &(event->crossSection             ));
    theTree->SetBranchAddress("totalNumberOfInitialEvent", &(event->totalNumberOfInitialEvent));
}
