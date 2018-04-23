### compile the code
`./compile`

### run the code
**make sure TileIds.txt file is in the working directory**

`sudo root -l FEBDAQMULT_GSU.C`

triggers are setup in `CITIROC_SC_PROFILE1.txt`

#### Start DAQ
> start taking data

#### Stop DAQ
> stop taking data

#### High Voltage
> flash light for High Voltage status

#### Test Run
> start to standard test run
> 
> when running time reached number (in seconds) below stop the run and save data to a ROOT TTree
>
> default threshold is 300
>
> default running time is 900s
>
> default trigger is ch0 and ch1

### Output
all root file will be saved under LocalRootBase and Dorpbox folder

all txt file will be saved under LocalDataBase and Dorpbox folder include Tested\_TileIds and information of tested tiles

### expert mode

following fuctions are hide for expert only

#### Update Config
> read default configuration file

#### Reset Histo
> reset histograms

#### SetThreshold DAC1
> set threshold to the number above

#### Save data tree
> save all data to a ROOT TTree based on click time under LocalRootBase
>
> save individual tile adc histogram to a ROOT file under LocalRootBase
>
> save relevant information for individual tile under LocalDataBase

