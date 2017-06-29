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

    virtual void AssignParts(std::vector<std::vector<Relationship*>*> *);

    std::vector<std::vector<Relationship*>*> GetParts() { return *Parts; }

protected:
	std::vector<Relationship*> * Considered;
    /*this looks somewhat complicated
      by 'Part' I mean pair of detected segments that satisfies one or more of considered constraints
      that is why this is a vector of vectors - maybe there is only one constraint, maybe more
      (e.g. maybe consecutive edges have to perpendicular - like in square)
      this vector will be filled during the learning process
     */
    std::vector<std::vector<Relationship*>*> * Parts;
	std::string Name;
	int Level;
	std::string FilePath;
};

