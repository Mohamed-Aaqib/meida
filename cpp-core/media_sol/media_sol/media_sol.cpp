#include <tesseract/baseapi.h>
#include <iostream>
#include "core/ThreadPool.h"
#include "core/Chunker.h"
#include "utils/FileUtils.h"
#include "processors/PDFProcessor.h"
#include <iostream>
#include <filesystem>

int main()
{

	std::string folderPath = "data/documents/";
	std::string outFolder = "data/outputs/";

	auto pdfFiles = listPDFFiles(folderPath);
	if (pdfFiles.empty()) {
		std::cout << "No PDFs found in " << folderPath << std::endl;
		return 0;
	}

	ThreadPool file_pool(std::thread::hardware_concurrency() / 2);
	for (const auto& file : pdfFiles) {

		file_pool.enqueue([file,outFolder]() {
			PDFProcessor pdf(file);
			std::string full_text = pdf.extractTextHybrid();
			auto chunks = Chunker::chunkText(full_text);
			std::string outPath = outFolder + std::filesystem::path(file).stem().string()+".json";
			std::cout << std::endl << outPath << std::endl;
			Chunker::saveAsJSON(outPath,chunks);
			//std::cout << "Processed: " << file << " -> " << outPath
			//	<< " (text length: " << full_text.length() << ")" << std::endl;		
		});
	}
	file_pool.waitCompletion();
	std::cout << "Components tested and tried !, except for the multithreading/parallelism" << std::endl;
	return 0;

}
