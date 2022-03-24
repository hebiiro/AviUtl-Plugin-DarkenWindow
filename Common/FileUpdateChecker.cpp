#include "pch.h"
#include "FileUpdateChecker.h"

BOOL getFileTime(LPCWSTR filePath, FILETIME* fileTime)
{
	HANDLE file = ::CreateFileW(filePath, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

	if (file == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL retValue = ::GetFileTime(file, 0, 0, fileTime);

	::CloseHandle(file);

	return retValue;
}

FileUpdateChecker::FileUpdateChecker(LPCWSTR filePath)
{
	MY_TRACE(_T("FileUpdateChecker::FileUpdateChecker(%ws)\n"), filePath);

	m_filePath = filePath;
	getFileTime(filePath, &m_fileTime);
}

BOOL FileUpdateChecker::isFileUpdated()
{
//	MY_TRACE(_T("FileUpdateChecker::isFileUpdated()\n"));

	FILETIME fileTime;

	if (!getFileTime(m_filePath, &fileTime))
		return FALSE;

	if (!::CompareFileTime(&m_fileTime, &fileTime))
		return FALSE;

	m_fileTime = fileTime;

	return TRUE;
}

FileUpdateCheckers::FileUpdateCheckers()
{
	MY_TRACE(_T("FileUpdateCheckers::FileUpdateCheckers()\n"));
}

void FileUpdateCheckers::clear()
{
	MY_TRACE(_T("FileUpdateCheckers::clear()\n"));

	m_items.clear();
}

void FileUpdateCheckers::add(LPCWSTR filePath)
{
	MY_TRACE(_T("FileUpdateCheckers::add(%ws)\n"), filePath);
	FileUpdateCheckerPtr ptr(new FileUpdateChecker(filePath));
	m_items.push_back(ptr);
}

BOOL FileUpdateCheckers::isFileUpdated()
{
//	MY_TRACE(_T("FileUpdateCheckers::isFileUpdated()\n"));

	for (auto item : m_items)
	{
		if (item->isFileUpdated())
			return TRUE;
	}

	return FALSE;
}
