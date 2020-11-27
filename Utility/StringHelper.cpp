#include "stdafx.h"
#include "StringHelper.h"

std::wstring StringHelper::StringToWide(std::string str)
{
	return std::wstring(str.begin(),str.end());
}

std::string StringHelper::GetDirectoryFromPath(const std::string& filepath)
{
	auto last_index = filepath.find_last_of("\\/");
	auto directory = filepath.substr(0, last_index + 1);

	return directory;
}
//확장자를 얻어옴
std::string StringHelper::GetFileExtension(const std::string& filename)
{
	size_t off = filename.find_last_of('.');
	if (off == std::string::npos)
		return {};

	return filename.substr(0,off + 1);
}
