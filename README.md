ANNRI Gd Model v1.1
=====

The ANNRI Gd Model was developed by the ANNRI collaboration at J-PARC and simulates neutron capture on gadolinium using Geant4. ANNRI v1 only supports Geant4v9.6. ANNRI v1.1 is in the process of
being updated to support Geant4v10.1.3 by Cassandra Little of the Univ. of Michigan / the JSNS^2 collaboration (littleca@umich.edu).

The original distribution can be found at the [ANNRI Gd Model website](https://www.physics.okayama-u.ac.jp/~sakuda/ANNRI-Gd_ver1.html)

Refrence papers for ANNRI:
- [Gamma-ray spectrum from thermal neutron capture on gadolinium-157](https://doi.org/10.1093/ptep/ptz002)
- [Gamma-ray spectra from thermal neutron capture on gadolinium-155 and natural gadolinium](https://doi.org/10.1093/ptep/ptaa015)


How to Run
=====
## Before compiling:

- Source Geant4 enviornments
```bash
source geant4.sh
source geant4make.sh
```
- Define global variables in ./exe_ANNRI_BGO.cc
- If desired, conditions on the output trees can be set in ./src/EventAction.cc

 

## Compile:

- Export paths to the Geant4 data files

```bash
source Geant4
make
```

 

## After compiling:

- Set the neutron gun and desired number of events in ./macro/beam.mac
- Run the executable together with the macro, as follows:
```bash
./bin/Linux-g++/exe_ANNRI_BGO ./macro/beam.mac
```
 

Default output files:
- OriginalGamma.root contains all the generated gamma cascades
- NewMC.root contains the detected gammas
