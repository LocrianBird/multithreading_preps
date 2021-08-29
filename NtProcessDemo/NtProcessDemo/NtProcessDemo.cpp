#include "tchar.h"
#include <Windows.h>
#include <Winternl.h>
#include <iostream>

using namespace std;

typedef NTSTATUS(NTAPI* QUERYINFORMATIONPROCESS)(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

int _tmain(int argc, _TCHAR* argv[])
{
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInformation = { 0 };

    BOOL bSuccess = CreateProcess(
        TEXT("C://Windows//notepad.exe"), NULL, NULL,
        NULL, FALSE, NULL, NULL, NULL, &startupInfo,
        &processInformation);

    if (bSuccess)
    {
        cout << "Process Started" << endl
            << "Process ID: "
            << processInformation.dwProcessId << endl;

        PROCESS_BASIC_INFORMATION pbi;
        ULONG uLength = 0;

        HMODULE hDLL = LoadLibrary(TEXT("C://Windows//System32//ntdll.dll"));

        if (hDLL)
        {
            QUERYINFORMATIONPROCESS QueryInformationProcess =
                (QUERYINFORMATIONPROCESS)GetProcAddress(
                    hDLL, "NtQueryInformationProcess");

            if (QueryInformationProcess)
            {
                NTSTATUS ntStatus = QueryInformationProcess(
                    processInformation.hProcess,
                    PROCESSINFOCLASS::ProcessBasicInformation,
                    &pbi, sizeof(pbi), &uLength);

                if (NT_SUCCESS(ntStatus))
                {
                    cout << "Process ID (from PCB): "
                        << pbi.UniqueProcessId << endl;
                }
                else
                {
                    cout << "Cannot open PCB!: "
                        << "Error code: " << GetLastError() << endl;
                }
            }
            else
            {
                cout << "Cannot get "
                    << "NtQueryInformationProcess function!" 
                    << endl << "Error code: " << GetLastError() << endl;
            }

            FreeLibrary(hDLL);
        }
        else
        {
            cout << "Connot load ntdll.dll!" << endl
                << "Error code: " << GetLastError() << endl;
        }
    }
    else
    {
        cout << "Process cannot start!" << endl
            << "Error Code: " << GetLastError() << endl;
    }
    
    return 0;
}

