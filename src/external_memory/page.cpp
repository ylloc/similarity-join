#include <page.h>

#include <iostream>

using namespace std;

Page::Page(int size, std::shared_ptr<BufferManager> manager): buffer_manager(manager), page_size_(size) {
	buffer = new uint8_t[page_size_];
	is_located_in_memory_ = true;
}

Page::~Page() {
	if (is_located_in_memory_) {
		delete[] buffer;
	}
}

int Page::GetPageId() { return page_id; }

bool Page::LoadPageFromFile() {
	if (is_located_in_memory_) {
		return true;
    }

	if (not buffer) {
		buffer = new char[pageSize];
    } else {
		return false;
	}

	if (not buffer_manager->CopyPageFromDisk(page_id_, buffer)) {
		cout << "reading error" << endl;
    }
	buffer_manager->LoadUpdate(page_size_);
	is_located_in_memory_ = true;
	return true;
}

bool Page::SaveToFile() {
	if (not is_located_in_memory_) {
		return false;
    }
	if (buffer_manager->isDirty(page_id_)) {
		buffer_manager->SavePageToDisk(nodeID, buffer);
	}
	is_located_in_memory_ = false;
	bm->SaveUpdate(nodeID);
	delete [] buffer;
	buffer = nullptr;
	return true;
}