/**************************************************************************

  Copyright 2015-2016 Pachira

  Author : wangjie

  Date : 2015-11-02

  Description : This file provides the definitions of class Wave.

**************************************************************************/

#include "wave.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

#include "util.h"


/* feature type name */
extern const std::string g_wave_feat_name[kWaveFeatMax] = {
  "wave_second",
  "silent_second",
  "vocal_second",
  "loud_second",
  "noise_second",
  "silent_rate",
  "loud_rate",
  "noise_rate"
};

short g_silent_limit = 100;
float g_silent_rate = 0.6;    //60%

short g_loud_thres = 26000;   //80%
short g_loud_num = 8;

/**************************************************************************
  initialize feats_ and data_
**************************************************************************/
Wave::Wave() : feats_(kWaveFeatMax) {
  memset((void*)&data_, 0, sizeof(data_));
}

/**************************************************************************
  if file is not end with .wav, the parameter path is considered invalid.
**************************************************************************/
Wave::Wave(const std::string &path)
    : path_(path), feats_(kWaveFeatMax) {
  memset((void*)&data_, 0, sizeof(data_));
  if (!IsSuffixLegal(path_, g_suffix_wav)){
    path_ = "";
  }
}

/**************************************************************************
  First Extract file name from path. Second read .wav file to get data to
  buffer, Then extract all the features of wave and at last release the
  buffer.
**************************************************************************/
int Wave::WaveProc() {
  if (0 != ExtractName(path_, name_)) {
    std::cerr << "WaveProc error: file " << path_
              << " ExtractName failed" << std::endl;
    return -1;
  }

  if (0 != AllocWaveData()) {
    ReleaseWaveData();
    std::cerr << "WaveProc error: file " << path_
              << " AllocWaveData failed" << std::endl;
    return -1;
  }

  ExtractWaveFeat();

  ReleaseWaveData();

  return 0;
}

/**************************************************************************
  --------
  path : /home/xxx/file.wav
  name : file
  feat-speaker : A
  feat-office : 1111
  ....
**************************************************************************/
void Wave::Print() const {
  std::cout << "--------" << std::endl
            << "path : " << path_ << std::endl
            << "name : " << name_ << std::endl;

  std::vector<std::string>::size_type index;
  for (index = 0; index != feats_.size(); ++index) {
    std::cout << "feat-" << g_wave_feat_name[index] << " : "
              << feats_[index] << std::endl;
  }

  return;
}

/**************************************************************************
  Open file with STL ifstream.
  If error occurs, print error message with iostream and return -1.
  Copy data to buffer and then close file to release file resource.
**************************************************************************/
int Wave::AllocWaveData() {
  /* get file stream */
  std::ifstream stream(path_.c_str(), std::ifstream::binary);
  if (!stream) {
    std::cerr << "AllocWaveData error: can't open file " 
              << path_ << std::endl;
    return -1;
  }

  /* get length of file */
  stream.seekg(0, stream.end);
  int size = stream.tellg();
  stream.seekg(0, stream.beg);

  if (size <= WAVE_HEAD_LEN) {
    std::cerr << "AllocWaveData error: file "
              << path_ << " length is error" << std::endl;
    return -1;
  }

  /* read data to buffer */
  char *buffer = new char[size];
  stream.read(buffer, size);
  stream.close();

  /* set data_ */
  data_.buffer = buffer;
  data_.size = size;

  data_.wave_header = (WaveHeader *)(void *)data_.buffer;
  if (data_.wave_header->data_size != data_.size - WAVE_HEAD_LEN) {
    std::cerr << "AllocWaveData error: file "
              << path_ << " wav format is error" << std::endl;
    return -1;
  }

  return 0;
}

/**************************************************************************
  delete buffer and initial data_
**************************************************************************/
void Wave::ReleaseWaveData() {
  if (data_.buffer != NULL) {
    delete[] data_.buffer;
  }
  memset((void *)&data_, 0, sizeof(data_));
  return;
}

/**************************************************************************
  calculate the signal properties for every single second.
**************************************************************************/
void Wave::ExtractWaveFeat() {
  unsigned int data_size = data_.wave_header->data_size;
  unsigned int byte_per_sec = data_.wave_header->byte_per_sec;
  unsigned int sample_rate = data_.wave_header->sample_rate;
  unsigned short channel_num = data_.wave_header->channel_num;
  unsigned short bits_per_sample = data_.wave_header->bits_per_sample;

  unsigned int sample_num = sample_rate * channel_num;
  unsigned short bytes_per_sample = bits_per_sample / 8;

  unsigned int wave_sec = data_size / byte_per_sec + 1;
  unsigned int silent_sec = 0;
  unsigned int loud_sec = 0;

  /* extract feature in every second */
  char *wave_buf = data_.buffer + WAVE_HEAD_LEN;
  for (unsigned int second = 0; second < wave_sec - 1; ++second) {
    /* statistical variables */
    unsigned int silent_num = 0;
    unsigned int loud_num = 0;

    /* calculate every sample point */
    char *sample_buf = wave_buf;
    for (unsigned int sample_cnt = 0; sample_cnt < sample_num; 
         ++sample_cnt) {
      /* get sample point value and convert to ratio */
      short sample = 0;
      if (bytes_per_sample == 2) {
        sample = *(short *)(void *)sample_buf;
      }
      else if (bytes_per_sample == 1) {
        sample = 0;
      }
      else {
      }

      /* calculate statistial variables by sample point ratio */
      if ((sample >= -g_silent_limit) && 
          (sample <= g_silent_limit)) {
        silent_num++;
      }
      if ((sample <= -g_loud_thres) || 
          (sample >= g_loud_thres)) {
        loud_num++;
      }

      sample_buf += bytes_per_sample;
    }

    /* judge the frame according to statistical variables */
    if (silent_num > (g_silent_rate * sample_num)) {
      silent_sec++;
    }
    if ((int)loud_num > g_loud_num) {
      loud_sec++;
    }
    
    wave_buf += byte_per_sec;
  }

  feats_[kWaveFeatWaveSec] = wave_sec;
  feats_[kWaveFeatSilentSec] = silent_sec;
  feats_[kWaveFeatLoudSec] = loud_sec;
  feats_[kWaveFeatVocalSec] = wave_sec - silent_sec;
  /* round */
  feats_[kWaveFeatSilentRate] = ((silent_sec * 1000) / wave_sec + 5) / 10;
  feats_[kWaveFeatLoudRate] = ((loud_sec * 1000) / wave_sec + 5) / 10;

  return;
}


