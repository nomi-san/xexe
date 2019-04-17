
#include <stdio.h>
#include "util.h"
#include "quicklz.h"
#include "res.h"

#include "../bxsdk/include/BoxedAppSDK.h"

#if defined(_MSC_VER)
#if defined(__X64)
#pragma comment(lib, "../../bxsdk/lib/bxsdk64.lib")
#else
#pragma comment(lib, "../../bxsdk/lib/bxsdk32.lib")
#endif
#else
#error This compiler is not supported!
#endif

#if defined(__WINDOWS)
int __stdcall
WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpCmdLine, int nShowCmd)
#else
int __cdecl
main(int argc, char *argv[])
#endif
{
	void *bin_data;
	size_t bin_size;
	LoadRCData(NULL, APP_BIN, &bin_data, &bin_size);

	qlz_state_decompress qsdc;
	void *app_data = malloc(bin_size * 5);
	size_t app_size = qlz_decompress(bin_data, app_data, &qsdc);

	BoxedAppSDK_Init();
	const char *name = "__app__.exe";

	DWORD temp;
	HANDLE file = BoxedAppSDK_CreateVirtualFileA(name, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
	WriteFile(file, app_data, app_size, &temp, NULL);
	CloseHandle(file);

	DWORD exit_code = 0;

#if defined(__WINDOWS)
	ExecuteA(name, lpCmdLine, &exit_code);
#else
	char lpCmdLine[MAX_PATH];
	char *str = lpCmdLine;
	for (int i = 0; i < argc; i++) \
		str += sprintf(str, "%s ", argv[i]);

	lpCmdLine[str - lpCmdLine] = '\0';
	ExecuteA(name, lpCmdLine, &exit_code);
#endif

	BoxedAppSDK_Exit();
	free(app_data);
	return exit_code;
}