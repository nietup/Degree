#include "Model.h"

void Model::ChooseRelationships(std::vector<int> * Chosen) {
	std::vector<Relationship*> tmp;

	for (int &i : *Chosen) 
		tmp.push_back(Considered->operator[](i));
	
	delete Considered;
	Considered = new std::vector<Relationship*>(tmp);
}
