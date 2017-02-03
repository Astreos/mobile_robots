# Add-on for coupling ROBOTRAN with LMGC90

LMGC90 is software dedicated to the modelling of granular media (or more
generally divided media). It is based on the Discrete Element Method 
(DEM) which enable to acount for each grain of the media. MOre precisely,
it implement the so-called Non-Smooth Contact Dynamics approach (NSCD).
For this purpose, it encompasses powerfull methods for dealing with contact,
in particular detecting contact, solving contact equations, ...  

This add-on aims at coupling Robotran with LMGC90 in order to model
multibody systems interacting with granular material.


## Building the MBsysC modules and the add-on

First, it is needed to build the MBsysC modules. Then, the mbs_lmgc add-on
is compiled and linked to MBsysC libraries

### Build MBsys modules
If it was not done before, compile the MBsysC modules:
* Go to the MBsysC/MBsysC folder and create a build directory (next to the mbs_common folder).
It must be called `build`, otherwise manual configuration will be needed at next step.
* Create the cmake project, source being in mbs_common: `cmake ../mbs_common`
* Build MBsysC modules: `make`

### Build the mbs_lmgc add-on
* Go to te add-on folder and create a build directory:
 `cd MBsysC/MBsysC/mbs_common/mbs_add_on/mbs_lmgc  
  mkdir build`
* Create the CMake project: `cmake ..`
* Build the project `make`
This should build 2 libraries:
 * the mbs_lmgc binding
 * the python interface that enable to configure the bonding from a python shell

## Downloading and building LMGC90

### Downloading 
 * Download LMGC90 from https://git-xen.lmgc.univ-montp2.fr/lmgc90/lmgc90_user
   (or from here for the development version: 
   https://git-xen.lmgc.univ-montp2.fr/lmgc90/lmgc90_dev)


### Build LMGC90 and link with the Robotran project

 * Adapt the CMake properties when compiling LMGC90. From the build
   directory of LMGC90 (assuming `cmake ../path/to/src/of/LMGC90` was
   already done):  
  ` cmake . -DEXT_MBS_VERSION=Robotran -DEXT_MBS_FILE=.../MBsysC/MBsysC/mbs_common/mbs_add_on/mbs_lmgc/build/robotran_lmgc90.cfg`
 * (re-)Build LMGC90:  
  `make`
 
### Building and running a project

### Build the Robotran project

* Go to the workR folder of the project and creat the cmake project in a
build folder  
```cd .../MBsysC/MBprojects/MultiplePendulum/workR 
mkdir build  
cd build  
cmake ..  -DROBOTRAN_SOURCE_DIR=...path/to/MBsysC/MBsysC```
* Check that the FLAG_SEPARATE_BUILD is set to ON (or set to ON if it is not)
```cmake . -DFLAG_SEPARATE_BUILD=ON```
* Build the project
```make```

   
### Running the simulations

* Go to the folder in which are located the python scripts (for
instance the workR/LMGC90 folder of the project):  
```cd .../MBsysC/MBprojects/MultiplePendulum/workR/lmgc90```

* Add LMGC90 and Robotran-LMGC90 add on folder to the Python path:  
```export PYTHONPATH=.../LMGC90/build/:.../MBsysC/mbs_common/mbs_add_on/mbs_lmgc/build```

* Run the python script:  
```python command_MBS_LMGC.py```
