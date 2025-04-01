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

#include <iostream>
#include <windows.h>
#include <shlobj.h>

int wmain() {
	if (!OpenClipboard(nullptr)) {
		std::wcerr << L"Failed to open clipboard.\n";
		return 1;
	}
	HANDLE data = GetClipboardData(CF_HDROP);
	if (!data) {
		std::wcout << L"No files on clipboard.\n";
		CloseClipboard();
		return 0;
	}
	HDROP drop = static_cast<HDROP>(GlobalLock(data));
	if (!drop) {
		std::wcerr << L"Failed to read clipboard data.\n";
		CloseClipboard();
		return 1;
	}
	UINT file_count = DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0);
	if (file_count == 0) std::wcout << L"No files on clipboard.\n";
	else {
		for (UINT i = 0; i < file_count; ++i) {
			wchar_t file_path[MAX_PATH];
			if (DragQueryFileW(drop, i, file_path, MAX_PATH)) std::wcout << file_path << L"\n";
		}
	}
	GlobalUnlock(data);
	CloseClipboard();
	return 0;
}
