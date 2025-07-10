Change Log
====
#### Juan David Cortés Echeverría

<br>

These changes should allow ANNRI to work with GEANT4 version 10.1.3. I have divided them by file in order to, hopefully, ensure a better legibility.

<br>

### Table of Contents

- [`GNUmakefile`](#gnumakefile)
- [`Physics_MuonPhysics.hh`](#physics_muonphysicshh)
- [`Physics_HadronPhysics.cc`](#physics_hadronphyiscscc)
- [`BGOHit.cc`](#bgohitcc)
- [`Physics_EMPhysics.hh`](#physics_emphysicshh)
- [`Physics_EMPhysics.cc`](#physics_emphysicscc)
- [`GdCaptureGammas_glg4sim.cc`](#gdcapturegammas_glg4simcc)
- [`DetectorConstruction.cc`](#detectorconstructioncc)
- [`GdCaptureGammas_ggarnet.cc`](#gdcapturegammas_ggarnetcc)
- [`Physics_GeneralPhysics.cc`](#physics_generalphysicscc)
- [`Physics_RadioactiveDecayPhysics.cc`](#physics_radioactivedecayphysicscc)
- [`Physics_PhysicsList.hh`](#physics_physicslisthh)
- [`Physics_PhysicsList.cc`](#physics_physicslistcc)
- [`RunAction.cc`](#runactioncc)
- [`GdNeutronHPCaptureFS.cc`](#gdneutroncapturehpcapturefscc)
- [`Notes`](#notes)

<br>
<br>

The Files (And, ultimately, the Notes)
====

<br>
<br>


## GNUmakefile

Added `CPPFLAGS += -std=c++11` near the top


<br>
<br>


## Physics_MuonPhysics.hh

Replaced `“G4MuonMinusCaptureAtRest.hh”` with `“G4MuonMinusCapture.hh”`


<br>
<br>


## Physics_HadronPhysics.cc

- #included `"G4NeutronHPInelastic.hh"`

- Replaced `“G4LElastic.hh”` with `“G4HadronElastic.hh”`

- Replaced the previous low energy models for protons, neutrons, pions, kaons (i.e. `"G4LEPionPlusInelastic.hh"`, `"G4LEPionMinusInelastic.hh"`, `"G4LEKaonPlusInelastic.hh"`, `"G4LEKaonZeroSInelastic.hh"`, `"G4LEKaonZeroLInelastic.hh"`, `"G4LEKaonMinusInelastic.hh"`, `"G4LEProtonInelastic.hh"`, `"G4LEAntiProtonInelastic.hh"`, `"G4LENeutronInelastic.hh"`, `"G4LEAntiNeutronInelastic.hh"`) with `"G4CascadeInterface.hh"`

- Replaced the previous low energy models for deuterons, tritons and alphas (i.e. `"G4LEDeuteronInelastic.hh"`, `"G4LETritonInelastic.hh"`, `"G4LEAlphaInelastic.hh"`) with `"G4BinaryLightIonReaction.hh"`

- Replaced the previous high energy models (i.e. `"G4HEPionPlusInelastic.hh"`, `"G4HEPionMinusInelastic.hh"`, `"G4HEKaonPlusInelastic.hh"`, `"G4HEKaonZeroInelastic.hh"`, `"G4HEKaonZeroInelastic.hh"`, `"G4HEKaonMinusInelastic.hh"`, `"G4HEProtonInelastic.hh"`, `"G4HEAntiProtonInelastic.hh"`, `"G4HENeutronInelastic.hh"`, `"G4HEAntiNeutronInelastic.hh"`) with `“G4FTFModel.hh”`

- Replaced `"G4LCapture.hh"` with `"G4NeutronHPCapture.hh"`

- Replaced `"G4PiMinusAbsorbtionAtRest.hh"` with `"G4PiMinusAbsorptionBertini.hh"`

- Replaced `"G4KaonMinusAbsorptionAtRest.hh"` with `"G4KaonMinusAbsorptionBertini.hh"`

- Added the declaration of `theParticleIterator`:

```bash
G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
```

- Removed `“G4AntiProtonAnnihilationAtRest.hh”` (it wasn’t being used)

- Each and every single model from `G4FTFModel.hh` needs to be added to an instance of `G4TheoFSGenerator` in order to then be registered using `RegisterMe()` (https://apc.u-paris.fr/~franco/g4doxy/html/classG4HadronicAbsorptionFritiof.html). Thus, #included the following files:

```bash
#include "G4TheoFSGenerator.hh"
#include "G4LundStringFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4PreCompoundModel.hh"
#include "G4GeneratorPrecompoundInterface.hh"
```


and, changed the following code

```bash
G4FTFModel* theHEInelasticModel = new G4FTFModel();
theInelasticProcess->RegisterMe(theHEInelasticModel);
pmanager->AddDiscreteProcess(theInelasticProcess);
```

into:

```bash
G4TheoFSGenerator * theHEInelasticModel = new G4TheoFSGenerator("FTFP");
G4FTFModel * theHEStringModel = new G4FTFModel();
G4LundStringFragmentation * theHELund = new G4LundStringFragmentation();
G4ExcitedStringDecay * theHEStringDecay = new G4ExcitedStringDecay(theHELund);
G4VPreCompoundModel * theHEDeExcitation = new G4PreCompoundModel();
G4GeneratorPrecompoundInterface * theHETransport = new G4GeneratorPrecompoundInterface(theHEDeExcitation);

theHEStringModel->SetFragmentationModel(theHEStringDecay);

theHEInelasticModel->SetHighEnergyGenerator(theHEStringModel);
theHEInelasticModel->SetTransport(theHETransport);

theInelasticProcess->RegisterMe(theHEInelasticModel);
pmanager->AddDiscreteProcess(theInelasticProcess);
```



<br>
<br>


## BGOHit.cc

#included `“G4SystemOfUnits.hh”`


<br>
<br>


## Physics_EMPhysics.hh

Replaced `“G4MultipleScattering.hh”` with `“G4eMultipleScattering.hh”`


<br>
<br>


## Physics_EMPhysics.cc

Added the declaration of `theParticleIterator`:

```bash
G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
```


<br>
<br>



## GdCaptureGammas_glg4sim.cc

#included `“G4PhysicalConstants.hh”`


<br>
<br>


## DetectorConstruction.cc

- #included `“G4SystemOfUnits.hh”` 

- #included `“G4PhysicalConstants.hh”`


<br>
<br>


## GdCaptureGammas_ggarnet.cc

#included `"G4PhysicalConstants.hh"`


<br>
<br>


## Physics_GeneralPhysics.cc

Added the declaration of `theParticleIterator`:

```bash
G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
```


<br>
<br>



## Physics_RadioactiveDecayPhysics.cc

Added the declaration of `theParticleIterator`:

```bash
G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
```

<br>
<br>


## Physics_PhysicsList.hh

In order to properly source the ions for each `IonTable`, added an override of the `ConstructParticle()` function:

```bash
virtual void ConstructParticle() override;
```

<br>
<br>


## Physics_PhysicsList.cc

- #included `"G4SystemOfUnits.hh"`

- To continue with the proper sourcing of the ions for each `IonTable`, #included the following files:

```bash
#include "G4IonPhysics.hh"
#include "G4IonTable.hh"
#include "G4NuclideTable.hh"
```

registered a new instance of `G4IonPhysics` in `Physics_PhysicsList::Physics_PhysicsList():  G4VModularPhysicsList()`:

```bash
RegisterPhysics(new G4IonPhysics());
```

and overrode `ConstructParticle()`:

```bash
void Physics_PhysicsList::ConstructParticle()
{

        G4VModularPhysicsList::ConstructParticle();
        G4GenericIon::GenericIonDefinition();
        G4IonTable::GetIonTable()->CreateAllIon();

        auto* nuclideTab = G4NuclideTable::GetNuclideTable();
        nuclideTab->SetThresholdOfHalfLife(0.0);
        nuclideTab->GenerateNuclide();
        auto* ionTab = G4IonTable::GetIonTable();
        size_t nIso = nuclideTab->entries();
        for ( size_t i = 0; i < nIso; ++i ) {
                auto* prop = nuclideTab->GetIsotopeByIndex(i);
                ionTab->GetIon(prop->GetAtomicNumber(),
                                prop->GetAtomicMass(),
                                prop->GetIsomerLevel());
        }

}
```


<br>
<br>

## RunAction.cc

To continue with the proper sourcing of the ions for each `IonTable`, #included `"G4IonTable.hh"` and created all the ions for the `IonTable` in `void RunAction::BeginOfRunAction(const G4Run*)`:

```bash
G4IonTable::GetIonTable()->CreateAllIon();
```


<br>
<br>



## GdNeutronHPCaptureFS.cc

- Replaced `theResult.Clear();` with:

```bash
auto * theFinalState = theResult.Get();

if (!theFinalState) {
                theResult.Put(new G4HadFinalState);
                theFinalState = theResult.Get();
                theFinalState->Clear();
                cout << "theFinalState was null" << endl;
 }
```



- Replaced `theResult.AddSecondary(theOne);` with `theFinalState->AddSecondary(theOne);`

- Replaced `theResult.AddSecondary(theTwo);` with `theFinalState->AddSecondary(theTwo);`

- Replaced `theResult.SetStatusChange(stopAndKill);` with `theFinalState->SetStatusChange(stopAndKill);`

- Replaced `return &theResult;` with `return theFinalState;`

- To continue with the proper sourcing of the ions for each `IonTable`, it is necessary to use `CreateAllIon()` and a loop that retrieves particle information from a `nuclideTable`. Additionally, since `FindIon()` is ultimately used in each `IonTable` to find the Gd information, it is also necessary to properly include its new location (`FindIon()` was moved from `G4ParticleTable` to `G4IonTable`). Thus, #included the following files

```bash
#include “G4IonTable.hh”
#include "G4NuclideTable.hh"
```

and, wherever `FindIon()` is called, added:

```bash
G4ParticleTable* theTable1 = G4ParticleTable::GetParticleTable();

G4IonTable* tableOfIons1 = theTable1 ->GetIonTable();

tableOfIons1->CreateAllIon();

auto* nuclideTable1 = G4NuclideTable::GetNuclideTable();
nuclideTable1->SetThresholdOfHalfLife(0.0);
nuclideTable1->GenerateNuclide();
size_t nIso1 = nuclideTable1->entries();
for (size_t p = 0; p < nIso1; ++p) {
                auto* prop = nuclideTable1->GetIsotopeByIndex(p);
                tableOfIons1->GetIon(prop->GetAtomicNumber(), prop->GetAtomicMass(), prop->GetIsomerLevel());
}

…

tableOfIonsTemp1->FindIon(...)
```

(In this case, I opted to use numbers and temps to differentiate between the different moments in which `FindIon()` is called)


- Since the definition of `m2` shadows the definition of meters squared in `G4SystemOfUnits`, renamed `m1` and `m2` to `tempMass1` and `tempMass2` in order to avoid  any shadowing and maintain the same naming scheme

- Changed `tempMass1 = tempIon1->GetPDGMass();` for:

```bash
G4double tempMass1 = 0.0;

if (!tempIon1) {
cout << "tempIon1 is null" << endl;
}

tempMass1 = tempIon1->GetPDGMass();
```

- Added a “progress system” to give mc event by mc event updates about the progress of the simulation; first, including the necessary files and declaring the relevant variables

```bash
#include <cmath>
int eventCount = 0;
int totalEventNum = 100;
double percentProgress = 0.0;
double eventsPerSecond = log(static_cast<double>(totalEventNum) / 1.1547) / 2.2163; // Assuming linearly completed neutron capture events and a logarithmic processing relation with the total number of events in the simulation
double elapsedTime = 0.0;
double totalTime = static_cast<double>(totalEventNum) / eventsPerSecond; // In Seconds 
double remainingTime = totalTime; // Also in seconds
int daysElapsed = 0;
int daysRemaining = 0;
int hoursElapsed = 0;
int hoursRemaining = 0;
int minutesElapsed = 0;
int minutesRemaining = 0;
int secondsElapsed = 0;
int secondsRemaining = 0;
int numSecondsPerDay = 86400;
int numSecondsPerHour = 3600;
int numSecondsPerMinute = 60;
```

and, second, writing the necessary code just before returning in `G4HadFinalState * GdNeutronHPCaptureFS::ApplyYourself()`

```bash
// Just for the sake of knowing our current iteration
eventCount = eventCount + 1;
elapsedTime = static_cast<double>(eventCount) / eventsPerSecond;

// This should legibly express elapsedTime in days, hours, minutes and seconds 
daysElapsed = elapsedTime / numSecondsPerDay;
hoursElapsed = (static_cast<int>(elapsedTime) % numSecondsPerDay) / numSecondsPerHour;
minutesElapsed = (static_cast<int>(elapsedTime) % numSecondsPerHour) / numSecondsPerMinute;
secondsElapsed = static_cast<int>(elapsedTime) % numSecondsPerMinute;

remainingTime = totalTime - elapsedTime;

// This should legibly express remainingTime in days, hours, minutes and seconds 
daysRemaining = remainingTime / numSecondsPerDay;
hoursRemaining = (static_cast<int>(remainingTime) % numSecondsPerDay) / numSecondsPerHour;
minutesRemaining = (static_cast<int>(remainingTime) % numSecondsPerHour) / numSecondsPerMinute;
secondsRemaining = static_cast<int>(remainingTime) % numSecondsPerMinute;


percentProgress = (static_cast<double>(eventCount) / static_cast<double>(totalEventNum)) * 100.0;
cout << "Event number " <<  eventCount << " of " << totalEventNum << endl;
cout << "Estimated Elapsed time: " << daysElapsed << " days, " << hoursElapsed << " hours, " << minutesElapsed << " minutes, " << secondsElapsed <<  " seconds" << endl;
cout << "Estimated Time Remaining: " << daysRemaining << " days, " << hoursRemaining << " hours, " << minutesRemaining << " minutes, " << secondsRemaining <<  " seconds" << endl;
cout << "Current Progress: " << percentProgress << "%" << endl << endl << endl;
```


<br>
<br>


## Notes

- Even though the code for `GdNeutronHPCaptureFS.cc` shadows `theNeutron`, `theTarget`, and `aNucleus` (all from `GdNeutronHPCaptureFS.hh`, for some reason), it works fine.

- The adjustments that ensure the proper sourcing of the ions for `IonTable` require the inclusion of specific paths to `G4ENSDFSTATE1.0` within your `~/.bashrc` file (more specifically, between where you source your `geant4.sh` and your `geant4make.sh`), like so:

```bash
source …/software/geant4-10.1.3-install/bin/geant4.sh
export G4ENSDFSTATEDATA=".../software/geant4-10.1.3-install/share/Geant4-10.1.3/data/G4ENSDFSTATE1.0"
export G4ENSDFSTATE_PATH=.../software/geant4-10.1.3-install/share/Geant4-10.1.3/data/G4ENSDFSTATE1.0
source …/software/geant4-10.1.3-install/share/Geant4-10.1.3/geant4make/geant4make.sh
```


