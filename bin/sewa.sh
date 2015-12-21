#!/bin/bash

# arguments
wave_dir=""
desc_dir=""
sewa_dir="sewa/"
sewa_hour=0

if [ $# -eq 4]
then
  wave_dir=$1
  desc_dir=$2
  sewa_dir=$3
  sewa_hour=$4
elif [ $# -eq 3]
then
  wave_dir=$1
  sewa_dir=$2
  sewa_hour=$3
elif [ $# -eq 0]
then
  break
else
  echo "Usage1: "$0" wave_dir desc_dir sewa_dir sewa_hour"
  echo "Usage2: "$0" wave_dir sewa_dir sewa_hour"
  echo "Usage3: "$0
  exit -1;
fi

# debug version output the analysis result and
# save the list directory.
debug=true
conf="config"
list_dir="list"

# var only use in this script
wave_list="$list_dir/wave.list"
desc_list="$list_dir/desc.list"
sewa_list="$list_dir/sewa.list"
sewa_log="$list_dir/sewa.log"
sewa_res=-1


# check arguments
if [ ! -d $wave_dir ]
then
  echo "Error: wave file directory is not exist."
  exit -1
fi

if [ $# -eq 4 && ! -d $desc_dir ]
then
  echo "Error: desc file directory is not exist."
fi

if [ $sewa_hour -le 0 ]
then
  echo "Error: select wave hour is less than or equal to zero."
  exit -1
fi

# make temporary file directory
if [ -d $list_dir ]
then
    rm -rf $list_dir
fi
mkdir $list_dir

# make wave list file
find $wave_dir -name "*.wav" > $wave_list
#wave_cnt=`cat $wave_list | wc -l`
#if [ $wave_cnt -eq 0 ]
if [ ! -s $wave_list ]
then
  echo "Error: NO wave files in wave directory."
  exit -1
fi

# make desc list file
if [ $desc_dir != "" ]
then
  find $desc_dir -name "*.wav" > $desc_list
  #desc_cnt=`cat $desc_list | wc -l`
  #if [ $desc_cnt -eq 0 ]
  if [ ! -s $desc_list ]
  then
    echo "Error: NO desc files in desc directory."
    exit -1
  fi
fi

# run the main program
if [ $debug && -s $desc_list ]
then
  sewa_res=`./sewa -f $conf -d $desc_list -l $sewa_log $wave_list $sewa_hour $sewa_list`
elif [ $debug && ! -s $desc_list ]
then
  sewa_res=`./sewa -f $conf -l $sewa_log $wave_list $sewa_hour $sewa_list`
elif [ ! $debug && -s $desc_list ]
then
  sewa_res=`./sewa -f $conf -d $desc_list $wave_list $sewa_hour $sewa_list`
elif [ ! $debug && ! -s $desc_list ]
then
  sewa_res=`./sewa -f $conf $wave_list $sewa_hour $sewa_list`
fi

if [ $sewa_res -eq 0 ]
then
  echo "Select wave files success."
else
  echo "Select wave files failed."
  exit -1
fi

# copy files in sewa.list to sewa directory
while read line
do
    name=${line##*/}
    path=${sewa_dir}${path}

    dir=${path%/*}
    if [ ! -d $dir ]
    then
      mkdir -p $dir
    fi

    cp $line $path
done < $sewa_list
echo "Copy wave files success"

# rm temporary files in debug mode
if [ ! $debug ]
  rm -rf $list_dir
fi


