#!/usr/bin/env python
# read a root tree using Pyroot and find muon track
import os

from ROOT import TTree, TObjArray
from ROOT import TCanvas, TPad, TFile, TPaveText
from rootpy.interactive import wait

file=TFile("isoResults.root")

# get all leaves 
leaves = file.GetListOfKeys()

print leaves
for i in leaves:
   print i

#t = file.GetObjectChecked("h_miniIso_muon_T2tt_850_100_isoVsPt","TH2F")
t = file.Get("h_miniIso_muon_T2tt_850_100_isoVsPt")
t.Draw()
#t.ProfileX().Draw()

tb = ROOT.TBrowser()

wait()
