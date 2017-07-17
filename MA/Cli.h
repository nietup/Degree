//
// Created by nietup on 13.07.17.
//

#ifndef DEGREE_CLI_H
#define DEGREE_CLI_H
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include "Learning.h"

using namespace std;
using namespace elsd;

class Cli {
public:
    Cli();
    void Run();
    void SelectMode();
    void SelectModel();
    void SelectLearningSamples();
    void SelectTestingSamples();
    void ShowModel();
    void SaveModel();
    void Test();
    void Learn();
    void CreateModel();
    void LoadModel();
    void GetFilesInDirectory(std::vector<string> &out,
        const string &directory);

    enum Mode {TEST, LEARN};
    Mode mode;
    string pathToModel;
    string pathToPos;
    string pathToNeg;
    string pathToTest;
    Model model;
    vector<vector<shared_ptr<LineWrap>>> testingSamples;
    vector<vector<shared_ptr<LineWrap>>> posSamples;
    vector<vector<shared_ptr<LineWrap>>> negSamples;
};


#endif //DEGREE_CLI_H
