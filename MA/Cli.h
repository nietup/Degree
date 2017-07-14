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
#include "Learning.h"

using namespace std;
using namespace elsd;

class Cli {
public:
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

private:
    enum Mode {TEST, LEARN};
    Mode mode;
    string pathToModel;
    string pathToPos;
    string pathToNeg;
    string pathToTest;
    Model model;
    vector<vector<weak_ptr<LineWrap>>> testingSamples;
    vector<vector<weak_ptr<LineWrap>>> posSamples;
    vector<vector<weak_ptr<LineWrap>>> negSamples;
};


#endif //DEGREE_CLI_H
