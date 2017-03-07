#pragma once
#include <string>
#include <functional>

class Relationship {
public:
	Relationship(std::string);
	~Relationship();

	std::string GetName() { return Name; };
    void SetScoringFunction(std::function<char ()>);
    char Score() { return ScoringFunction(); };

private:
	std::string Name;
    //scoring function will return score in scale 0-100 where 0 is best match
    std::function<char ()> ScoringFunction;
};

