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
    LineWrap(std::pair<double, double>, std::pair<double, double>);

    double Distance(const PointWrap & p) const;
    double Distance(const LineWrap & l) const ;
    double Length() const;
    double Dot(const LineWrap & a) const;
    double GetCos(const LineWrap & a) const;
    PointWrap GetStart() const { return start; };
    PointWrap GetEnd() const {return end; };

    bool matched;
    const PointWrap start, end;
};


#endif //DEGREE_LINEWRAP_H
