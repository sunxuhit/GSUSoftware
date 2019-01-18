### this is a code package for 2018 mRICH FermiLab Beam Test Analysis

#### macro
- fast analysis selected PMT/MPPC runs
- plot tdc QA for selected runs
- extract pre-tdc cut for selected runs

---------------

#### list
- run list for all configurations

---------------

#### src && include

##### BeamTest
- PixelMap
> pixel maps of PMT & MPPC
- Calibration
> calculate `number of photons`, `radius` and `angels`
>
> configure `quantum efficiency`, `noise level`, `transmisson of glass window` and `mirrir geometry`

##### Simulation

---------------

#### compile the code
- mkdir build
- cd ./build
- cmake /path/to/sorce/code
- cmake --build .
- you should get an executable file
