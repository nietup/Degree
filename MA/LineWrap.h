//
// Created by nietup on 11.04.17.
//

#ifndef DEGREE_LINEWRAP_H
#define DEGREE_LINEWRAP_H

#include <elsdpgmfilereader.hpp>
#include <elsdetector.hpp>
#include <elsdsvgwriter.hpp>
#include "PointWrap.h"

/*! Wrapper for elsd::LineSegment
 * Provides useful functions for LineSegments:
 * - Length
 * - Distance
 * - Cos of angle
 */

class LineWrap {
public:
    LineWrap(elsd::LineSegment a);

    double Distance(const PointWrap & p);
    double Distance(LineWrap & l);
    double Length();
    double Dot(LineWrap & a);
    double GetCos(LineWrap & a);
    PointWrap GetStart() { return start; };
    PointWrap GetEnd() {return end; };

private:
    const PointWrap start, end;
};


#endif //DEGREE_LINEWRAP_H
