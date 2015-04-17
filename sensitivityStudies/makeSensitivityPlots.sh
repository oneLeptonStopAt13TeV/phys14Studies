



python sensitivityAsFunctionOfLumi.py -s 1 -r 3 -F excludedSignalStrength   && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_850_100_SR3_exclusion.pdf
python sensitivityAsFunctionOfLumi.py -s 1 -r 3 -F discoveredSignalStrength && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_850_100_SR3_discovery.pdf
python sensitivityAsFunctionOfLumi.py -s 2 -r 3 -F excludedSignalStrength   && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_650_325_SR3_exclusion.pdf
python sensitivityAsFunctionOfLumi.py -s 2 -r 3 -F discoveredSignalStrength && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_650_325_SR3_discovery.pdf

python sensitivityAsFunctionOfLumi.py -s 1 -r 3 -F excludedSignalStrength   -v syst && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_850_100_SR3_exclusion_varSyst.pdf
python sensitivityAsFunctionOfLumi.py -s 1 -r 3 -F discoveredSignalStrength -v syst && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_850_100_SR3_discovery_varSyst.pdf
python sensitivityAsFunctionOfLumi.py -s 2 -r 3 -F excludedSignalStrength   -v syst && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_650_325_SR3_exclusion_varSyst.pdf
python sensitivityAsFunctionOfLumi.py -s 2 -r 3 -F discoveredSignalStrength -v syst && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_650_325_SR3_discovery_varSyst.pdf

python sensitivityAsFunctionOfLumi.py -s 1 -r 3 -F excludedSignalStrength   -v tt2l && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_850_100_SR3_exclusion_vartt2l.pdf
python sensitivityAsFunctionOfLumi.py -s 1 -r 3 -F discoveredSignalStrength -v tt2l && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_850_100_SR3_discovery_vartt2l.pdf
python sensitivityAsFunctionOfLumi.py -s 2 -r 3 -F excludedSignalStrength   -v tt2l && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_650_325_SR3_exclusion_vartt2l.pdf
python sensitivityAsFunctionOfLumi.py -s 2 -r 3 -F discoveredSignalStrength -v tt2l && mv sensitivityAsFunctionOfLumi.pdf plots/sensitivity/T2tt_650_325_SR3_discovery_vartt2l.pdf

