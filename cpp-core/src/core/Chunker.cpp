#include "core/Chunker.h"
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

static std::vector<std::string> chunkText(const std::string& full_text, int fineToken) {

	std::vector<std::string> chunks;
	std::istringstream iss(full_text);
	std::string word;
	std::stringstream buffer;
	int token_count = 0;

	// input from string
	while (iss >> word) {
		// append to string
		buffer << word << " ";
		token_count++;
		if (token_count >= fineToken) {
			chunks.push_back(buffer.str());
			buffer.str("");
			buffer.clear();
			token_count = 0;
		}
	}

	if (!buffer.str().empty()) chunks.push_back(buffer.str()); 
	return chunks;

}

using json = nlohmann::json;

static void saveAsJSON(const std::string& outPath, const std::vector<std::string>& chunks) {

	// using a try catch would be efficient here
	json j;
	j["chunks"] = chunks;
	std::ofstream file(outPath);
	file << j.dump(4);
	file.close();



}