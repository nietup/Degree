#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include "Relationship.h"

class Model {
public:
	virtual std::string GetRelationshipName(int i) { return ((*Considered)[i])->GetName(); };
	virtual int NumberOfRelationships() { return Considered->size(); };

	virtual void ChooseRelationships(std::vector<int>*);
	virtual void SetName(std::string s) { Name = s; };
	virtual void SetFilePath(std::string s) { FilePath = s; };

	virtual void SaveModel();

protected:
	std::vector<Relationship*> * Considered;
	std::string Name;
	int Level;
	std::string FilePath;
};

