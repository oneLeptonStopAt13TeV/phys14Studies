Install
=======

Setup a CMS release somewhere (to have ROOT, python)

```
cmsrel CMSSW_7_3_0
cd CMSSW_7_3_0/src && cmsenv && cd ../../
```

Setup PyROOF

```
git clone https://github.com/alexAubin/flatTreeAnalysis PyROOF 
```

Install rootpy (once for all - if you already did it in the past, shouldnt be needed.)

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
cd PyROOF/analysis && ln -s ../../phys14Selection stopPhys14 && cd ../../
```

Compile MT2W C++ implementation

```
cd phys14Selection/inputs/MT2W/ && make && cd ../../..
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
