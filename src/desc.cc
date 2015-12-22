/**************************************************************************

  Copyright 2015-2016 Pachira

  Author : wangjie

  Date : 2015-11-04

  Description : This file provides the definitions of class Desc.

**************************************************************************/

#include "desc.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "util.h"

using namespace std;


/* feature type name */
extern const string g_desc_feat_name[kDescFeatMax] = {
  "speaker",
  "office",
  "platform",
  "format",
  "business"
};

/**************************************************************************
  desc initial
**************************************************************************/
Desc::Desc() : feats_(kDescFeatMax) { }

/**************************************************************************
  If file name is not end with .des, the path is considered invalid.
**************************************************************************/
Desc::Desc(const string &path)
    : path_(path), feats_(kDescFeatMax) {
  if (!IsSuffixLegal(path_, g_suffix_des)){
    path_ = "";
  }
}

/**************************************************************************
  Read file and extract features.
**************************************************************************/
int Desc::DescProc() {
  /* extract file name */
  if (0 != ExtractName(path_, name_)) {
    cerr << "desc error: " << path_ << " ExtractName failed" << endl;
    return -1;
  }

  /* get file stream */
  ifstream stream(path_.c_str());
  if (!stream) {
    cerr << "desc error: can't open file " << path_ << endl;
    return -1;
  }

  /* get features */
  int count = 0;
  string line;
  while (1) {
    getline(stream, line);
    if (stream.eof()) {
      break;
    }

    if (0 != ExtractDescFeat(line, count)) {
      break;
    }
    count++;
  }

  stream.close();

  if (count != kDescFeatMax) {
    cerr << "desc error: file " << path_ 
         << " json item process failed" << endl;
    return -1;
  } else {
    return 0;
  }
}

/**************************************************************************
  Check the format of file strickly.
  "speaker":"s"
  "office":"o"
  "platform":"p"
  "format":"f"
  "business":"b"
**************************************************************************/
int Desc::ExtractDescFeat(const string &line, const int count) {
  /* find pos of split charactor ':' */
  size_t pos = line.find_first_of(':');
  if (pos == string::npos) {
    cerr << "desc error: file " << path_ << " item " 
         << g_desc_feat_name[count] << " can't find ':'" << endl;
    return -1;
  }

  /* check '"' */
  if ((line[0] != '"') || (line[pos-1] != '"') || 
      (line[pos+1] != '"') || (line[line.length()-1] != '"')) {
    cerr << "desc error: file " << path_ << " item " 
         << g_desc_feat_name[count] << " '\"' is invalid" << endl;
    return -1;
  }

  /* get key */
  string key = line.substr(1, pos-2);
  if (0 != key.compare(g_desc_feat_name[count])) {
    cerr << "desc error: file " << path_ << " item " 
         << g_desc_feat_name[count] << " key is invalid" << endl;
    return -1;
  }

  /* get value */
  feats_[count] = line.substr(pos+2, line.length()-pos-3);
  return 0;
}

/**************************************************************************
  desc-feat
    speaker: "XX"
    office: "XX"
    platform: "XX"
    format: "XX"
    business: "XX"
    path: /home/xxx/file.des
**************************************************************************/
void Desc::Print(ofstream &ofs) const {
  if (!ofs) {
    return;
  }

  ofs << "desc-feat" << endl;

  vector<string>::size_type index;
  for (index = 0; index != feats_.size(); ++index) {
    ofs << "\t" << g_desc_feat_name[index] << ": \"" << feats_[index] 
        << "\"" << endl;
  }

  ofs << "\tpath: " << path_ << endl;

  return;
}


