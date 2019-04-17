#include "util.h"

#include <stdio.h>
#include <string.h>

int LoadFile(const char *filename, char **data)
{
	int size = 0;
	FILE *fp = fopen(filename, "rb");

	if (fp == NULL)
	{
		*data = NULL;
		return -1; // opening fail 
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = (char *)malloc(size + 1);

	if (size != fread(*data, sizeof(char), size, fp))
	{
		free(*data);
		return -2; // reading fail 
	}

	fclose(fp);
	(*data)[size] = 0;
	return size;
}

int SaveFile(const char *filename, const char *data, size_t size)
{
	FILE *fp = fopen(filename, "wb");

	if (fp != NULL)
	{
		int ret = fwrite(data, size, 1, fp);
		fclose(fp);

		return ret;
	}

	return -1;
}

int SaveHeaderBytes(const char *filename, const char *name, const void *data, size_t size)
{
	FILE *fp = fopen(filename, "w");

	const unsigned char *bytes = data;

	if (fp != NULL)
	{
		fprintf(fp, "static const unsigned char %s[] = {\n", name);

		for (size_t i = 0; i < size; i++)
		{
			if (i % 50 == 0)
			{
				fprintf(fp, "\n\t");
			}
			fprintf(fp, "0x%02x,", bytes[i]);
		}

		fprintf(fp, "\n};\n");
		fclose(fp);

		return 0;
	}

	return -1;
}

BOOL LoadRCData(HINSTANCE instance, DWORD id, void **mem, size_t *size)
{
	HRSRC hRes = FindResource(instance, MAKEINTRESOURCE(id), RT_RCDATA);

	if (hRes != NULL) {
		HGLOBAL hMem = LoadResource(instance, hRes);
		if (hMem != NULL) {
			void* pMem = LockResource(hMem);
			if (pMem != NULL) {
				DWORD uSize = SizeofResource(instance, hRes);
				if (mem != NULL) (*mem) = pMem;
				if (size != NULL) (*size) = uSize;
				return TRUE;
			}
		}
	}

	if (mem != NULL) (*mem) = NULL;
	return FALSE;
}

BOOL ExecuteA(const char *name, char *cmdLine, DWORD *exitCode)
{
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFOA startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	int nStrBuffer = strlen(cmdLine) + 50;

	startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	startupInfo.dwFlags = STARTF_USESTDHANDLES;

	// Create the process
	BOOL result = CreateProcessA(name, cmdLine, NULL, NULL, TRUE,
		NORMAL_PRIORITY_CLASS /*| CREATE_NO_WINDOW*/,
		NULL, NULL, &startupInfo, &processInformation);

	if (!result) {
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);

		fprintf(stderr, "Execution failed at CreateProcess()\nCommand = %s\nMessage = %s\n\n", cmdLine, (LPSTR)lpMsgBuf);

		// Free resources created by the system
		LocalFree(lpMsgBuf);

		// We failed.
		return FALSE;
	}
	else {
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		// Get the exit code.
		DWORD code;
		result = GetExitCodeProcess(processInformation.hProcess, &code);
		if (exitCode != NULL) (*exitCode) = code;

		// Close the handles.
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		// Flush all message printed to stdout (if Sleep is used).
		fflush(stdout);

		if (!result) {
			// Could not get exit code.
			fprintf(stderr, "Executed command but couldn't get exit code.\nCommand=%s\n", cmdLine);

			return FALSE;
		}

		// We succeeded.
		return TRUE;
	}
}

BOOL ExecuteW(wchar_t *cmdLine, DWORD *exitCode)
{
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFOW startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	int nStrBuffer = wcslen(cmdLine) + 50;

	startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	startupInfo.dwFlags = STARTF_USESTDHANDLES;

	BOOL result = CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS /*| CREATE_NO_WINDOW*/,
		NULL, NULL, &startupInfo, &processInformation);

	if (!result) {
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		fprintf(stderr, "Execution failed at CreateProcess()\nCommand = %ws\nMessage = %ws\n\n", cmdLine, (LPTSTR)lpMsgBuf);

		LocalFree(lpMsgBuf);
		return FALSE;
	}
	else {
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		DWORD code;
		result = GetExitCodeProcess(processInformation.hProcess, &code);
		if (exitCode != NULL) (*exitCode) = code;

		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		fflush(stdout);

		if (!result) {
			fprintf(stderr, "Executed command but couldn't get exit code.\nCommand=%ls\n", cmdLine);
			return FALSE;
		}

		return TRUE;
	}
}