//
// Created by nietup on 08.02.17.
//

#ifndef DEGREE_IMAGEHANDLER_H
#define DEGREE_IMAGEHANDLER_H

#include <string>

class ImageHandler {
public:
    ImageHandler(std::string PathToFolder);

    /*purpose of this function:
     * open file
     * segment it
     * prepare proper learning sample
     * return it to the learning algorithm
     */
    void HandleFile(std::string PathToFile);

private:
    std::string Path;
};


#endif //DEGREE_IMAGEHANDLER_H