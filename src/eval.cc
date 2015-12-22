/**************************************************************************

  Copyright 2015-2016 Pachira

  Author : wangjie

  Date : 2015-11-04

  Description : This file provides the definitions of class Eval.

**************************************************************************/

#include "eval.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include "wave.h"
#include "desc.h"
#include "syns.h"

using namespace std;


/* dimention name */
extern const string g_dim_name[kDimMax] = {
  "wave",
  "desc"
};

/* wide rate, narrow value */
int g_wave_feat_min[kWaveFeatMax]
                   = {60, 0, 0, 0, 0};
int g_wave_feat_max[kWaveFeatMax]
                   = {300, 120, 20, 60, 10};

int g_wave_weight[kWaveFeatMax] = {15, 15, 20, 30, 20};
int g_desc_weight[kDescFeatMax] = {100, 0, 0, 0, 0};
int g_dim_weight[kDimMax] = {50, 50};

/**************************************************************************
  eval initial
**************************************************************************/
Eval::Eval() 
    : wave_(NULL), desc_(NULL), syns_(NULL), wave_scores_(kWaveFeatMax),
      desc_scores_(kDescFeatMax), dim_scores_(kDimMax), priority_(0) { }

/**************************************************************************
  eval initial
**************************************************************************/
Eval::Eval(const Wave *const wave)
    : wave_(wave), desc_(NULL), syns_(NULL), wave_scores_(kWaveFeatMax),
      desc_scores_(kDescFeatMax), dim_scores_(kDimMax), priority_(0) { }

/**************************************************************************
  eval initial
**************************************************************************/
Eval::Eval(const Wave *const wave, const Desc *const desc, 
           const Syns *const syns)
    : wave_(wave), desc_(desc), syns_(syns), wave_scores_(kWaveFeatMax),
      desc_scores_(kDescFeatMax), dim_scores_(kDimMax), priority_(0) { }

/**************************************************************************
  call the wave and desc evaluate function and sum the score
**************************************************************************/
int Eval::EvalProc() {

  if (wave_ == NULL) {
    cerr << "eval error: wave_ is NULL" << endl;
  }
  EvalWave();

  if ((desc_ != NULL) && (syns_ != NULL)) {
    EvalDesc();
  }

  priority_ = 0;
  vector<int>::size_type index = 0;
  for (index = 0; index < kDimMax; ++index) {
    priority_ += g_dim_weight[index] * dim_scores_[index];
  }

  return 0;
}

/**************************************************************************
  wave-score
    wave_second: XX
    silent_second: XX
    loud_second: XX
    silent_rate: XX
    loud_rate: XX
  desc-score
    speaker: XX
    office: XX
    platform: XX
    format: XX
    business: XX
  dim-score
    wave: XX
    desc: XX
**************************************************************************/
void Eval::Print(ofstream &ofs) const {
  if (!ofs) {
    return;
  }

  vector<int>::size_type index = 0;

  /* print wave_scores_ */
  ofs << "wave-score" << endl;
  for (index = 0; index != wave_scores_.size(); ++index) {
    ofs << "\t" << g_wave_feat_name[index] << ": "
        << wave_scores_[index] << endl;
  }

  /* print desc_scores_ */
  ofs << "desc-score" << endl;
  for (index = 0; index != desc_scores_.size(); ++index) {
    ofs << "\t" << g_desc_feat_name[index] << ": "
        << desc_scores_[index] << endl;
  }

  /* print dim_scores_ */
  ofs << "dim-score" << endl;
  for (index = 0; index != dim_scores_.size(); ++index) {
    ofs << "\t" << g_dim_name[index] << ": "
        << dim_scores_[index] << endl;
  }

  return;
}

