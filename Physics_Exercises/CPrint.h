#ifndef __CPRINT_H__
#define __CPRINT_H__

#if _WIN32
#include <windows.h>
#include <iostream>

namespace cprint
{
	//Used to move the Console Cursor to a point on the screen for more accurate text management.
	static void GotoXY(COORD _pos)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _pos);
	};

	//Used to print out text at the specified coordinate, with the specified effect.
	static void Print(COORD pos, std::wstring str, int effect = 15)
	{
		GotoXY(pos);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), effect);
		std::wcout << str;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	};

	//Prints text at coord with colour one letter at a time
	static void SlowPrint(COORD _pos, std::wstring _message, int effect = 15, int _wait = 10)
	{
		GotoXY(_pos);
		for (wchar_t _char : _message)
		{
			Sleep(_wait);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), effect);
			std::wcout << _char;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}

#endif

#endif