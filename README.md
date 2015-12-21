This is a program to select better transcript wave files from original wave
data set.

Usage: ./sewa.sh [OPTION]


#### USAGE METHON ONE :
$ ./sewa.sh wave_dir desc_dir sewa_dir sewa_hour

arguments:
* wave_dir : directory of standard wave files
* desc_dir : directory of wave description files
* sewa_dir : directory of selected transcript wave files
* sewa_hour: hour time of selected transcript wave files

#### USAGE METHON TWO:
$ ./sewa.sh wave_dir sewa_dir sewa_hour

arguments:
* wave_dir : directory of standard wave files
* sewa_dir : directory of selected transcript wave files
* sewa_hour: hour time of selected transcript wave files

#### USAGE METHON THREE:
$ ./sewa.sh

Note : wave_dir sewa_dir and sewa_hour must be set in file sewa.sh


