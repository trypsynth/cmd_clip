/* fclear.cpp - clear any files from the Windows clipboard.
 *
 * Copyright (C) 2025 Quin Gillespie.
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <array>
#include <iostream>
#include <windows.h>

int wmain() {
	if (!OpenClipboard(nullptr)) {
		std::wcerr << L"Failed to open clipboard. Error: " << GetLastError() << L"\n";
		return 1;
	}
	const std::array<UINT, 1> formats_to_remove{CF_HDROP};
	bool cleared_anything{false};
	for (const auto& format : formats_to_remove) {
		if (IsClipboardFormatAvailable(format)) {
			if (!EmptyClipboard()) {
				std::wcerr << L"Failed to clear clipboard content.\n";
				CloseClipboard();
				return 1;
			}
			cleared_anything = true;
		}
	}
	CloseClipboard();
	if (cleared_anything) std::wcout << L"File contents removed from clipboard.\n";
	return 0;
}
