#!/usr/bin/env python

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

B  = 0
S1 = 0
S2 = 0
B_uncert  = 0
S1_uncert = 0
S2_uncert = 0

for line in open("yieldTable.tab"):
    line = line.replace(' ', '')
    columns = line.split('|')
    
    if (len(columns) < 4) :
        continue
    
    figure = columns[5].split("+/-")

    if (columns[1] == "totalSM") :
        B        = float(figure[0])
        B_uncert = float(figure[1])

    if (columns[1] == "T2tt_850_100") :
        S1        = float(figure[0])
        S1_uncert = float(figure[1])
    
    if (columns[1] == "T2tt_650_325") :
        S2        = float(figure[0])
        S2_uncert = float(figure[1])

print "For 1 fb-1 :"
print "  B  =", B,  "+/-", B_uncert
print "  S1 =", S1, "+/-", S1_uncert
print "  S2 =", S2, "+/-", S2_uncert

###################################
# Compute FOM as function of lumi #
###################################

def FOM_discovery(s,b) :
    f = 0.5
    return s/sqrt(b+f*f*b*b)

_FOM1_disco             = FOM_discovery(S1,B) 
_FOM1_disco_statp1sigma = FOM_discovery(S1,B+B_uncert) 
_FOM1_disco_statm1sigma = FOM_discovery(S1,B-B_uncert) 
_FOM2_disco             = FOM_discovery(S2,B) 
_FOM2_disco_statp1sigma = FOM_discovery(S2,B+B_uncert) 
_FOM2_disco_statm1sigma = FOM_discovery(S2,B-B_uncert) 

lumi = range(0,30)

FOM1_disco             = []
FOM1_disco_statp1sigma = []
FOM1_disco_statm1sigma = []
FOM2_disco             = []
FOM2_disco_statp1sigma = []
FOM2_disco_statm1sigma = []

for l in lumi :
    FOM1_disco            .append(sqrt(l) * _FOM1_disco            )
    FOM1_disco_statp1sigma.append(sqrt(l) * _FOM1_disco_statp1sigma)
    FOM1_disco_statm1sigma.append(sqrt(l) * _FOM1_disco_statm1sigma) 
    FOM2_disco            .append(sqrt(l) * _FOM2_disco            )
    FOM2_disco_statp1sigma.append(sqrt(l) * _FOM2_disco_statp1sigma)
    FOM2_disco_statm1sigma.append(sqrt(l) * _FOM2_disco_statm1sigma) 

###############
#  Make plot  #
###############

plotting.figure()

plotting.xlabel("Lumi [fb-1]", fontsize = 16)
plotting.ylabel("Discovery sensitivity [sigma]", fontsize = 16)
plotting.grid()

plotting.plot(lumi, FOM1_disco, 'r-', linewidth=1.5, color="blue", label="T2tt (850/100)")
plotting.fill_between(lumi, FOM1_disco_statp1sigma, FOM1_disco_statm1sigma, alpha = 0.1, facecolor = "blue")
plotting.plot(lumi, FOM2_disco, 'r-', linewidth=1.5, color="orange", label="T2tt (650/325)")
plotting.fill_between(lumi, FOM2_disco_statp1sigma, FOM2_disco_statm1sigma, alpha = 0.1, facecolor = "orange")

plotting.legend(loc="upper left", fancybox=True)

plotting.savefig("sensitivityAsFunctionOfLumi.pdf")
