#!/bin/ksh
# split_list.sh file_list.txt  --> loops over file_list.txt textfile which contains
# a list of files. file_list.txt is then splittet with split level nsplit
if [ $# -eq 1 ] 
then
  filecounter=0
  nsplit=1
  jobcounter=0
  filename=./condor/Split_picoDst_Run14AuAu200GeV_
  fileb=_
  fileend=.list
  fullfilename="$filename$jobcounter$fileend"
  echo fullfilename = $fullfilename
  rm ${fullfilename}
  for item in `cat $1`
  do 
    echo FILE name = $item
    echo ${item}>>${fullfilename}
    let "filecounter=filecounter+1"
    if [ $filecounter -ge $nsplit ]
    then
      let "filecounter=0"
      let "jobcounter=jobcounter+1"
      fullfilename="$filename$jobcounter$fileend"
      echo fullfilename = $fullfilename
      rm ${fullfilename}
      echo N Counter: ${jobcounter}
    fi

    #echo "**************************************************************************************"
    #echo   FILE COUNTER: ${filecounter}
    #echo "**************************************************************************************"

  done
else
  echo "**************************************************************************************"
  echo "\t" usage: mycompare text-file1 text-file2 
  echo 
  echo "\t\t" returns lines of text-file1 which are not contained in text-file2
  echo "**************************************************************************************" 
fi
