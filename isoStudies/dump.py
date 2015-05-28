#!/usr/bin/env python
#
# read a root tree using Pyroot and find muon track
#
#
import os

from ROOT import TTree, TObjArray
from ROOT import TCanvas, TPad, TFile, TPaveText

#import read_diff
from read_diff import listOfeventId

#eIdNotFound = listOfeventId('ref_dump/T2tt_850_100_muons_uclaucsb.txt.sort','T2tt_850_100_mu_Strasbourg.dump')
#eIdNotFound = listOfeventId('T2tt_850_100_mu_Strasbourg.dump','ref_dump/T2tt_850_100_muons_uclaucsb.txt.sort')
#eIdNotFound = listOfeventId('ref_dump/T2tt_850_100_electrons_uclaucsb.txt','T2tt_850_100_el_Strasbourg.dump')
eIdNotFound = listOfeventId('T2tt_850_100_el_Strasbourg.dump','ref_dump/T2tt_850_100_electrons_uclaucsb.txt')
print(eIdNotFound)
if '99992' in eIdNotFound:
   print('happy')

# this works only on Unix
fileLoc = "../../store/babyTuples/"
fileName = fileLoc + "T2tt_850_100.root"

# FlatTree
#fileLoc = "root://sbgse1.in2p3.fr//cms/phedex/store/user/aaubin/FlatTrees/v20150314-WildBeast-v2/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/crab_T2tt_850_100/150314_225910/0000/"
#fileName = fileLoc + "FlatTree_1.root"

print "reading from file",fileName

file=TFile(fileName)
tree=file.Get("babyTuple")

# get all leaves 
leaves = tree.GetListOfLeaves()


# define dynamically a python class containing root Leaves objects
class PyListOfLeaves(dict) :
    pass

# create an istance
pyl = PyListOfLeaves()

# add leaves as attributes
for i in range(0,leaves.GetEntries() ) :
    leaf = leaves.At(i)
    name = leaf.GetName()
    # add dynamically attribute to my class 
    pyl.__setattr__(name,leaf)
    print(name) 

#default output run:lumi:event:lepton_pt:lepton_pdgid:MET:mT:NJets:NBJets:HT
def dump(e):
   print int(e.runId.GetValue()) , ":" , int(e.lumiId.GetValue()) , ":" , int(e.eventId.GetValue()) , ":" , \
   	e.leadingLeptonPt.GetValue() , ":" , int(e.leadingLeptonId.GetValue()) , ":" , \
	e.ETmiss.GetValue() , ":" , e.MT.GetValue() , ":" , \
	int(e.numberOfSelectedJets.GetValue()) , ":" , int(e.numberOfBTaggedJets.GetValue()) , ":" , \
	e.HT.GetValue()

#loop over events
nev = tree.GetEntries()

eId_nLep = 0
eId_total = 0

for iev in range(0,nev) :
    tree.GetEntry(iev)
    # get values from the tree using Python class pyl which contains leaves
    # objects 
    #px = pyl.px.GetValue()
    #py = pyl.py.GetValue()
    #if (iev < 10) : print px,py
    #if pyl.eventId.GetValue()==103294:
    #if str(int(pyl.eventId.GetValue())) in [i for i,j in eIdNotFound]:
    #a = (str(int(pyl.eventId.GetValue())),str(int(pyl.lumiId.GetValue())))
    #print(a)
    #if (str(int(pyl.eventId.GetValue())),str(int(pyl.lumiId.GetValue()))) in eIdNotFound:
    if (str(int(pyl.lumiId.GetValue())),str(int(pyl.eventId.GetValue()))) in eIdNotFound:
	if int(pyl.numberOfSelectedLeptons.GetValue()) != 1:
		eId_nLep = eId_nLep + 1 
	        print('nofSelectedLeptons = ',pyl.numberOfSelectedLeptons.GetValue())
 		print('leptons: el = ', pyl.numberOfSelectedElectrons.GetValue(), ' mu = ',pyl.numberOfSelectedMuons.GetValue())
	else:
        	dump(pyl)
	eId_total = eId_total + 1

print('##############################')
print('# nof_id not found in total: ',eId_total)
print('# nof_if where nlep>1:       ',eId_nLep)
print('##############################')
