#pragma once
#include <string>
#include <functional>
#include <elsdpgmfilereader.hpp>
#include <elsdetector.hpp>
#include <elsdsvgwriter.hpp>
#include "LineWrap.h"

using namespace elsd;

class Relationship {
public:
	Relationship(std::string);
	~Relationship();

	std::string GetName() { return Name; };
    void SetScoringFunction(std::function<int (LineWrap *, LineWrap *)>);
    int Score(LineWrap * a, LineWrap * b) { return ScoringFunction(a, b); };

private:
	std::string Name;
    //scoring function will return score in scale 0-100 where 0 is best match
    std::function<int (LineWrap *, LineWrap *)> ScoringFunction;
};
