/**************************************************************************

  Copyright 2015-2016 Pachira

  Author : wangjie

  Date : 2015-11-11

  Description : Read the config parameters and run the main function.

**************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "syns.h"


extern short g_silent_short_limit;
extern float g_silent_float_rate; 
extern short g_loud_short_thres;
extern short g_loud_short_num;

extern int g_wave_feat_min[];
extern int g_wave_feat_max[];
extern int g_wave_weight[];
extern int g_desc_weight[];
extern int g_dim_weight[];

std::string wave_list;
std::string desc_list;
std::string select_list;
int select_hour;


/**************************************************************************
  Function: ExtractArray
  Description: get array parameters
  Input: const std::string &val
  Output: int *array
  Return: none
  Notice: none
**************************************************************************/
void ExtractArray(const std::string &val, int *array) {
  size_t pos_beg = 0;
  size_t pos_end = 0;
  int i = 0;

  do {
    pos_end = val.find_first_of(',', pos_beg);
    if (pos_end == std::string::npos) {
      pos_end = val.find_first_of(']', pos_beg);
    }

    if (pos_end != std::string::npos) {
      std::string sub = val.substr(pos_beg+1, pos_end-pos_beg-1);
      array[i] = atoi(sub.c_str());
      pos_beg = pos_end + 1;
      i++;
    }
  } while (pos_end != std::string::npos);

  return;
}

/**************************************************************************
  Function: ExtractConfig
  Description: get key parameters
  Input: const std::string &line
  Output: none
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
int ExtractConfig(const std::string &line) {
  /* empty line */
  if (0 == line.compare("")) {
    return 0;
  }

  /* comment line */
  if ('#' == line[0]) {
    return 0;
  }

  /* find pos of charactor '=' */
  std::string::size_type pos = line.find_first_of('=');
  if (pos == std::string::npos) {
    std::cerr << "ExtractConfig error: can't find '=' in '" << line << "'" 
              << std::endl;
    return -1;
  }

  /* get key and val */
  std::string key = line.substr(0, pos-1);
  std::string val = line.substr(pos+2, line.length()-pos-2);

  /* main */
  if (0 == key.compare("wave_list")) {
    wave_list = val;
  }
  else if (0 == key.compare("desc_list")) {
    desc_list = val;
  }
  else if (0 == key.compare("select_list")) {
    select_list = val;
  }
  else if (0 == key.compare("select_hour")) {
    select_hour = atoi(val.c_str());
  }
  /* wave */
  else if (0 == key.compare("silent_short_limit")) {
    g_silent_short_limit = (short)atoi(val.c_str());
  }
  else if (0 == key.compare("silent_float_rate")) {
    g_silent_float_rate = (float)atof(val.c_str());
  }
  else if (0 == key.compare("loud_short_thres")) {
    g_loud_short_thres = (short)atoi(val.c_str());
  }
  else if (0 == key.compare("loud_short_num")) {
    g_loud_short_num = (short)atoi(val.c_str());
  }
  /* eval */
  else if (0 == key.compare("wave_feat_min")) {
    ExtractArray(val, g_wave_feat_min);
  }
  else if (0 == key.compare("wave_feat_max")) {
    ExtractArray(val, g_wave_feat_max);
  }
  else if (0 == key.compare("wave_weight")) {
    ExtractArray(val, g_wave_weight);
  }
  else if (0 == key.compare("desc_weight")) {
    ExtractArray(val, g_desc_weight);
  }
  else if (0 == key.compare("dim_weight")) {
    ExtractArray(val, g_dim_weight);
  }
  else {
  }

  return 0;
}

/**************************************************************************
  Function: ReadConfig
  Description: read config file 
  Input: const std::string &config
  Output: none
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
int ReadConfig(const std::string &config) {
  bool fail = false;

  /* get file stream */
  std::ifstream stream(config.c_str());
  if (!stream) {
    std::cerr << "ReadConfig error: can't open file " 
              << config << std::endl;
    return -1;
  }

  /* get config */
  std::string line;
  while (!stream.eof()) {
    std::getline(stream, line);
    if (0 != ExtractConfig(line)) {
      fail = true;
      break;
    }
  }
  stream.close();

  if (fail) {
    std::cerr << "ReadConfig error: file " << config
              << " process failed" << std::endl;
    return -1;
  } else {
    return 0;
  }
}

/**************************************************************************
  main function
**************************************************************************/
int main(int argc, char* argv[]) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " config." << std::endl;
    return -1;
  }

  if (0 != ReadConfig(argv[1])) {
    std::cerr << argv[0] << " read config failed." << std::endl;
    return -1;
  }

  Syns syns(wave_list, desc_list);

  if (0 != syns.SynsProc()) {
    std::cerr << argv[0] << " process failed." << std::endl;
    return -1;
  }

  if (0 != syns.Select(select_hour, select_list)) {
    std::cerr << argv[0] << " select failed." << std::endl;
    return -1;
  }

  std::cout << argv[0] << " process success." << std::endl;
  return 0;
}


