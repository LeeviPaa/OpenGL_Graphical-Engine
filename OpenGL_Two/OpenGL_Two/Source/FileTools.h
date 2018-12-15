#pragma once
#include <iostream>
#include <string>

class FileTools
{
public:
	static std::string programPath;

	static int SetPath(char* path) {
		int len = 0;
		while(path[len])
		{
			len++;
		}

		programPath.append(path);

		//replace \ with /
		for (int i = 0; i < len; i++)
		{
			if (programPath[i] == '\\')
			{
				programPath[i] = '/';
			}
		}

		//remove the .exe file from the path
		for (int i = len-1; i >= 0; i--)
		{
			if (programPath[i] == '/')
			{
				programPath.erase(i + 1, len - i -1);
				break;
			}
		}

		return len;
	}
};