//
// Created by nietup on 08.02.17.
//

#ifndef DEGREE_IMAGEHANDLER_H
#define DEGREE_IMAGEHANDLER_H

#include <string>
#include <vector>

class ImageHandler {
public:
    ImageHandler(std::vector<std::string> * FileNames, std::string Directory);

    /*purpose of this function:
     * open file
     * segment it
     * prepare proper learning sample
     * return it to the learning algorithm
     */
    void ImageToLearningSample();

private:
    std::vector<std::string> * Path;
    std::string Directory;
};


#endif //DEGREE_IMAGEHANDLER_H
