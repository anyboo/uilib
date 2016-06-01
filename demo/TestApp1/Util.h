#pragma once

HBITMAP CreateDesktopBitmap(HWND hWnd);
HBITMAP CreateDesktopMaskBitmap(HWND hWnd);
HWND SmallestWindowFromCursor(RECT& rcWindow);
void BitmapToJpg(HBITMAP hbmpImage, int width, int height, const std::wstring& path);