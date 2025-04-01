/* fpaste.cpp - paste files from the Windows clipboard into the current directory.
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
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include <shellapi.h>

namespace fs = std::filesystem;

std::vector<std::wstring> get_clipboard_files();

int wmain() {
	auto files = get_clipboard_files();
	if (files.empty()) {
		std::wcout << L"No files on clipboard.\n";
		return 1;
	}
	fs::path cwd = fs::current_path();
	std::wostringstream output;
	for (const auto& file : files) {
		fs::path dest = cwd / fs::path(file).filename();
		try {
			fs::copy(file, dest, fs::copy_options::overwrite_existing);
			output << L"Copied " << file << L" to " << dest << L"\n";
		} catch (const std::exception& e) {
			std::wcerr << L"Failed to copy " << file << L": " << e.what() << L"\n";
		}
	}
	std::wcout << output.str();
	return 0;
}

std::vector<std::wstring> get_clipboard_files() {
	std::vector<std::wstring> files;
	if (!OpenClipboard(nullptr)) return files;
	HANDLE data = GetClipboardData(CF_HDROP);
	if (!data) {
		CloseClipboard();
		return files;
	}
	HDROP df = static_cast<HDROP>(data);
	UINT count = DragQueryFileW(df, 0xFFFFFFFF, nullptr, 0);
	for (UINT i = 0; i < count; ++i) {
		std::vector<wchar_t> file(MAX_PATH);
		if (DragQueryFileW(df, i, file.data(), MAX_PATH)) files.emplace_back(file.data());
	}
	CloseClipboard();
	return files;
}
