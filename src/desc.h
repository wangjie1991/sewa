/**************************************************************************

  Copyright 2015-2016 Pachira

  Author: wangjie

  Date: 2015-11-04

  Description: This file provides a description and declearation of
               features of a single .des file.

**************************************************************************/

#ifndef DESC_H
#define DESC_H

#include <string>
#include <vector>


/* feature type */
enum DescFeat {
  kDescFeatSpeaker,   //who provides the service
  kDescFeatOffice,    //speaker's group or working site
  kDescFeatPlatform,  //platform used
  kDescFeatFormat,    //format of recording file
  kDescFeatBusiness,  //the problem speakers deal with
  kDescFeatMax
};

extern const std::string g_desc_feat_name[];

/**************************************************************************
  This class provides the function to get desc feature from desc file.
  The value of feature can be accessed by eval object to evaluate the
  priority.
**************************************************************************/
class Desc {
 public:
  
/**************************************************************************
  Function: Desc
  Description: default constructor
  Input: none
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  Desc();

/**************************************************************************
  Function: Desc
  Description: constructor with path_ initialization
  Input: const std::string &path
  Output: none
  Return: none
  Notice: constructor initializer for variables
**************************************************************************/
  explicit Desc(const std::string &path);

/**************************************************************************
  Function: ~Desc
  Description: destructor
  Input: none
  Output: none
  Return: none
  Notice: none
**************************************************************************/
  ~Desc() { }

/**************************************************************************
  Function: DescProc
  Description: main process function
  Input: none
  Output: none
  Return: -1, failed.
           0, success.
  Notice: none
**************************************************************************/
  int DescProc();

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
  const std::string &name() const { return name_; }
  const std::vector<std::string> &feats() const { return feats_; }

 private:

/**************************************************************************
  Function: ExtractDescFeat
  Description: Extract features from desc data
  Input:  const std::string &line   current json item
          const int count           current index of json item
  Output: none
  Return: -1, failed
           0, success
  Notice: none 
**************************************************************************/
  int ExtractDescFeat(const std::string &line, const int count);

  /* file full path name */
  std::string path_;

  /* file name without directory and suffix */
  std::string name_;

  /* desc features */
  std::vector<std::string> feats_;

};

#endif


