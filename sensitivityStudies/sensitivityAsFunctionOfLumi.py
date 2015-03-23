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
   
    figure = columns[6].split("+/-")

    if (columns[1] == "totalSM") :
        B        = float(figure[0])
        B_uncert = float(figure[1])

        # Setting B_uncert as 50%
        #B_uncert = 0.5*B

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

def excludedSignalStrength(s,b) :
    f = 0.5
    return s/sqrt(s+b+f*f*b*b)
    #mu = 2 * (1 + sqrt(1 + b + f*f*b*b)) / s
    return mu

def discoveredSignalStrength(s,b) :
    f = 0.5
    return s/sqrt(b+f*f*b*b)
    #mu = 3 * sqrt(b + f*f*b*b) / s
    return mu


mu1_disco = [] 
mu1_exclu = []
mu2_disco = []
mu2_exclu = []

lumi = range(1,30)

for l in lumi :
    S1_ = S1 * l 
    S2_ = S2 * l
    B_  = B  * l

    print S1_, B_, discoveredSignalStrength(S1_,B_)

    mu1_disco.append(discoveredSignalStrength(S1_,B_))
    mu1_exclu.append(excludedSignalStrength(S1_,B_)) 
    mu2_disco.append(discoveredSignalStrength(S2_,B_))
    mu2_exclu.append(excludedSignalStrength(S2_,B_)) 

###############
#  Make plot  #
###############

print mu1_disco

plotting.figure()

plotting.xlabel("Lumi [fb-1]", fontsize = 16)
plotting.ylabel("Discovery sensitivity [sigma]", fontsize = 16)
plotting.grid()

plotting.plot(lumi, mu1_disco, 'r--', linewidth=1.5, color="blue",   label=r"T2tt (850/100) (3\sigma disco)")
plotting.plot(lumi, mu1_exclu, 'r-',  linewidth=1.5, color="blue",   label=r"T2tt (850/100) (2\sigma exclu)")
plotting.plot(lumi, mu2_disco, 'r--', linewidth=1.5, color="orange", label=r"T2tt (650/325) (3\sigma disco)")
plotting.plot(lumi, mu2_exclu, 'r-',  linewidth=1.5, color="orange", label=r"T2tt (650/325) (2\sigma exclu)")

plotting.legend(loc="upper left", fancybox=True)

plotting.savefig("sensitivityAsFunctionOfLumi.pdf")
