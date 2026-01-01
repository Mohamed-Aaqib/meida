#include <vector>
#include "utils/FileUtils.h"
#include <string>
#include <filesystem>


std::vector<std::string> listPDFFiles(const std::string& folderPath) {

	std::vector<std::string> pdfs;
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".pdf") {
			pdfs.push_back(entry.path().string());
		}
	}

}
