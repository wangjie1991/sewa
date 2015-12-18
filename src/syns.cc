/**************************************************************************

  Copyright 2015-2016 Pachira

  Author : wangjie

  Date : 2015-11-06

  Description : This file provides the definition of class Syns.

**************************************************************************/

#include "syns.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>

#include "util.h"


/**************************************************************************
  syns initial
**************************************************************************/
Syns::Syns(const std::string &wave_file, const std::string &desc_file)
    : wave_file_(wave_file), desc_file_(desc_file) { }

/**************************************************************************
  desc files can be empty while wave files must be exist.
**************************************************************************/
int Syns::SynsProc() {

  if (0 != GenWaveList()) {
    std::cerr << "SynsProc error: GenWaveList failed" << std::endl;
    return -1;
  }

  if (0 != GenDescList()) {
    std::cout << "SynsProc warning: GenDescList failed" << std::endl;
  }

  GenVectors();

  return 0;
}

/**************************************************************************
**************************************************************************/
int Syns::Select(const int select_hour, std::string &select_file) const {
  /* get file stream */
  std::ofstream stream(select_file.c_str()); 
  if (!stream) {
    std::cerr << "Select error: can't open file " 
              << select_file << std::endl;
    return -1;
  }

  /* walk evals_ */
  int select_second = 0;
  int remain_second = select_hour * 3600;

  std::vector<Eval>::const_iterator iter;
  for (iter = evals_.begin(); iter != evals_.end(); ++iter) {
    const Eval &eval = *iter;
    const Wave *wave = eval.wave();
    int second = wave->feats()[kWaveFeatWaveSec];
    
    select_second += second;
    remain_second -= second;

    if (remain_second < 0) {
      break;
    } else {
      stream << wave->name() << "\t";
      stream << eval.priority() << "\t";
      stream << second << "\t";
      stream << remain_second << std::endl;
    }
  }

  if (remain_second > 0) {
    std::cerr << "Select error: selected files is less than "
              << select_hour << " hours." << std::endl;
    return -1;
  }

  return 0;
}

/**************************************************************************
  Open file with STL ifstream.
  If error occurs, print error message and return -1.
**************************************************************************/
int Syns::GenWaveList() {
  /* check wave_file_ */
  if (wave_file_.empty()) {
    std::cerr << "GenWaveList error: wave_file_=\"\"" << std::endl;
    return -1;
  }

  /* get file stream */
  std::ifstream stream(wave_file_.c_str()); 
  if (!stream) {
    std::cerr << "GenWaveList error: can't open file " 
              << wave_file_ << std::endl;
    return -1;
  }

  /* read stream and getline */
  std::string line;
  while(!stream.eof()) {
    std::getline(stream, line);
    if (IsSuffixLegal(line, g_suffix_wav)) {
      wave_list_.push_back(line);
    }
  }

  stream.close();

  return 0;
}

/**************************************************************************
  Open file with STL ifstream.
  If error occurs, print error message iostream and return -1.
**************************************************************************/
int Syns::GenDescList() {
  /* check desc_file_ */
  if (desc_file_.empty()) {
    std::cout << "GenDescList warning: desc_file_=\"\"" << std::endl;
    return -1;
  }

  /* get file stream */
  std::ifstream stream(desc_file_.c_str()); 
  if (!stream) {
    std::cerr << "GenDescList error: can't open file " 
              << desc_file_ << std::endl;
    return -1;
  }

  /* read stream and getline */
  std::string line;
  while(!stream.eof()) {
    std::getline(stream, line);
    if (IsSuffixLegal(line, g_suffix_des)) {
      desc_list_.push_back(line);
    }
  }

  stream.close();

  return 0;
}

/**************************************************************************
  decrease sort function for std::sort
**************************************************************************/
bool EvalDec(const Eval &eval1, const Eval &eval2) {
  return eval1.priority() > eval2.priority();
}

/**************************************************************************
  First push wave and desc object to vector and count desc set.
  Second push eval object according to wave and desc objects.
**************************************************************************/
void Syns::GenVectors() {

  std::vector<std::string>::const_iterator liter;
  /* process all wave files */
  for (liter = wave_list_.begin(); liter != wave_list_.end(); ++liter) {
    Wave wave(*liter);
    if (0 == wave.WaveProc()) {
      waves_.push_back(wave);
#ifdef _DEBUG_
      wave.Print();
#endif
    }
  }
  /* process all desc files */
  for (liter = desc_list_.begin(); liter != desc_list_.end(); ++liter) {
    Desc desc(*liter);
    if (0 == desc.DescProc()) {
      descs_.push_back(desc);
#ifdef _DEBUG_
      desc.Print();
#endif
    }
  }

  /* desc count process */
  CountDescSpeaker();

  /* evaluate the priority of file */
  std::vector<Wave>::const_iterator witer;
  for (witer = waves_.begin(); witer != waves_.end(); ++witer) {
    const Wave *const wave = &(*witer);
    const Desc *const desc = FindDescByWave(wave);

    Eval *eval = NULL;
    if (desc == NULL) {
      eval = new Eval(wave);
    } else {
      eval = new Eval(wave, desc, this);
    }

    if (0 == eval->EvalProc()) {
      evals_.push_back(*eval);
#ifdef _DEBUG_
      eval->Print();
#endif
    }

    if (eval != NULL) {
      delete eval;
    }
  }

  std::sort(evals_.begin(), evals_.end(), EvalDec);

  return;
}

/**************************************************************************
  Count speaker in descs_ and calculate rate.
  Note the rate is int and is multiplied by 100.
**************************************************************************/
void Syns::CountDescSpeaker() {
  
  std::vector<Desc>::const_iterator diter;
  std::map<std::string, int>::iterator miter;

  for (diter = descs_.begin(); diter != descs_.end(); ++diter) {
    const std::string &speaker = diter->feats()[kDescFeatSpeaker];
    miter = speaker_count_.find(speaker);
    if (miter != speaker_count_.end()) {
      miter->second++;
    } else {
      speaker_count_.insert(std::pair<std::string,int>(speaker, 0));
    }
  }

  int size = descs_.size();
  for (miter = speaker_count_.begin();
       miter != speaker_count_.end();
       ++miter) {
    speaker_rate_[miter->first] = (miter->second * 100) / size;
  }

  return;
}

/**************************************************************************
  walk descs_ and compare name with wave
**************************************************************************/
const Desc *const Syns::FindDescByWave(const Wave *const wave) const {
  const std::string &wave_name = wave->name();

  std::vector<Desc>::const_iterator iter;
  for (iter = descs_.begin(); iter != descs_.end(); ++iter) {
    const std::string &desc_name = iter->name();
    if (wave_name == desc_name) {
      break;
    }
  }

  if (iter != descs_.end()) {
    return &(*iter);
  } else {
    return NULL;
  }
}


