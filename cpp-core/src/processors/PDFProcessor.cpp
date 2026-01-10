#include "core/ThreadPool.h"
#include "processors/PDFProcessor.h"
#include <poppler/cpp/poppler-page-renderer.h>
#include <leptonica/allheaders.h>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

// every thread has its own tls_tes, independent from other threads
// TLS = THREAD LOCAL STORAGE
thread_local tesseract::TessBaseAPI* tls_tess = nullptr;

PDFProcessor::PDFProcessor(const std::string& fp):filePath(fp) {

	doc = poppler::document::load_from_file(fp.c_str());
	
}

PDFProcessor::~PDFProcessor() {
	if (doc != nullptr) delete doc;
}

// singleton pattern, for the TLS
tesseract::TessBaseAPI* PDFProcessor::getThreadTess() {

	if (tls_tess == nullptr) {
		tls_tess = new tesseract::TessBaseAPI();
		tls_tess->Init(nullptr,"eng",tesseract::OEM_LSTM_ONLY);
		tls_tess->SetPageSegMode(tesseract::PSM_AUTO);
	}

	return tls_tess;

}


std::string PDFProcessor::processPage(poppler::page* pg) {

	try
	{
		// poppler
		std::string text = pg->text().to_latin1();
		if (text.length() >= 150) return text;
		
		// tesseract fallback
		poppler::page_renderer renderer;
		// sharper OCR, using antialiasing
		renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);
		renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
		// render image 300x300 dpi, 24 bits per pixel
		poppler::image img = renderer.render_page(pg, 300, 300, poppler::image::format_rgb24);

		if (img.is_valid()) {
			// 32 bits per pixel RGBA
			Pix* pix = pixCreate(img.width(), img.height(), 32);
			const unsigned char* src = (unsigned char*)img.const_data();
			unsigned char* dst = (unsigned char*)pixGetData(pix);
			int stride = img.bytes_per_row();

			for (int y = 0; y < img.height(); y++) {
				// dst, src and amount to be compied
				memcpy(dst + (y * pixGetWpl(pix) * 4), src + (y * stride), stride);
			}

			tesseract::TessBaseAPI* ocr = getThreadTess();
			ocr->SetImage(pix);
			char* out = ocr->GetUTF8Text();

			if (out) {
				text = out;
				delete[] out;
			}
			pixDestroy(&pix);
			ocr->Clear();
		}

		return text;


	}
	catch (const std::exception& err)
	{
		std::cerr << "Error processing page: " << err.what()<< std::endl;
	}

}

// main function
std::string PDFProcessor::extractTextHybrid() {

	if (doc == nullptr || doc->is_locked()) {
		std::cerr << "Failed to load PDF: " << filePath << std::endl;
		return "";
	}

	int num_pages = doc->pages();
	if (num_pages == 0) return " ";

	std::vector<poppler::page*> pages(num_pages);
	for (int i = 0; i < num_pages; i++) {
		pages[i] = doc->create_page(i);
		if (pages[i] == nullptr) std::cerr << "Failed to create page " << i << std::endl;
	}

	std::vector<std::string> page_texts(num_pages);

	//Start of multithreading
	std::mutex text_mutex;
	int max_inner = std::min(12, std::max(1, (int)std::thread::hardware_concurrency));

	ThreadPool page_pool(std::min(max_inner, num_pages));
	for (int i = 0; i < num_pages; i++) {
		if (pages[i] == nullptr) continue;

		page_pool.enqueue([this,i,&pages,&text_mutex, &page_texts]() {
			std::string t = processPage(pages[i]);
			{
				std::lock_guard<std::mutex> lock(text_mutex);
				page_texts[i] = std::move(t);
			}
		});

	}

	page_pool.waitCompletion();
	for (auto* p : pages) if (p != nullptr) delete p;

	std::string full_text;
	for (const auto& t : page_texts) full_text += t + "\n";

	return full_text;

}