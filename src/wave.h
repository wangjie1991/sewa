/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-02

  Description: This file provides a description and declearation of
               class Wave.

**************************************************************************/

#ifndef WAVE_H
#define WAVE_H

#include <string>
#include <vector>


/* feature types */
enum WaveFeat {
  kWaveFeatWaveSec,
  kWaveFeatSilentSec,
  kWaveFeatVocalSec,
  kWaveFeatLoudSec,
  kWaveFeatNoiseSec,
  kWaveFeatSilentRate,
  kWaveFeatLoudRate,
  kWaveFeatNoiseRate,
  kWaveFeatMax
};

struct WaveHeader {
  /* RIFF WAVE chunk, 12 bytes */
  unsigned char RIFF[4];
  unsigned int file_size;
  unsigned char WAVE[4];

  /* format chunk, 24 bytes */
  unsigned char fmt[4];
  unsigned int chunk_size;
  unsigned short compress;
  unsigned short channel_num;
  unsigned int sample_rate;
  unsigned int byte_per_sec;
  unsigned short block_align;
  unsigned short bits_per_sample;

  /* data chunk, 8 bytes */
  unsigned char data[4];
  unsigned int data_size;
};  

struct WaveData {
  char *buffer;
  unsigned int size;
  WaveHeader *wave_header;
};

extern const std::string g_wave_feat_name[];

/**************************************************************************
  This class provides the function to extract features from .wav file.
**************************************************************************/
class Wave {
 public:
  
/**************************************************************************
  Function: Wave
  Description: default constructor
  Input: none
  Output: none
  Return: none
  Notice: use constructor initializer
**************************************************************************/
  Wave();

/**************************************************************************
  Function: Wave
  Description: constructor with path_ initialization
  Input: const std::string &path
  Output: none
  Return: none
  Notice: use constructor initializer
**************************************************************************/
  explicit Wave(const std::string &path);

/**************************************************************************
  Function: ~Wave
  Description: destructor
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  ~Wave() { }

/**************************************************************************
  Function: WaveProc
  Description: wave main process
  Input: none
  Output: none
  Return: -1, failed.
           0, success.
  Notice: None
**************************************************************************/
  int WaveProc();

/**************************************************************************
  Function: Print
  Description: print the member value of object
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void Print() const;

/**************************************************************************
variable member function
**************************************************************************/
  //void set_file_path(const std::string &path) { file_path_ = path; }
  const std::string &name() const { return name_; }
  const std::vector<int> &feats() const { return feats_; }

 private:
/**************************************************************************
  Function: AllocWaveData
  Description: read file and copy to file_buffer_
  Input: none
  Output: none
  Return: -1, failed
           0, success
  Notice: alloc memory to *file_buffer_
**************************************************************************/
  int AllocWaveData();

/**************************************************************************
  Function: ReleaseWaveData
  Description: release buffer in file_buffer_
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void ReleaseWaveData();

/**************************************************************************
  Function: ExtractWaveFeat
  Description: Extract features from wave data
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void ExtractWaveFeat();

  /* full file name */
  std::string path_;

  /* file name without prefix and suffix */
  std::string name_;

  /* buffer and size of wave data */
  WaveData data_;

  /* wave features */
  std::vector<int> feats_;

};

#endif


