#pragma once

class FileUpdateChecker
{
private:

	_bstr_t m_filePath;
	FILETIME m_fileTime;

public:

	FileUpdateChecker(LPCWSTR filePath);

	BOOL isFileUpdated();
};

typedef std::shared_ptr<FileUpdateChecker> FileUpdateCheckerPtr;
typedef std::list<FileUpdateCheckerPtr> FileUpdateCheckerContainer;

class FileUpdateCheckers
{
private:

	FileUpdateCheckerContainer m_items;

public:

	FileUpdateCheckers();

	void clear();
	void add(LPCWSTR filePath);
	BOOL isFileUpdated();
};
