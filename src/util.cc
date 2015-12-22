/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-03

  Description: This file provides util operation definitions

**************************************************************************/

#include "util.h"

#include <string>
#include <iostream>

using namespace std;


extern const string g_suffix_wav(".wav");
extern const string g_suffix_des(".des");


/**************************************************************************
  Compare path last few charactors with suffix in the same length
**************************************************************************/
bool IsSuffixLegal(const string &path, const string &suffix) {
  /* check path and suffix */
  if (path.empty() || suffix.empty()) {
    return false;
  }

  if (path.length() <= suffix.length()) {
    return false;
  }

  int cmp = path.compare(path.length() - suffix.length(), 
                         suffix.length(), 
                         suffix);

  if (cmp == 0) {
    return true;
  } else {
    return false;
  }
}

/**************************************************************************
  Get substr with STL string.
**************************************************************************/
int ExtractName(const string &path, string &name) {
  /* check path */
  if (path.empty()) {
    cerr << "util error: path=\"\"." << endl;
    return -1;
  }

  /* start pos */
  size_t index = 0;

  size_t start_pos = 0;
  index = path.find_last_of('/');
  if (index != string::npos) {
    start_pos = index + 1;
  } 

  /* end pos */
  size_t end_pos = start_pos;
  index = path.find_last_of('.');
  if (index != string::npos) {
    end_pos = index - 1;
  }

  name = path.substr(start_pos, end_pos-start_pos+1);

  /* check name */
  if (name.empty()) {
    cerr << "util error: name=\"\"" << endl;
    return -1;
  }

  return 0;
}


