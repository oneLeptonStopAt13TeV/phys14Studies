#!/usr/bin/env python
#
# read a root tree using Pyroot and find muon track
#
#
import os

from ROOT import TTree, TObjArray
from ROOT import TCanvas, TPad, TFile, TPaveText

# this works only on Unix
fileLoc = "../../store/babyTuples/"
fileName = fileLoc + "T2tt_850_100.root"
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


#loop over events
nev = tree.GetEntries()
for iev in range(0,nev) :
    tree.GetEntry(iev)
    # get values from the tree using Python class pyl which contains leaves
    # objects 
    #px = pyl.px.GetValue()
    #py = pyl.py.GetValue()
    #if (iev < 10) : print px,py
    


