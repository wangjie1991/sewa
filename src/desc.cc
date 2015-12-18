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


/* feature type name */
extern const std::string g_desc_feat_name[kDescFeatMax] = {
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
Desc::Desc(const std::string &path)
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
    std::cerr << "DescProc error: " << path_ 
              << " ExtractName failed" << std::endl;
    return -1;
  }

  /* get file stream */
  std::ifstream stream(path_.c_str());
  if (!stream) {
    std::cerr << "DescProc error: can't open file " 
              << path_ << std::endl;
    return -1;
  }

  /* get features */
  int count = 0;
  std::string line;
  while (1) {
    std::getline(stream, line);
    if (stream.eof()) {
      break;
    }

    //std::cout << "DescProc : file " << path_ 
              //<< " line=" << line << " count=" << count << std::endl;
    if (0 != ExtractDescFeat(line, count)) {
      break;
    }
    count++;
  }

  stream.close();

  if (count != kDescFeatMax) {
    std::cerr << "DescProc error: file " << path_
              << " json item process failed" << std::endl;
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
int Desc::ExtractDescFeat(const std::string &line, const int count) {
  /* find pos of split charactor ':' */
  std::string::size_type pos = line.find_first_of(':');
  if (pos == std::string::npos) {
    std::cerr << "ExtractDescFeat error: file " << path_ 
              << " item " << g_desc_feat_name[count] 
              << " can't find ':'" << std::endl;
    return -1;
  }

  /* check '"' */
  if ((line[0] != '"') || (line[pos-1] != '"') || 
      (line[pos+1] != '"') || (line[line.length()-1] != '"')) {
    std::cerr << "ExtractDescFeat error: file " << path_ 
              << " item " << g_desc_feat_name[count] 
              << " '\"' is invalid" << std::endl;
    return -1;
  }

  /* get key */
  std::string key = line.substr(1, pos-2);
  if (0 != key.compare(g_desc_feat_name[count])) {
    std::cerr << "ExtractDescFeat error: file " << path_ 
              << " item " << g_desc_feat_name[count] 
              << " key is invalid" << std::endl;
    return -1;
  }

  /* get value */
  feats_[count] = line.substr(pos+2, line.length()-pos-3);
  return 0;
}

/**************************************************************************
  --------
  path : /home/xxx/file.wav
  name : file
  feat-speaker : A
  feat-office : 1111
  ....
**************************************************************************/
void Desc::Print() const {
  std::cout << "--------" << std::endl
            << "path : " << path_ << std::endl
            << "name : " << name_ << std::endl;

  std::vector<std::string>::size_type index;
  for (index = 0; index != feats_.size(); ++index) {
    std::cout << "feat-" << g_desc_feat_name[index] << " : "
              << feats_[index] << std::endl;
  }

  return;
}


