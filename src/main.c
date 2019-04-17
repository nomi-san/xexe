
#include "util.h"
#include "quicklz.h"
#include "res.h"

#ifdef _MSC_VER
#ifdef __X64
#pragma comment(lib, "../../bxsdk/lib/bxsdk64.lib")
#else
#pragma comment(lib, "../../bxsdk/lib/bxsdk32.lib")
#endif
#include "../bxsdk/include/BoxedAppSDK.h"
#endif

#if (defined(WINMAIN) || defined(WINDOWS) || defined(_WINDOWS)) \
	&& (!defined(CONSOLE) || !defined(_CONSOLE))
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

	DWORD temp;
	HANDLE file = BoxedAppSDK_CreateVirtualFileA("__app__.exe", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
	WriteFile(file, app_data, app_size, &temp, NULL);
	CloseHandle(file);

	DWORD exit_code = 0;
	ExecuteA("__app__.exe", "", &exit_code);

	BoxedAppSDK_Exit();

	puts("done!");
	return exit_code;
}