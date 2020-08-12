[![Build Status](https://travis-ci.org/UMD-AOSC/OISSTv3.svg?branch=develop)](https://travis-ci.org/UMD-AOSC/OISSTv3)

# OISSTv3

Prototype for the Optimal Interpolation Sea Surface Temperature v3 (OISSTv3)

A variational data assmilation system built on the JCSDA Joint Effort for Data Assimilation Integration (JEDI). This project is just beginning, and further documentation will be created as the project proresses.

## Getting Started
The following steps assume you already have one of the JEDI containers setup and working on your computer. See the [JEDI Documentation](https://jointcenterforsatellitedataassimilation-jedi-docs.readthedocs-hosted.com/en/latest/) for additional details on how to do this.

> ***NOTE: until JEDI is publicly released, anyone trying to build OISSTv3 must have access to the private JEDI repositories!***

To build OISSTv3 and run the continuous integration tests:
```
git clone https://github.com/UMD-AOSC/OISSTv3.git oisst.src
mkdir -p oisst.build
cd oisst.build
ecbuild ../oisst.src/bundle
cd oisst
make -j <nprocs>
ctest
```
