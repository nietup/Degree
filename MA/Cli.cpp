//
// Created by nietup on 13.07.17.
//

#include <iostream>
#include "Cli.h"

using namespace std;

void Cli::Run() {
    SelectMode();
    SelectModel();
    ShowModel();

    if (mode == TEST) {
        SelectTestingSamples();
        Test();
    }
    else if (mode == LEARN) {
        SelectLearningSamples();
        Learn();
        SaveModel();
    }
}