/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-03

  Description: This file provides util operation declearations

**************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <string>
using namespace std;


#define WAVE_HEAD_LEN 44
#define EVAL_SET_NUM 2

extern const string g_suffix_wav;
extern const string g_suffix_des;

/**************************************************************************
  Function: IsSuffixLegal
  Description: check the suffix of path is legal or not
  Input: const string &path
         const string &suffix
  Output: none
  Return: true, suffix is legal
          false, suffix is not legal
  Notice: none
**************************************************************************/
bool IsSuffixLegal(const string &path, const string &suffix);

/**************************************************************************
  Function: ExtractName
  Description: extract pure file name from full path name
  Input: const string &path
  Output: string &name
  Return: -1, failed
           0, success
  Notice: erase directory and suffix from path
**************************************************************************/
int ExtractName(const string &path, string &name);

#endif


