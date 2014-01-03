在低权限下枚举进程列表
ULONG GetSystemProcessInfoSize()
{
    ULONG ulSize = sizeof(SYSTEM_PROCESS_INFORMATION);
	ULONG ulRet;
	    NTSTATUS ntStatus;
	LPBYTE pSystemProcInfo = new BYTE[ulSize];
	do
	{
		VirtualFree(pSystemProcInfo, 0, MEM_RELEASE);
		        ulSize += sizeof(SYSTEM_PROCESS_INFORMATION);
		pSystemProcInfo = (LPBYTE)VirtualAlloc(NULL, ulSize, MEM_COMMIT, PAGE_READWRITE);
		ntStatus = NtQuerySystemInformation(SystemProcessInformation, pSystemProcInfo, ulSize, &ulRet);
	 
	}while (STATUS_INFO_LENGTH_MISMATCH == ntStatus);
	 
	VirtualFree(pSystemProcInfo, 0, MEM_RELEASE);
	return ulSize;
}
	 
int _tmain(int argc, _TCHAR* argv[])
{
	ULONG ulRet;
	ULONG ulSize = GetSystemProcessInfoSize();
	LPBYTE pBuffer = new BYTE[ulSize];
	PSYSTEM_PROCESS_INFORMATION pSystemProcInfo;
	 
	NTSTATUS ntStatus = NtQuerySystemInformation(SystemProcessInformation, pBuffer, ulSize, &ulRet);
	 
	if (NT_SUCCESS(ntStatus))
	{
		pSystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)pBuffer;
	 
		do 
		{           
            if (0 != (DWORD)pSystemProcInfo->ProcessId && 4 != (DWORD)pSystemProcInfo->ProcessId)
            {
                    wprintf_s(L"%d %s %d K ", 
                    pSystemProcInfo->ProcessId, pSystemProcInfo->ImageName.Buffer,
                    pSystemProcInfo->VirtualMemoryCounters.PeakWorkingSetSize/1024);
            }
	 
		pSystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)((LONG)pSystemProcInfo + pSystemProcInfo->NextEntryOffset);
		 
		} while (0 != pSystemProcInfo->NextEntryOffset);
		 
	}
	 
	system("pause");
	 
	return 0;
}
