#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#define WIN32_LEAN_AND_MEAN
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
	auto cwd = fs::current_path();
	for (const auto& file : files) {
		auto dest = cwd / fs::path(file).filename();
		std::wcout << L"Copying " << file << L" to " << dest << L"\n";
		fs::copy(file, dest, fs::copy_options::overwrite_existing);
	}
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
