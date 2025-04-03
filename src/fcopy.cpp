/* fcopy.cpp - copy files to the Windows clipboard from the command line.
 *
 * Copyright (C) 2025 Quin Gillespie.
 *
 * This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>
#include <shlobj.h>

namespace fs = std::filesystem;

void copy_to_clipboard(const std::vector<std::wstring>& files);

int wmain(int argc, wchar_t** argv) {
	if (argc < 2) {
		std::wcerr << L"Usage: " << argv[0] << L" <file1> [file2 ...]\n";
		return 1;
	}
	std::vector<std::wstring> files;
	for (int i{1}; i < argc; ++i) files.emplace_back(fs::absolute(argv[i]).wstring());
	copy_to_clipboard(files);
	return 0;
}

void copy_to_clipboard(const std::vector<std::wstring>& files) {
	if (files.empty()) return;
	size_t total_size{sizeof(DROPFILES) + sizeof(wchar_t)};
	for (const auto& file : files) total_size += (file.size() + 1) * sizeof(wchar_t);
	auto global_mem{std::unique_ptr<void, decltype(&GlobalFree)>(GlobalAlloc(GHND, total_size), GlobalFree)};
	if (!global_mem) return;
	auto* df{static_cast<DROPFILES*>(GlobalLock(global_mem.get()))};
	if (!df) return;
	df->pFiles = sizeof(DROPFILES);
	df->fWide = TRUE;
	wchar_t* data{reinterpret_cast<wchar_t*>(df + 1)};
	for (const auto& file : files) {
		std::copy(file.begin(), file.end(), data);
		data += file.size();
		*data++ = L'\0';
	}
	*data = L'\0';
	GlobalUnlock(global_mem.get());
	if (OpenClipboard(nullptr)) {
		EmptyClipboard();
		SetClipboardData(CF_HDROP, global_mem.release());
		CloseClipboard();
	}
}
