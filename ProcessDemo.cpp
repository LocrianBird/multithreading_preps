#include "tchar.h"
#include <Windows.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
	STARTUPINFO startupInfo = { 0 };
	PROCESS_INFORMATION processInformation = { 0 };

	BOOL bSuccess = CreateProcess(
		TEXT("C:\\Windows\\notepad.exe"), NULL, NULL,
		NULL, FALSE, NULL, NULL, NULL, &startupInfo,
		&processInformation);

	if (bSuccess)
	{
		cout << "Process Started." << endl
			<< "Process ID:\t"
			<< processInformation.dwProcessId << endl;
	}
	else
	{
		cout << "Process cannot start!" << endl
			<< "Error Code:\t" << GetLastError() << endl;
	}

	CloseHandle(processInformation.hProcess);
	return system("pause");
}