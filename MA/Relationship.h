#pragma once
#include <string>

class Relationship {
public:
	Relationship(std::string);
	~Relationship();

	std::string GetName() { return Name; };

    //SATISFACTION TEST WILL BE LAMBDA
    //or maybe we do derivation for every possible relationship

private:
	std::string Name;
};

