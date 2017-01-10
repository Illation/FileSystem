#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
void DisplayError(LPTSTR lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	lpDisplayBuf =
		(LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf)
			+ lstrlen((LPCTSTR)lpszFunction)
			+ 40) // account for format string
			* sizeof(TCHAR));

	if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error code %d as follows:\n%s"),
		lpszFunction,
		dw,
		lpMsgBuf)))
	{
		printf("FATAL ERROR: Unable to output error code.\n");
	}

	_tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
FILE_HANDLE FILE_BASE::Open( const char * pathName, FILE_ACCESS_FLAGS accessFlags, FILE_ACCESS_MODE accessMode )
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wFilename = converter.from_bytes(std::string(pathName));

	UINT mode = 0;
	UINT share = 0;
	switch (accessMode)
	{
	case Read:
		mode |= GENERIC_READ;
		share = FILE_SHARE_READ;
		break;
	case Write:
		mode |= GENERIC_WRITE;
		break;
	case ReadWrite:
		mode |= GENERIC_READ | GENERIC_WRITE;
		break;
	}
	UINT flags = OPEN_ALWAYS;//might need to revise those flags meanings
	if (accessFlags.GetFlag(FILE_ACCESS_FLAGS::FLAGS::Truncate)
		&& accessFlags.GetFlag(FILE_ACCESS_FLAGS::FLAGS::Exists))
	{
		//std::cout << "TRUNCATE_EXISTING" << std::endl;
		flags = TRUNCATE_EXISTING;
	}
	else if (accessFlags.GetFlag(FILE_ACCESS_FLAGS::FLAGS::Create)
		&& !accessFlags.GetFlag(FILE_ACCESS_FLAGS::FLAGS::Exists))
	{
		//std::cout << "CREATE_NEW" << std::endl;
		flags = CREATE_NEW;
	}
	else if (accessFlags.GetFlag(FILE_ACCESS_FLAGS::FLAGS::Create))
	{
		//std::cout << "CREATE_ALWAYS" << std::endl;
		flags = CREATE_ALWAYS;
	}
	else if (accessFlags.GetFlag(FILE_ACCESS_FLAGS::FLAGS::Exists))
	{
		//std::cout << "OPEN_EXISTING" << std::endl;
		flags = OPEN_EXISTING;
	}
	/*else
	{
		std::cout << "OPEN_ALWAYS" << std::endl;
	}*/

	auto ret = CreateFile(wFilename.c_str(), mode, share, NULL, flags, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( ret == INVALID_HANDLE_VALUE)
	{
		DisplayError(TEXT("CreateFile"));
	}
	return ret;
}

bool FILE_BASE::Close( FILE_HANDLE handle )
{
	return CloseHandle(handle);
}

bool FILE_BASE::ReadFile( FILE_HANDLE handle, std::string & content )
{
	const DWORD bufferSize = GetFileSize(handle, NULL) + 1;
	char* buffer_read = new char[bufferSize];
	DWORD bytes_read = 0;
	if (FALSE == ::ReadFile(handle, buffer_read, bufferSize - 1, &bytes_read, NULL))
	{
		return false;
	}
	if (bytes_read >= 0 && bytes_read <= bufferSize - 1) //move buffer to string
	{
		buffer_read[bytes_read] = '\0'; // NULL character
		content = buffer_read;
		return true;
	}
	return false;
}

bool FILE_BASE::WriteFile( FILE_HANDLE handle, const std::string & content)
{
	DWORD bytesWritten = 0;
	if (FALSE == ::WriteFile(handle, content.c_str(), content.size(), &bytesWritten, NULL))
	{
		return false;
	}
	else if (bytesWritten != content.size()) return false;
	return true;
}
