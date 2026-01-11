#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include "utils/FileUtils.h"
#include <poppler/cpp/poppler-document.h>


std::vector<std::string> listPDFFiles(const std::string& folderPath) {

	std::vector<std::string> pdfs;
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (entry.is_regular_file() && entry.path().extension() == ".pdf") {
				pdfs.push_back(entry.path().string());
			}
		}
	}
	catch (const std::exception& err)
	{
		std::cout << "couldn't find directory please try again: " << err.what() << std::endl;
	}
	return pdfs;
}