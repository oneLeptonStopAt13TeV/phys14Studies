#!/usr/bin/env python

###################
# Import packages #
###################

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plotting
from math import sqrt, log
import time, sys, os
from optparse import OptionParser 

plotting.rc('text', usetex=True)
#plotting.rc('font', family='serif')

###################
# Parse options #
###################

parser = OptionParser()

parser.add_option("-s", "--signal", dest="signal", default=-1,
        action="store", help="ID of the signal, 1 = 850/100, 2 = 650/325")
parser.add_option("-r", "--regionId", dest="regionId", default=3,
        action="store", help="ID of the signal region to consider, 1 = SR1, 2 = SR2, 3 = SR3")
parser.add_option("-f", "--systematic", dest="systematicUncertainty", default=0.25,
        action="store", help="Systematic uncertainty for the background")
parser.add_option("-F", "--FOM", dest="FOMToUse", default="excludedSignalStrength",
        action="store", help="FOM to use")
parser.add_option("-v", "--variation", dest="variation", default="",
        action="store", help="Special variation : syst or tt2l")

(options, args) = parser.parse_args() 

if (options.signal == "1") :
    signalTag = "T2tt_850_100"
    signalLabel = "T2tt (850/100)"

if (options.signal == "2") :
    signalTag = "T2tt_650_325"
    signalLabel = "T2tt (650/325)"

if (options.variation == "syst") :
    systVariations    = [ 0.4,  0.25,  0.1 ]
    FOMSystVariations = [ [],   [],    []  ]
    systVariationsColors = [  "#ff0077", "#0077ff","#00aa77"  ]
if (options.variation == "tt2l") :
    tt2lVariations       = [ 1,    0.75,  0.5 ]
    FOMtt2lVariations    = [ [],   [],    []  ]
    tt2lVariationsColors = [ "#0077ff", "#ff00ff", "#00aa77" ]

###################
# FOM definitions #
###################

def exclusionSignificance(s,b,f) :
    return s/sqrt(s+b+f*f*b*b)

def discoverySignificance(s,b,f) :
    return s/sqrt(b+f*f*b*b)

def excludedSignalStrength(s,b,f) :
    mu = 2 * (1 + sqrt(1 + b + f*f*b*b)) / s
    return mu

def discoveredSignalStrength(s,b,f) :
    mu = 3 * sqrt(b + f*f*b*b) / s
    return mu

if (options.FOMToUse == "excludedSignalStrength")   : FOM = excludedSignalStrength;     FOMlabel="2$\sigma$ excludable signal strength";
if (options.FOMToUse == "discoveredSignalStrength") : FOM = discoveredSignalStrength;   FOMlabel="3$\sigma$ discoverable signal strength";
if (options.FOMToUse == "exclusionSignificance")    : FOM = exclusionSignificance;      FOMlabel="2$\sigma$ exclusion significance";
if (options.FOMToUse == "discoverySignificance")    : FOM = discoverySignificance;      FOMlabel="2$\sigma$ discovery significance";

#################################
# Read B and S from yield table #
#################################

tt2l = 0

B    = 0
S    = 0
B_uncert  = 0
S_uncert = 0

for line in open("yieldTable.tab"):
    line = line.replace(' ', '')
    columns = line.split('|')
    
    if (len(columns) < 4) :
        continue
    
    # FIXME : this is a very dirty way of accessing the right signal region
    figure = columns[3+int(options.regionId)].split("+/-")

    if (columns[1] == "ttbar_2l") :
        tt2l = float(figure[0])

    if (columns[1] == "totalSM") :
        B        = float(figure[0])
        B_uncert = float(figure[1])

    if (columns[1] == signalTag) :
        S        = float(figure[0])
        S_uncert = float(figure[1])
    
print "For 1 fb-1 :"
print "  B  =", B, "+/-", B_uncert
print "  S  =", S, "+/-", S_uncert

###################################
# Compute FOM as function of lumi #
###################################

mu = []


lumi = range(1,30)
f = options.systematicUncertainty
for l in lumi :
    S_ = S * l
    B_ = B * l

    mu.append(FOM(S_,B_,f))

    if (options.variation == "syst") :
        for i in range(len(systVariations)) :
            FOMSystVariations[i].append(FOM(S_,B_,systVariations[i]))
    if (options.variation == "tt2l") :
        for i in range(len(tt2lVariations)) :
            B__ = B_ - (1 - tt2lVariations[i]) * tt2l * l
            FOMtt2lVariations[i].append(FOM(S_,B__,f))

###############
#  Make plot  #
###############

fig = plotting.figure()

fig.suptitle(signalLabel,      fontsize = 20)
plotting.xlabel("Lumi [fb$^{-1}$]", fontsize = 20)
plotting.ylabel(FOMlabel,      fontsize = 20)
plotting.grid()
plotting.tick_params(labelsize=16)
plotting.ylim(0,4)

if (options.variation == "") :
    plotting.plot(lumi, mu, 'r', linewidth=2, color="#0077ff")

if (options.variation == "syst") :
    for i in range(len(systVariations)) :
        plotting.plot(lumi,FOMSystVariations[i], linewidth=2, color=systVariationsColors[i], label="f = "+str(systVariations[i]))
    plotting.legend(loc="upper right")
if (options.variation == "tt2l") :
    for i in range(len(tt2lVariations)) :
        plotting.plot(lumi,FOMtt2lVariations[i], linewidth=2, color=tt2lVariationsColors[i], label=r"$t\bar{t}\rightarrow ll$ lowered by "+str((1-tt2lVariations[i])*100)+"\%")
    plotting.legend(loc="upper right")

plotting.plot([0,30],[1,1], 'k-', lw=2)

plotting.savefig("sensitivityAsFunctionOfLumi.pdf")
