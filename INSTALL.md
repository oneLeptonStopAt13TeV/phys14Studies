Install
=======

CMSSW release (required to have ROOT and python)
-------------

```
cmsrel CMSSW_7_3_0 && cd CMSSW_7_3_0/src && cmsenv && cd ../../
```

rootpy (required for PyROOF)
------

Once for all - if you already did it in the past, shouldnt be needed.

```
git clone git://github.com/rootpy/rootpy.git
cd rootpy
python setup.py install --user
cd ..
```

If the `python setup.py ...` complains, you might have to install setuptools :

```
wget https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py
python ez_setup.py --user
```

Then retry from the `python setup.py ...` step.

PyROOF
------

```
git clone https://github.com/oneLeptonStopAt13TeV/PyROOF 
```

SonicScrewdriver
----------------

```
git clone https://github.com/alexAubin/sonicScrewdriver.git
cd sonicScrewdriver && make clean lib && cd ..
```

phys14 selection & studies setup
--------------------------------

```
git clone https://github.com/oneLeptonStopAt13TeV/phys14Selection.git
git clone https://github.com/oneLeptonStopAt13TeV/phys14Studies.git
```

Link the selection to PyROOF :

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

Check PyROOF is running (it creates a small babyTuple using only the first file of the first dataset)

```
cd PyROOF
./PyROOF.py --debug
```

phys14 selection usage
----------------------

The selection is configured from the files in `phys14Selection` :
- `Datasets.py` defines the datasets (from a local wildcard, or a folder on dpm)
- `Analyzer.py` is the boss, it defines all the step you want to do for each event
- `Selection.py` defines the objects selection and event skimming
- `Variables.py` contains the variable definitions from the selected objects
- `BabyTupleFormat.py` defines how you want to write all your stuff in the babyTuple

Once you're happy with your changes, check that they work running PyROOF in debug mode :

```
cd PyROOF
./PyROOF.py --debug
```

Note : if it runs on DPM-located datasets, you will need an active proxy

If you're okay and want to run on every datasets :
- check that you're happy with the configuration for multiprocessing (number of workers, output directory) in `config/localMultiprocessing.py`. 
- Then launch the beast (in a screen session maybe) with `./PyROOF.py --local` or simply `./PyROOF.py -l`

