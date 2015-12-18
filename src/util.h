/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-03

  Description: This file provides util operation declearations

**************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <string>

#define WAVE_HEAD_LEN 44
#define EVAL_SET_NUM 2

extern const std::string g_suffix_wav;
extern const std::string g_suffix_des;

/**************************************************************************
  Function: IsSuffixLegal
  Description: check the suffix of path is legal or not
  Input: const std::string &path
         const std::string &suffix
  Output: none
  Return: true, suffix is legal
          false, suffix is not legal
  Notice: none
**************************************************************************/
bool IsSuffixLegal(const std::string &path, const std::string &suffix);

/**************************************************************************
  Function: ExtractName
  Description: extract pure file name from full path name
  Input: const std::string &path
  Output: std::string &name
  Return: -1, failed
           0, success
  Notice: erase directory and suffix from path
**************************************************************************/
int ExtractName(const std::string &path, std::string &name);

#endif


