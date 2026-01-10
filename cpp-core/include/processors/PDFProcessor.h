#pragma once
#include <string>
#include <vector>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <tesseract/baseapi.h>


class PDFProcessor {

public:

	PDFProcessor(const std::string& filePath);

	// final result
	std::string extractTextHybrid();

	~PDFProcessor();

private:

	std::string filePath;
	poppler::document* doc = nullptr;

	std::string processPage(poppler::page* pg);

	tesseract::TessBaseAPI* getThreadTess();

};