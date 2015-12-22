/**************************************************************************

  Copyright 2015-2016 Pachira

  Author : wangjie

  Date : 2015-11-11

  Description : Get the config parameters and run the main function.

**************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "wave.h"
#include "desc.h"
#include "syns.h"

using namespace std;


extern short g_silent_limit;
extern float g_silent_rate; 
extern short g_loud_thres;
extern short g_loud_num;

extern int g_wave_feat_min[];
extern int g_wave_feat_max[];
extern int g_wave_weight[];
extern int g_desc_weight[];
extern int g_dim_weight[];


/**************************************************************************
  Function: ExtractConf
  Description: get conf parameters
  Input: const string &line
  Output: none
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
int ExtractConf(const string &line) {
  /* empty or comment line */
  if ((0 == line.compare("")) || ('#' == line[0])) {
    return 0;
  }

  /* find pos of charactor '=' */
  size_t pos = line.find_first_of('=');
  if (pos == string::npos) {
    cerr << "main error: can't find '=' in conf file line " << line << endl;
    return -1;
  }

  /* get key and val */
  string key = line.substr(0, pos-1);
  string val = line.substr(pos+2, line.length()-pos-2);
  const char *c_val = val.c_str();

  if (0 == key.compare("silent_limit")) {
    g_silent_limit = (short)atoi(c_val);
  }
  else if (0 == key.compare("silent_rate")) {
    g_silent_rate = (float)atof(c_val);
  }
  else if (0 == key.compare("loud_thres")) {
    g_loud_thres = (short)atoi(c_val);
  }
  else if (0 == key.compare("loud_num")) {
    g_loud_num = (short)atoi(c_val);
  }

  else if (0 == key.compare("wave_minute_min")) {
    g_wave_feat_min[kWaveFeatWaveSec] = (int)(atof(c_val)*60);
  }
  else if (0 == key.compare("wave_minute_max")) {
    g_wave_feat_max[kWaveFeatWaveSec] = (int)(atof(c_val)*60);
  }
  else if (0 == key.compare("silent_minute_min")) {
    g_wave_feat_min[kWaveFeatSilentSec] = (int)(atof(c_val)*60);
  }
  else if (0 == key.compare("silent_minute_max")) {
    g_wave_feat_max[kWaveFeatSilentSec] = (int)(atof(c_val)*60);
  }
  else if (0 == key.compare("loud_minute_min")) {
    g_wave_feat_min[kWaveFeatLoudSec] = (int)(atof(c_val)*60);
  }
  else if (0 == key.compare("loud_minute_max")) {
    g_wave_feat_max[kWaveFeatLoudSec] = (int)(atof(c_val)*60);
  }
  else if (0 == key.compare("silent_rate_min")) {
    g_wave_feat_min[kWaveFeatSilentRate] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("silent_rate_max")) {
    g_wave_feat_max[kWaveFeatSilentRate] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("loud_rate_min")) {
    g_wave_feat_min[kWaveFeatLoudRate] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("loud_rate_max")) {
    g_wave_feat_max[kWaveFeatLoudRate] = (int)(atof(c_val)*100);
  }

  else if (0 == key.compare("weight_wave_minute")) {
    g_wave_weight[kWaveFeatWaveSec] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_silent_minute")) {
    g_wave_weight[kWaveFeatSilentSec] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_loud_minute")) {
    g_wave_weight[kWaveFeatLoudSec] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_silent_rate")) {
    g_wave_weight[kWaveFeatSilentRate] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_loud_rate")) {
    g_wave_weight[kWaveFeatLoudRate] = (int)(atof(c_val)*100);
  }

  else if (0 == key.compare("weight_speaker")) {
    g_desc_weight[kDescFeatSpeaker] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_office")) {
    g_desc_weight[kDescFeatOffice] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_platform")) {
    g_desc_weight[kDescFeatPlatform] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_format")) {
    g_desc_weight[kDescFeatFormat] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_business")) {
    g_desc_weight[kDescFeatBusiness] = (int)(atof(c_val)*100);
  }

  else if (0 == key.compare("weight_wave")) {
    g_dim_weight[kDimWave] = (int)(atof(c_val)*100);
  }
  else if (0 == key.compare("weight_desc")) {
    g_dim_weight[kDimDesc] = (int)(atof(c_val)*100);
  }

  else {
    cerr << "main error: No such conf item in line " << line << endl;
    return -1;
  }

  return 0;
}

/**************************************************************************
  Function: GetConf
  Description: read config file 
  Input: const string &config
  Output: none
  Return: -1, failed
           0, success
  Notice: none
**************************************************************************/
int GetConf(const string &conf) {
  bool fail = false;

  /* get file stream */
  ifstream stream(conf.c_str());
  if (!stream) {
    cerr << "main error: can't open file " << conf << endl;
    return -1;
  }

  /* get conf */
  string line;
  while (!stream.eof()) {
    getline(stream, line);
    if (0 != ExtractConf(line)) {
      fail = true;
      break;
    }
  }
  stream.close();

  if (fail) {
    cerr << "main error: fail to extract conf file " << conf << endl;
    return -1;
  } else {
    return 0;
  }
}

/**************************************************************************
  print the usage message
**************************************************************************/
void Usage() {
  cerr << "\nUsage: ./sewa [options] wave_list sewa_hour sewa_list" << endl;
  cerr << "Select sewa_hour WAV files from wave_list to sewa_list." << endl;
  cerr << "Example: ./sewa yintianxia.list 100 output.list.\n" << endl;
  cerr << "Options:" << endl;
  cerr << " -c conf       file of parameters to control selection" << endl;
  cerr << " -d desc_list  file of list of description files" << endl;
  cerr << " -l sewa_log   file of detail information in selection\n" << endl;
  return;
}

/**************************************************************************
  main function
**************************************************************************/
int main(int argc, char* argv[]) {

  int opt = 0;
  string conf;
  string desc_list;
  string sewa_log;

  while ((opt = getopt(argc, argv, "c:d:l:")) != -1) {
    switch(opt) {
      case 'c':
        conf = optarg;
        break;
      case 'd':
        desc_list = optarg;
        break;
      case 'l':
        sewa_log = optarg;
        break;
      case '?':
        Usage();
        return -1;
    }
  }

  if ((argc - optind) != 3) {
    Usage();
    return -1;
  }

  string wave_list(argv[optind]);
  int sewa_hour = atoi(argv[optind + 1]);
  string sewa_list(argv[optind + 2]);

  if (!conf.empty() && (0 != GetConf(conf))) {
    cerr << "main error: get conf params failed." << endl;
    return -1;
  }

  Syns syns(wave_list, desc_list, sewa_hour, sewa_list, sewa_log);
  if (0 != syns.SynsProc()) {
    cerr << "main error: sewa process failed." << endl;
    return -1;
  }

  cout << "main msg: sewa process success." << endl;
  return 0;
}


