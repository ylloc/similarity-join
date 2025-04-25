#include <buffer_manager.h>

#include <memory>

class Page {
public:
	Page(int size, std::shared_ptr<BufferManager> manager);
	virtual ~Page();
	int GetPageId(); 
	bool LoadPageFromFile();
	bool SavePageToFile();
private:
	bool is_located_in_memory_{false};

protected:
	int page_size_{4096};
	int page_id_{0};
	uint8_t* buffer;
	std::shared_ptr<BufferManager> buffer_manager;
};
