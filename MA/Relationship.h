#pragma once
#include <string>

class Relationship {
public:
	Relationship(std::string);
	~Relationship();

	std::string GetName() { return Name; };

private:
	std::string Name;
};

