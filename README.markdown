### compile the code
`./compile`

### run the code
**make sure TileIds.txt file is in the working directory**

`sudo root -l FEBDAQMULT_GSU.C`

if you do **NOT** use `Test Run` please set the number below it to 0

### GUI
**always select trigger fisrt**

### Output
all root file will be saved under LocalRootBase

all txt file will be saved under LocalDataBase include Tested\_TileIds and information of tested tiles

#### Update Config
> read default configuration file

#### Start DAQ
> start taking data

#### Stop DAQ
> stop taking data

#### Reset Histo
> reset histograms

#### High Voltage
> flash light for High Voltage status

#### SetThreshold DAC1
> set threshold to the number above

#### Save data tree
> save all data to a ROOT TTree based on click time under LocalRootBase
>
> save individual tile adc histogram to a ROOT file under LocalRootBase
>
> save relevant information for individual tile under LocalDataBase

#### Test Run
> start to standard test run
> 
> when running time reached number (in seconds) below stop the run and save data to a ROOT TTree

