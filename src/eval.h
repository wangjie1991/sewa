/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-04

  Description: This file provides a description and declearation of class
               Eval.

**************************************************************************/

#ifndef EVAL_H
#define EVAL_H

#include <string>
#include <vector>

class Wave;
class Desc;
class Syns;


enum Dimension {
  kDimWave,
  kDimDesc,
  kDimMax
};

extern const std::string g_dim_name[];

/**************************************************************************
  This class provides data and method to evaluate priority
**************************************************************************/
class Eval {
 public:
  
/**************************************************************************
  Function: Eval
  Description: default constructor
  Input: none
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  Eval();

/**************************************************************************
  Function: Eval
  Description: constructor with wave_ initialization
  Input: const Wave *const wave
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  explicit Eval(const Wave *const wave);

/**************************************************************************
  Function: Eval
  Description: constructor with wave_, desc_, syns_ initialization
  Input: const Wave *const wave
         const Desc *const desc
         const Syns *const syns
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  explicit Eval(const Wave *const wave, const Desc *const desc, 
                const Syns *const syns);

/**************************************************************************
  Function: ~Eval
  Description: destructor
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  ~Eval() { }

/**************************************************************************
  Function: EvalProc
  Description: eval main process
  Input: none
  Output: none
  Return: -1, failed.
           0, success.
  Notice: None
**************************************************************************/
  int EvalProc();

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
  const Wave *wave() const { return wave_; }
  int priority() const { return priority_; }

 private:

/**************************************************************************
  Function: EvalWave
  Description: evaluate the score of wave
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalWave();

/**************************************************************************
  Function: EvalWaveFeatPositive
  Description: evaluate the score of positive feature
  Input: const unsigned int wave_feat
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalWaveFeatPositive(const unsigned int wave_feat);

/**************************************************************************
  Function: EvalWaveFeatNegative
  Description: evaluate the score of negative feature
  Input: const unsigned int wave_feat
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalWaveFeatNegative(const unsigned int wave_feat);

/**************************************************************************
  Function: EvalDesc
  Description: evaluate the score of desc
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalDesc();

/**************************************************************************
  Function: EvalDescFeatSpeaker
  Description: evaluate the score of speaker
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalDescFeatSpeaker();

/**************************************************************************
  Function: EvalDescFeatOffice
  Description: evaluate the score of office
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalDescFeatOffice();

/**************************************************************************
  Function: EvalDescFeatPlatform
  Description: evaluate the score of platform
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalDescFeatPlatform();

/**************************************************************************
  Function: EvalDescFeatFormat
  Description: evaluate the score of format
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalDescFeatFormat();

/**************************************************************************
  Function: EvalDescFeatBusiness
  Description: evaluate the score of business
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  void EvalDescFeatBusiness();

  /* object pointer */
  const Wave *wave_;
  const Desc *desc_;
  const Syns *syns_;

  /* scores transformed from features */
  std::vector<int> wave_scores_;
  std::vector<int> desc_scores_;

  /* score of different dimention */
  std::vector<int> dim_scores_;
  int priority_;

};

#endif


