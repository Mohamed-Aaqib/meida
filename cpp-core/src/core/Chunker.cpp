#include "core/Chunker.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <thread>
#include <mutex>

std::vector<std::string> Chunker::chunkText(const std::string& full_text, int fineToken) {

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

// function to sanitize UTF-8 strings by replacing invalid bytes with '?'
std::string sanitizeUTF8(const std::string& str) {
	std::string result;
	result.reserve(str.size());
	
	for (size_t i = 0; i < str.size(); ++i) {
		unsigned char c = static_cast<unsigned char>(str[i]);
		
		// ASCII is always valid
		if (c < 0x80) {
			result += c;
		}
		// Check for valid UTF-8 multi-byte sequences, 2 , 3 and 4 bytes sequence (unicode);
		else if ((c & 0xE0) == 0xC0 && i + 1 < str.size() && (str[i + 1] & 0xC0) == 0x80) {
			result += c;
			result += str[++i];
		}
		else if ((c & 0xF0) == 0xE0 && i + 2 < str.size() && 
		         (str[i + 1] & 0xC0) == 0x80 && (str[i + 2] & 0xC0) == 0x80) {
			result += c;
			result += str[++i];
			result += str[++i];
		}
		else if ((c & 0xF8) == 0xF0 && i + 3 < str.size() &&
		         (str[i + 1] & 0xC0) == 0x80 && (str[i + 2] & 0xC0) == 0x80 && 
		         (str[i + 3] & 0xC0) == 0x80) {
			result += c;
			result += str[++i];
			result += str[++i];
			result += str[++i];
		}
		else {
			// Invalid byte - replace with '?' (safe ASCII character)
			result += '?';
		}
	}
	
	return result;
}

void Chunker::saveAsJSON(const std::string& outPath, const std::vector<std::string>& chunks) {

	try {
		
		// Sanitize chunks to remove invalid UTF-8 bytes
		std::vector<std::string> sanitizedChunks;
		sanitizedChunks.reserve(chunks.size());
		for (const auto& chunk : chunks) {
			sanitizedChunks.push_back(sanitizeUTF8(chunk));
		}
		
		json j;
		j["chunks"] = sanitizedChunks;
		
		//// Create parent directory if it doesn't exist
		//std::filesystem::path path(outPath);
		//std::filesystem::path parentDir = path.parent_path();
		//if (!parentDir.empty() && !std::filesystem::exists(parentDir)) {
		//	std::filesystem::create_directories(parentDir);
		//}
		
		// Open file with explicit flags

		std::ofstream file(outPath);

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << outPath << std::endl;
			return;
		}

		// Dump JSON once and use it
		std::string dumped = j.dump(4);

		// Write the JSON string, force it to be on the disk using flush
		file.write(dumped.c_str(), dumped.size());
		file.flush();
		
		// Check for errors
		if (file.fail() || file.bad()) {
			std::cerr << "WRITE FAILED for: " << outPath << std::endl;
			file.close();
			return;
		}
		
		file.close();
		
		//// Check file size after closing
		//if (std::filesystem::exists(outPath)) {
		//	auto size = std::filesystem::file_size(outPath);
		//	std::cout << "Size after write: " << size << std::endl;
		//	std::cout.flush();
		//} else {
		//	std::cerr << "File does not exist after write: " << outPath << std::endl;
		//}

	} catch (const std::exception& e) {
		std::cerr << "Exception in saveAsJSON: " << e.what() << std::endl;
		std::cerr.flush();
	}

}