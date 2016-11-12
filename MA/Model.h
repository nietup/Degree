#pragma once
#include <vector>
#include "Relationship.h"

class Model {
public:
	virtual std::string GetRelationshipName(int i) { return ((*Considered)[i])->GetName(); };
	virtual int NumberOfRelationships() { return Considered->size(); };

	virtual void ChooseRelationships(std::vector<int>*);

protected:
	std::vector<Relationship*> * Considered;
	std::string Name;
};

