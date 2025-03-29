#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>
#include <shlobj.h>

void copy_to_clipboard(const std::vector<std::wstring>& files);

int wmain(int argc, wchar_t* argv[]) {
	if (argc < 2) {
		std::wcerr << L"Usage: " << argv[0] << L" <file1> [file2 ...]\n";
		return 1;
	}
	std::vector<std::wstring> files;
	files.reserve(argc - 1);
	for (int i = 1; i < argc; ++i) files.push_back(std::filesystem::absolute(argv[i]).wstring());
	copy_to_clipboard(files);
	return 0;
}

void copy_to_clipboard(const std::vector<std::wstring>& files) {
	if (files.empty()) return;
	size_t total_size = sizeof(DROPFILES) + sizeof(wchar_t);
	for (const auto& file : files) total_size += (file.size() + 1) * sizeof(wchar_t);
	auto global_mem = std::unique_ptr<void, decltype(&GlobalFree)>(GlobalAlloc(GHND, total_size), GlobalFree);
	if (!global_mem) return;
	auto* df = static_cast<DROPFILES*>(GlobalLock(global_mem.get()));
	df->pFiles = sizeof(DROPFILES);
	df->fWide = TRUE;
	auto* data = reinterpret_cast<wchar_t*>(df + 1);
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
