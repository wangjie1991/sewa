/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-03

  Description: This file provides util operation definitions

**************************************************************************/

#include "util.h"

#include <string>
#include <iostream>


extern const std::string g_suffix_wav(".wav");
extern const std::string g_suffix_des(".des");


/**************************************************************************
  Compare path last few charactors with suffix in the same length
**************************************************************************/
bool IsSuffixLegal(const std::string &path, const std::string &suffix) {
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
int ExtractName(const std::string &path, std::string &name) {
  /* check path */
  if (path.empty()) {
    std::cerr << "ExtractName error: path=\"\"." << std::endl;
    return -1;
  }

  /* start pos */
  std::string::size_type index = 0;

  std::string::size_type start_pos = 0;
  index = path.find_last_of('/');
  if (index != std::string::npos) {
    start_pos = index + 1;
  } 

  /* end pos */
  std::string::size_type end_pos = start_pos;
  index = path.find_first_of('.', start_pos);
  if (index != std::string::npos) {
    end_pos = index - 1;
  }

  name = path.substr(start_pos, end_pos-start_pos+1);

  /* check name */
  if (name.empty()) {
    std::cerr << "ExtractName error: name=\"\"" << std::endl;
    return -1;
  }

  return 0;
}


