/* flist.cpp - list files currently on the Windows clipboard.
 *
 * Copyright (C) 2025 Quin Gillespie.
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#include <cwctype>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <shlobj.h>

void print_file_info(const wchar_t* file_path);

int wmain(int argc, wchar_t* argv[]) {
	bool detailed_info{false};
	if (argc > 1) {
		std::wstring arg{argv[1]};
		for (auto& ch : arg) ch = std::towlower(ch);
		if (arg == L"-i") detailed_info = true;
	}
	if (!OpenClipboard(nullptr)) {
		std::wcerr << L"Failed to open clipboard.\n";
		return 1;
	}
	HANDLE data{GetClipboardData(CF_HDROP)};
	if (!data) {
		std::wcout << L"No files on clipboard.\n";
		CloseClipboard();
		return 0;
	}
	HDROP drop{static_cast<HDROP>(GlobalLock(data))};
	if (!drop) {
		std::wcerr << L"Failed to read clipboard data.\n";
		CloseClipboard();
		return 1;
	}
	UINT file_count{DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0)};
	if (file_count == 0) std::wcout << L"No files on clipboard.\n";
	else {
		std::wcout << L"Clipboard contains " << file_count << L" item" << (file_count > 1 ? L"s:\n" : L":\n");
		for (UINT i = 0; i < file_count; ++i) {
			wchar_t file_path[MAX_PATH];
			if (DragQueryFileW(drop, i, file_path, MAX_PATH)) {
				if (detailed_info) {
					print_file_info(file_path);
				} else {
					std::wcout << file_path << L"\n";
				}
			}
		}
	}
	GlobalUnlock(data);
	CloseClipboard();
	return 0;
}

void print_file_info(const wchar_t* file_path) {
	WIN32_FILE_ATTRIBUTE_DATA file_info;
	if (!GetFileAttributesExW(file_path, GetFileExInfoStandard, &file_info)) {
		std::wcerr << L"Failed to get info for: " << file_path << L"\n";
		return;
	}
	std::wcout << file_path << L"\n";
	if (file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) std::wcout << L"\tType: Directory\n";
	else std::wcout << L"\tType: File\n";
	LARGE_INTEGER size;
	size.LowPart = file_info.nFileSizeLow;
	size.HighPart = file_info.nFileSizeHigh;
	std::wcout << L"\tSize: " << size.QuadPart << L" bytes\n";
	FILETIME ft = file_info.ftLastWriteTime;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	std::wcout << L"\tLast Modified: " << st.wYear << L"-" << st.wMonth << L"-" << st.wDay << L" " << st.wHour << L":" << st.wMinute << L":" << st.wSecond << L"\n";
	std::wcout << L"\tAttributes: ";
	bool has_attributes = false;
	if (file_info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) { std::wcout << L" Read-Only"; has_attributes = true; }
	if (file_info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) { std::wcout << L" Hidden"; has_attributes = true; }
	if (file_info.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) { std::wcout << L" System"; has_attributes = true; }
	if (!has_attributes) std::wcout << L" none";
	std::wcout << L"\n\n";
}
