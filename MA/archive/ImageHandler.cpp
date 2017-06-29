//
// Created by nietup on 08.02.17.
//

#include "ImageHandler.h"
#include <iostream>
#include <fstream>

#include <elsdpgmfilereader.hpp>
#include <elsdetector.hpp>
#include <elsdsvgwriter.hpp>

ImageHandler::ImageHandler(std::vector<std::string> * aPath, std::string Dir) : Path(aPath), Directory(Dir) {
}

void ImageHandler::ImageToLearningSample() {
    //this is proof of concept only now
    //TODO
    std::string inFile = Directory + (*Path)[0];
    Path->erase(Path->begin(), Path->begin()++);
    std::cout << "\nHANDLED FILE: " << inFile << "\n";

    using namespace std;
    using namespace elsd;

    //string outFile = inFile + ".svg";

    ImageInterface::Ptr image(new ElsdPgmFileReader(inFile));
    ShapesDetectorInterface::Ptr detector(new ElsDetector);
    //SvgWriterInterface::Ptr svg(new ElsdSvgWriter);

    detector->run(image);
    /*svg->setImageSize(image->xsize(), image->ysize());

    svg->addLineSegments(detector->getLineSegments().begin(), detector->getLineSegments().end());
    svg->addArcs(detector->getCircularArcs().begin(), detector->getCircularArcs().end());
    svg->addArcs(detector->getEllipticalArcs().begin(), detector->getEllipticalArcs().end());

    ofstream ofs(outFile, ofstream::out);
    ofs << *svg;
    ofs.close();*/

    cout << inFile << endl;
    cout << detector->getEllipticalArcs().size() << " elliptical arcs, "
         << detector->getCircularArcs().size() << " circular arcs, "
         << detector->getLineSegments().size() << " line segments"
         << endl;
}