/**************************************************************************
  call all the wave feature evaluate function and sum the score
**************************************************************************/
void Eval::EvalWave() {

  /* evaluate feature */
  EvalWaveFeatPositive(kWaveFeatWaveSec);
  EvalWaveFeatNegative(kWaveFeatSilentSec);
  EvalWaveFeatNegative(kWaveFeatLoudSec);
  EvalWaveFeatNegative(kWaveFeatSilentRate);
  EvalWaveFeatNegative(kWaveFeatLoudRate);

  /* sum score */
  int sum = 0;
  bool discard = false;
  vector<int>::size_type index = 0;

  for (index = 0; index < kWaveFeatMax; ++index) {
    if (g_wave_weight[index] > 0) {
      if (wave_scores_[index] < 0) {
        discard = true;
        break;
      } else {
        sum += g_wave_weight[index] * wave_scores_[index];
      }
    }
  }

  if (discard) {
    dim_scores_[kDimWave] = 0;
  } else {
    dim_scores_[kDimWave] = ((sum + 50) / 100);
  }

  return;
}

/**************************************************************************
  evaluate positive feature
**************************************************************************/
void Eval::EvalWaveFeatPositive(const unsigned int wave_feat) {
  const int feat = wave_->feats()[wave_feat];
  const int min = g_wave_feat_min[wave_feat];
  const int max = g_wave_feat_max[wave_feat];

  if ((feat < min) || (feat > max)) {
    wave_scores_[wave_feat] = -1;
  } else {
    wave_scores_[wave_feat] = (feat - min) * 100 / (max - min);
  }

  return;
}

/**************************************************************************
  evaluate negative feature
**************************************************************************/
void Eval::EvalWaveFeatNegative(const unsigned int wave_feat) {
  const int feat = wave_->feats()[wave_feat];
  const int min = g_wave_feat_min[wave_feat];
  const int max = g_wave_feat_max[wave_feat];

  if ((feat < min) || (feat > max)) {
    wave_scores_[wave_feat] = -1;
  } else {
    wave_scores_[wave_feat] = (max - feat) * 100 / (max - min);
  }

  return;
}

/**************************************************************************
  call all the desc feature evaluate function and sum the score
**************************************************************************/
void Eval::EvalDesc() {

  /* evaluate feature */
  EvalDescFeatSpeaker();
  EvalDescFeatOffice();
  EvalDescFeatPlatform();
  EvalDescFeatFormat();
  EvalDescFeatBusiness();

  /* sum score */
  int sum = 0;
  bool discard = false;
  vector<int>::size_type index = 0;

  for (index = 0; index < kDescFeatMax; ++index) {
    if (g_desc_weight[index] > 0) {
      if (desc_scores_[index] < 0) {
        discard = true;
        break;
      } else {
        sum += g_desc_weight[index] * desc_scores_[index];
      }
    }
  }

  if (discard) {
    dim_scores_[kDimDesc] = 0;
  } else {
    dim_scores_[kDimDesc] = ((sum + 50) / 100);
  }

  return;
}

/**************************************************************************
  Assume that one speaker's wave score is linear, then select in ratio of
  the speaker is reasonable.
**************************************************************************/
void Eval::EvalDescFeatSpeaker() {
  const string &speaker = desc_->feats()[kDescFeatSpeaker];
  const map<string, int> &rate = syns_->speaker_rate();

  map<string, int>::const_iterator iter = rate.find(speaker);
  if (iter != rate.end()) {
    desc_scores_[kDescFeatSpeaker] = iter->second;
  } else {
    desc_scores_[kDescFeatSpeaker] = 0;
  }

  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatOffice() {
  //const string &office = wave_->feats()[kDescFeatOffice];
  desc_scores_[kDescFeatOffice] = 0;
  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatPlatform() {
  //const string &platform = wave_->feats()[kDescFeatPlatform];
  desc_scores_[kDescFeatPlatform] = 0;
  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatFormat() {
  //const string &format = wave_->feats()[kDescFeatFormat];
  desc_scores_[kDescFeatFormat] = 0;
  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatBusiness() {
  //const string &business = wave_->feats()[kDescFeatBusiness];
  desc_scores_[kDescFeatBusiness] = 0;
  return;
}


