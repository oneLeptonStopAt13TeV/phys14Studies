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

typedef struct{
	//muons
	Float_t mu_pt[2];
	Float_t mu_eta[2];
	Float_t mu_miniIso[2];
	Float_t mu_relIsoDB[2];
	//electrons
	Float_t el_pt[2];
	Float_t el_eta[2];
	Float_t el_miniIso[2];
	Float_t el_relIsoDB[2];
	Float_t el_relIsoEA[2];
	//selection
        Int_t   numberOfSelectedJets;
        Int_t   numberOfBTaggedJets;
        Float_t ETmiss;
    // Intermediate pointers for special types
    // Yes, this shit is needed because ROOT is crap.
}
babyEvent;

// #############################
// #  Branches initialization  #
// #############################

void InitializeBranchesForReading(TTree* theTree, babyEvent* myEvent)
{

    theTree->SetBranchAddress("mu_pt",                 &(myEvent->mu_pt));
    theTree->SetBranchAddress("mu_eta",                 &(myEvent->mu_eta));
    theTree->SetBranchAddress("mu_miniIso",                 &(myEvent->mu_miniIso));
    theTree->SetBranchAddress("mu_relIsoDB",                 &(myEvent->mu_relIsoDB));
    theTree->SetBranchAddress("el_pt",                 &(myEvent->el_pt));
    theTree->SetBranchAddress("el_eta",                 &(myEvent->el_eta));
    theTree->SetBranchAddress("el_miniIso",                 &(myEvent->el_miniIso));
    theTree->SetBranchAddress("el_relIsoDB",                 &(myEvent->el_relIsoDB));
    theTree->SetBranchAddress("el_relIsoEA",                 &(myEvent->el_relIsoEA));
    theTree->SetBranchAddress("numberOfSelectedJets",                 &(myEvent->numberOfSelectedJets));
    theTree->SetBranchAddress("numberOfBTaggedJets",                 &(myEvent->numberOfBTaggedJets));
    theTree->SetBranchAddress("ETmiss",                 &(myEvent->ETmiss));
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
