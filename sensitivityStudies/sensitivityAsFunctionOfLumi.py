#!/usr/bin/env/ python

###################
# Import packages #
###################

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plotting
from math import sqrt

#################################
# Read B and S from yield table #
#################################

B = 0
S = 0
B_uncert = 0
S_uncert = 0

for line in open("yieldTable.tab"):
    line = line.replace(' ', '')
    columns = line.split('|')
    
    if (len(columns) < 4) :
        continue
    
    figure = columns[4].split("+/-")

    if (columns[1] == "totalSM") :
        B        = float(figure[0])
        B_uncert = float(figure[1])

    if (columns[1] == "T2tt_850_100") :
        S        = float(figure[0])
        S_uncert = float(figure[1])

print "For 1 fb-1 :"
print "  B =", B, "+/-", B_uncert
print "  S =", S, "+/-", S_uncert

###################################
# Compute FOM as function of lumi #
###################################

def FOM_discovery(s,b) :
    f = 0.5
    return s/sqrt(b+f*f*b*b)

_FOM_disco             = FOM_discovery(S,B) 
_FOM_disco_statp1sigma = FOM_discovery(S,B+B_uncert) 
_FOM_disco_statm1sigma = FOM_discovery(S,B-B_uncert) 

lumi = range(0,100)

FOM_disco             = []
FOM_disco_statp1sigma = []
FOM_disco_statm1sigma = []

for l in lumi :
    FOM_disco            .append(sqrt(l) * _FOM_disco            )
    FOM_disco_statp1sigma.append(sqrt(l) * _FOM_disco_statp1sigma)
    FOM_disco_statm1sigma.append(sqrt(l) * _FOM_disco_statm1sigma) 

###############
#  Make plot  #
###############

plotting.figure()

plotting.suptitle("T2tt (850/100) at 13 TeV", fontsize = 16)
plotting.xlabel("Lumi [fb-1]", fontsize = 16)
plotting.ylabel("Discovery sensitivity [sigma]", fontsize = 16)
plotting.grid()

plotting.plot(lumi, FOM_disco, 'r-', linewidth=1.5, color="blue")
plotting.fill_between(lumi, FOM_disco_statp1sigma, FOM_disco_statm1sigma, alpha = 0.1, facecolor = "blue")


plotting.savefig("sensitivityAsFunctionOfLumi.png")
