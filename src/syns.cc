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
#include <iomanip>

#include "util.h"

using namespace std;


/**************************************************************************
  syns initial
**************************************************************************/
Syns::Syns(const string &wave_file, const string &desc_file,
           const string &sewa_file, const string &sewa_log, 
           const int sewa_hour)
    : wave_file_(wave_file), desc_file_(desc_file), sewa_file_(sewa_file), 
      sewa_log_(sewa_log), sewa_hour_(sewa_hour) { }

/**************************************************************************
  desc files can be empty while wave files must be exist.
**************************************************************************/
int Syns::SynsProc() {

  if (0 != GenWaveList()) {
    cerr << "syns error: GenWaveList failed" << endl;
    return -1;
  }

  if (!desc_file_.empty() && (0 != GenDescList())) {
    cerr << "syns error: GenDescList failed" << endl;
    return -1;
  }

  GenVectors();
  if (0 != Select()) {
    cerr << "syns error: Select failed" << endl;
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
    cerr << "syns error: wave list file name is empty" << endl;
    return -1;
  }

  /* get file stream */
  ifstream stream(wave_file_.c_str()); 
  if (!stream) {
    cerr << "syns error: can't open file " << wave_file_ << endl;
    return -1;
  }

  /* read stream and getline */
  string line;
  while(!stream.eof()) {
    getline(stream, line);
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
  /* get file stream */
  ifstream stream(desc_file_.c_str()); 
  if (!stream) {
    cerr << "syns error: can't open file " << desc_file_ << endl;
    return -1;
  }

  /* read stream and getline */
  string line;
  while(!stream.eof()) {
    getline(stream, line);
    if (IsSuffixLegal(line, g_suffix_des)) {
      desc_list_.push_back(line);
    }
  }

  stream.close();

  return 0;
}

/**************************************************************************
  decrease sort function for sort
**************************************************************************/
bool EvalDec(const Eval &eval1, const Eval &eval2) {
  return eval1.priority() > eval2.priority();
}

/**************************************************************************
  First push wave and desc object to vector and count desc set.
  Second push eval object according to wave and desc objects.
**************************************************************************/
void Syns::GenVectors() {

  vector<string>::const_iterator liter;
  /* process all wave files */
  for (liter = wave_list_.begin(); liter != wave_list_.end(); ++liter) {
    Wave wave(*liter);
    if (0 == wave.WaveProc()) {
      waves_.push_back(wave);
    }
  }

  /* process all desc files */
  for (liter = desc_list_.begin(); liter != desc_list_.end(); ++liter) {
    Desc desc(*liter);
    if (0 == desc.DescProc()) {
      descs_.push_back(desc);
    }
  }

  /* desc count process */
  CountDescSpeaker();

  /* evaluate the priority of file */
  vector<Wave>::const_iterator witer;
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
    }

    if (eval != NULL) {
      delete eval;
    }
  }

  sort(evals_.begin(), evals_.end(), EvalDec);

  return;
}

/**************************************************************************
  select sewa_hour wave files and write path to sewa_file
  detail information write to sewa_log
**************************************************************************/
int Syns::Select() const {
  /* check params */
  if (sewa_hour_ <= 0) {
    cerr << "syns error: sewa hour is illegal." << endl;
    return -1;
  }
  if (sewa_file_.empty()) {
    cerr << "syns error: sewa list file name is empty" << endl;
    return -1;
  }

  /* get file stream */
  ofstream f_stream(sewa_file_.c_str()); 
  if (!f_stream) {
    cerr << "syns error: can't open file " << sewa_file_ << endl;
    return -1;
  }

  /* get log stream */
  ofstream l_stream;
  if (!sewa_log_.empty()) {
    l_stream.open(sewa_log_.c_str()); 
    if (!l_stream) {
      cerr << "syns error: can't open file " << sewa_log_ << endl;
      return -1;
    }
  }

  /* walk evals_ */
  int select_second = 0;
  int remain_second = sewa_hour_ * 3600;
  int last_priority = 0;
  bool drawline = false;

  vector<Eval>::const_iterator iter;
  for (iter = evals_.begin(); iter != evals_.end(); ++iter) {
    const Eval &eval = *iter;
    const Wave *wave = eval.wave();
    const Desc *desc = eval.desc();
    int second = wave->feats()[kWaveFeatWaveSec];
    int priority = eval.priority();

    if ((last_priority > 0) && (priority == 0)) {
      drawline = true;
      last_priority = priority; 
    }
    if ((priority > 0) && (remain_second > 0)) {
      select_second += second;
      remain_second -= second;

      if (remain_second <= 0) {
        drawline = true;
        select_second = sewa_hour_ * 3600;
        remain_second = 0;
      }

      f_stream << wave->path() << endl;
    }

    if (drawline) {
      if (l_stream) {
        l_stream << "--------------------------------------------------\n" 
                 << endl;
      }
      drawline = false;
    }

    float minute = second / 60.0;
    float select_hour = select_second / 3600.0;
    float remain_hour = remain_second / 3600.0;

    if (l_stream) {
      l_stream << wave->name() << endl
               << "syns-feat" << endl
               << "\tpriority: " << setprecision(4) << priority << endl
               << "\tminutes: " << setprecision(3) << minute << "m" << endl
               << "\tselect_hour: " << setprecision(4) << select_hour
               << "h" << endl
               << "\tremain_hour: " << setprecision(4) << remain_hour
               << "h" << endl;
      wave->Print(l_stream);
      if (desc != NULL) {
        desc->Print(l_stream);
      }
      eval.Print(l_stream);
      l_stream << endl;
    }
  }

  if (remain_second > 0) {
    cerr << "syns error: select hours is less than " 
         << sewa_hour_ << endl;
    return -1;
  }

  return 0;
}

/**************************************************************************
  Count speaker in descs_ and calculate rate.
  Note the rate is int and is multiplied by 100.
**************************************************************************/
void Syns::CountDescSpeaker() {
  
  vector<Desc>::const_iterator diter;
  map<string, int>::iterator miter;

  for (diter = descs_.begin(); diter != descs_.end(); ++diter) {
    const string &speaker = diter->feats()[kDescFeatSpeaker];
    miter = speaker_count_.find(speaker);
    if (miter != speaker_count_.end()) {
      miter->second++;
    } else {
      speaker_count_.insert(pair<string,int>(speaker, 0));
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
  const string &wave_name = wave->name();

  vector<Desc>::const_iterator iter;
  for (iter = descs_.begin(); iter != descs_.end(); ++iter) {
    const string &desc_name = iter->name();
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


