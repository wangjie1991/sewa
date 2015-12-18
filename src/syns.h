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
  Input: const std::string &wave_file
         const std::string &desc_file
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  explicit Syns(const std::string &wave_file, 
                const std::string &desc_file);

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
  Function: Select
  Description: write selected filename to select_file
  Input: const int select_hour
  Output: std::string &select_file
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
  int Select(const int select_hour, std::string &select_file) const;

/**************************************************************************
  variable member function
**************************************************************************/
  const std::map<std::string, int> &speaker_rate() const {
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
  std::string wave_file_;
  std::vector<std::string> wave_list_;

  /* The name of the file that lists the full path name of all desc files
     and the vector that stores these full path names */
  std::string desc_file_;
  std::vector<std::string> desc_list_;

  /* wave, desc and eval objects in vector */
  std::vector<Wave> waves_;
  std::vector<Desc> descs_;
  std::vector<Eval> evals_;

  /* statistical data of desc's speaker feature */
  std::map<std::string, int> speaker_count_;
  std::map<std::string, int> speaker_rate_;

};

#endif


