# Project 3 AK8PP
This project implements EREW PRAM algorithm and its scalable version using cpp with open mp. Created and tested on ubuntu.

## Prerequisities
* cmake >= 3.21
* openMP library (sudo apt install libomp-dev)

## Compilation
```
mkdir build
cd build
cmake ..
make -j
```

## Parameters
* ```--mode=<mode>``` - chose mode, either limited or scalable
* ```--size=<uint>``` - chose size for scalable mode (have to be divisible by 32)