Install
=======

Setup a CMS release somewhere (to have ROOT, python)

```
cmsrel CMSSW_7_0_0
cd CMSSW_7_0_0/src && cmsenv && cd ../../
```

Setup PyROOF

```
git clone https://github.com/alexAubin/flatTreeAnalysis PyROOF 
```

Install rootpy

```
git clone git://github.com/rootpy/rootpy.git
cd rootpy
python setup.py install --user
cd ..
```

Setup phys14Selection and studies

```
git clone https://github.com/oneLeptonStopAt13TeV/phys14Selection.git
git clone https://github.com/oneLeptonStopAt13TeV/phys14Studies.git
```

Link phys14Selection in PyROOF

```
cd PyROOF/analysis && ln -s ../../phys14Selection && cd ../../
```

Link store location
```
ln -s /opt/sbg/data/data2/cms/aaubin/store/phys14 store
```

Check PyROOF is running

```
cd PyROOF
./PyROOF --debug
```

Setup SonicScrewdriver

```
git clone https://github.com/alexAubin/sonicScrewdriver.git
cd sonicScrewdriver && make clean lib && cd ..
```
