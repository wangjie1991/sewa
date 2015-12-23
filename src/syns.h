/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-06

  Description: This file provides a description and declearation of class
               Syns.

**************************************************************************/

#ifndef SYNS_H
#define SYNS_H

#include <string>
#include <vector>
#include <map>

#include "wave.h"
#include "desc.h"
#include "eval.h"

using namespace std;

/**************************************************************************
  This class provides the synthesized function to select files in batch 
  through wave, desc and eval objects.
**************************************************************************/
class Syns {
 public:
  
/**************************************************************************
  Function: Syns
  Description: default constructor
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  Syns() { }

/**************************************************************************
  Function: Syns
  Description: constructor with file name initialization
  Input: const string &wave_file
         const string &desc_file
         const string &sewa_file
         const string &sewa_log
         const int    sewa_hour
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  explicit Syns(const string &wave_file,
                const string &desc_file,
                const string &sewa_file,
                const string &sewa_log,
                const int sewa_hour);

/**************************************************************************
  Function: ~Syns
  Description: destructor
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  ~Syns() { }

/**************************************************************************
  Function: SynsProc
  Description: main process function
  Input: none
  Output: none
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
  int SynsProc();

/**************************************************************************
  variable member function
**************************************************************************/
  const map<string, int> &speaker_rate() const {
    return speaker_rate_;
  }

 private:

/**************************************************************************
  Function: GenWaveList
  Description: push file name to wave_list_
  Input: none
  Output: none
  Return: -1, failed
           0, success
  Notice: If suffix is not .wav, it won't push to wave_list_
**************************************************************************/
  int GenWaveList();

/**************************************************************************
  Function: GenDescList
  Description: push file name to desc_list_
  Input: none
  Output: none
  Return: -1, failed
           0, success
  Notice: If suffix is not .des, it won't push to desc_list_
**************************************************************************/
  int GenDescList();

/**************************************************************************
  Function: GenVectors
  Description: push objects to vectors
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void GenVectors();

/**************************************************************************
  Function: Select
  Description: write selected filename to sewa_file
  Input: none
  Output: none
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
  int Select() const;

/**************************************************************************
  Function: CountDescSpeaker
  Description: count desc according to speaker
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void CountDescSpeaker();

/**************************************************************************
  Function: FindDescByWave
  Description: find the desc which has the same name with wave
  Input: const Wave *const wave
  Output: none
  Return: const Desc *const, if not find return NULL
  Notice: none
**************************************************************************/
  const Desc *const FindDescByWave(const Wave *const wave) const;

  /* The name of the file that lists the full path name of all wave files
     and the vector that stores these full path names */
  string wave_file_;
  vector<string> wave_list_;

  /* The name of the file that lists the full path name of all desc files
     and the vector that stores these full path names */
  string desc_file_;
  vector<string> desc_list_;

  string sewa_file_;
  string sewa_log_;
  int sewa_hour_;

  /* wave, desc and eval objects in vector */
  vector<Wave> waves_;
  vector<Desc> descs_;
  vector<Eval> evals_;

  /* statistical data of desc's speaker feature */
  map<string, int> speaker_count_;
  map<string, int> speaker_rate_;

};

#endif


