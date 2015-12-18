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
#include <map>

#include "wave.h"
#include "desc.h"
#include "syns.h"


/* dimention name */
extern const std::string g_dim_name[kDimMax] = {
  "wave",
  "desc"
};

/* wide rate, narrow value */
/* 60-300 -> 100-200 -> 10%-60% -> 10-120 */
int g_wave_feat_min[kWaveFeatMax]
                   = {60, 10, 50, 0, 0, 10, 0, 0};
int g_wave_feat_max[kWaveFeatMax]
                   = {300, 120, 300, 20, 30, 60, 10, 20};

int g_wave_weight[kWaveFeatMax] = {15, 15, 0, 20, 0, 30, 20, 0};
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
    std::cerr << "EvalProc error: wave_ is NULL" << std::endl;
  }
  EvalWave();

  if ((desc_ != NULL) && (syns_ != NULL)) {
    EvalDesc();
  }

  priority_ = 0;
  std::vector<int>::size_type index = 0;
  for (index = 0; index < kDimMax; ++index) {
    priority_ += g_dim_weight[index] * dim_scores_[index];
  }

  return 0;
}

/**************************************************************************
  ---------------------
  name : file
  score-wave-wavsec : A
  score-wave-noisec : B
  ......
  score-desc-speaker : A
  score-desc-office : B
  ......
  score-dim-wave : A
  score-dim-desc : B
  ......
  priority : A
**************************************************************************/
void Eval::Print() const {
  std::cout << "---------------------" << std::endl
            << "name : " << wave_->name() << std::endl;

  std::vector<int>::size_type index = 0;

  /* print wave_scores_ */
  for (index = 0; index != wave_scores_.size(); ++index) {
    std::cout << "score-wave-" << g_wave_feat_name[index] << " : "
              << wave_scores_[index] << std::endl;
  }

  /* print desc_scores_ */
  for (index = 0; index != desc_scores_.size(); ++index) {
    std::cout << "score-desc-" << g_desc_feat_name[index] << " : "
              << desc_scores_[index] << std::endl;
  }

  /* print dim_scores_ */
  for (index = 0; index != dim_scores_.size(); ++index) {
    std::cout << "score-dim-" << g_dim_name[index] << " : "
              << dim_scores_[index] << std::endl;
  }

  std::cout << "priority : " << priority_ << std::endl;

  return;
}

/**************************************************************************
  call all the wave feature evaluate function and sum the score
**************************************************************************/
void Eval::EvalWave() {

  /* evaluate feature */
  EvalWaveFeatPositive(kWaveFeatWaveSec);
  EvalWaveFeatNegative(kWaveFeatSilentSec);
  EvalWaveFeatPositive(kWaveFeatVocalSec);
  EvalWaveFeatNegative(kWaveFeatLoudSec);
  EvalWaveFeatNegative(kWaveFeatNoiseSec);
  EvalWaveFeatNegative(kWaveFeatSilentRate);
  EvalWaveFeatNegative(kWaveFeatLoudRate);
  EvalWaveFeatNegative(kWaveFeatNoiseRate);

  /* sum score */
  int sum = 0;
  bool discard = false;
  std::vector<int>::size_type index = 0;

  for (index = 0; index < kWaveFeatMax; ++index) {
    if (wave_scores_[index] < 0) {
      discard = true;
      break;
    }
    sum += g_wave_weight[index] * wave_scores_[index];
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
  std::vector<int>::size_type index = 0;

  for (index = 0; index < kDescFeatMax; ++index) {
    if (desc_scores_[index] < 0) {
      discard = true;
      break;
    }
    sum += g_desc_weight[index] * desc_scores_[index];
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
  const std::string &speaker = desc_->feats()[kDescFeatSpeaker];
  const std::map<std::string, int> &rate = syns_->speaker_rate();

  std::map<std::string, int>::const_iterator iter = rate.find(speaker);
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
  //const std::string &office = wave_->feats()[kDescFeatOffice];
  desc_scores_[kDescFeatOffice] = 0;
  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatPlatform() {
  //const std::string &platform = wave_->feats()[kDescFeatPlatform];
  desc_scores_[kDescFeatPlatform] = 0;
  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatFormat() {
  //const std::string &format = wave_->feats()[kDescFeatFormat];
  desc_scores_[kDescFeatFormat] = 0;
  return;
}

/**************************************************************************
  No detail transform method
**************************************************************************/
void Eval::EvalDescFeatBusiness() {
  //const std::string &business = wave_->feats()[kDescFeatBusiness];
  desc_scores_[kDescFeatBusiness] = 0;
  return;
}


