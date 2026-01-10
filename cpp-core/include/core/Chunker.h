#pragma once
#include <vector>
#include <string>

class Chunker {

public:
	static std::vector<std::string> chunkText(const std::string& full_text, int fineToken=500);
	static void saveAsJSON(const std::string& outPath, const std::vector<std::string>& chunks);

};