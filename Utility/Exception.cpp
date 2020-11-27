#include "stdafx.h"
#include "Exception.h"
#include <sstream>

Exception::Exception(int line, const char* file)
	:line(line),file(file)
{
}

const char* Exception::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Exception::GetType() const
{
	return "Exception";
}

int Exception::GetLine() const
{
	return line;
}

const std::string& Exception::GetFile() const
{
	return file;
}

std::string Exception::GetOriginString() const
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
