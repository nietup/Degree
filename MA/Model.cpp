#include "Model.h"

void Model::ChooseRelationships(std::vector<int> * Chosen) {
	std::vector<Relationship*> tmp;

	for (int &i : *Chosen)
		tmp.push_back(Considered->operator[](i));

	delete Considered;
	Considered = new std::vector<Relationship*>(tmp);
}

std::wstring s2ws(const std::string& s) {
//	int len;
//	int slength = (int)s.length() + 1;
//	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
//	wchar_t* buf = new wchar_t[len];
//	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
//	std::wstring r(buf);
//	delete[] buf;
//	return r;
}

void Model::SaveModel() {
	//to file
	std::ofstream outfile(FilePath);

	if (!outfile.is_open()) {
		//create folder if nonexistent
        //TODO linux verion
//		std::wstring stemp = s2ws(FilePath);
//		LPCWSTR DirPath = stemp.c_str();
//		CreateDirectory(DirPath, NULL);
//
//		outfile.open(FilePath);

		if (!outfile.is_open()) {
			std::cout << "\nPrzykra sprawa";
		}
	}

	outfile << Name << "\n" << Level << "\n" << Considered->size() << "\n";
	for (auto &rel : *Considered)
		outfile << rel->GetName() << std::endl;

	outfile.close();
}
