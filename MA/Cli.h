//
// Created by nietup on 13.07.17.
//

#ifndef DEGREE_CLI_H
#define DEGREE_CLI_H

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

private:
    enum Mode {TEST, LEARN};
    Mode mode;
};


#endif //DEGREE_CLI_H
