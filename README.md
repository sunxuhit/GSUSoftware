### this is a code package for 2018 mRICH FermiLab Beam Test Analysis

#### macro
- fast analysis selected PMT/MPPC runs
- plot tdc QA for selected runs
- extract pre-tdc cut for selected runs

---------------

#### list
- run list for all configurations

---------------

#### src

##### BeamTest
- PixelMap
> pixel maps of PMT & MPPC
- Calibration
> calculate `number of photons`, `radius` and `angels`
>
> configure `quantum efficiency`, `noise level`, `transmisson of glass window` and `mirror geometry`

##### Simulation
- Material
> quantum efficiency for PMT & MPPC
>
> pixelized histogram for detected photons
- Calibration
> calculate `number of photons`, `radius` and `angels`
>
> configure `quantum efficiency`, `noise level`, `transmisson of glass window` and `mirror geometry`

---------------

#### compile the code
- mkdir build
- cd ./build
- cmake ..
- make install 
- a shared lib can be found at ${PROJECT\_SOURCE\_DIR}/lib
