#pragma once

#include <ctime>
#include <fstream>

class BufTabEntry;

class BufferManager {
public:
	BufferManager(int size, int constraint, int page_size);
	~BufferManager();
	bool AddEntry(uint8_t* pointer, int id);
	void LoadUpdate(int id);
	void ReadUpdate(int id);
	void WriteUpdate(int id);
	void SaveUpdate(int id);
	int RequestID();
	bool isDirty(int id);
	bool CopyPageFromDisk(int id, char* buffer);
	bool SavePageToDisk(int id, char* buffer);
	int GetCurrentSize();
private:

	fstream file_on_disk;
	std::vector<BufTabEntry> entryArray;
	int max_size;
	int current_size;
	int number_of_open_pages;
	int page_constraint;
	int io_time;
	int page_size;
};