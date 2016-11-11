#include "Primitive.h"



Primitive::Primitive() {
	Considered->push_back(new Relationship("Rownoleglosc"));
	Considered->push_back(new Relationship("Prostopadlosc"));
	Considered->push_back(new Relationship("Przystawanie"));
}


Primitive::~Primitive() {
}
