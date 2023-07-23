#include "CPEEngine.h"
#include <winnt.h>

CPEEngine::CPEEngine()
{
}

BOOL CPEEngine::LockOEP(const string& filepath,BOOL bLock) {
	BOOL result = FALSE;
	LPHANDLE_MAPPING hMap = CFileApi::OpenFileToMapping(filepath);
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hMap->lpImageBase;
	IMAGE_NT_HEADERS64* ntHeader64 = (IMAGE_NT_HEADERS64*)(dosHeader->e_lfanew + hMap->lpImageBase);
	IMAGE_NT_HEADERS32* ntHeader32 = (IMAGE_NT_HEADERS32*)(dosHeader->e_lfanew + hMap->lpImageBase);
	
	if (hMap->lpImageBase[0] != 'M' || hMap->lpImageBase[1] != 'Z') {
		CFileApi::CloseFileMapping(hMap);
		return result;
	}
	//IMAGE_OPTIONAL_HEADER64 optionalHeader = ntHeader64->OptionalHeader;

	BYTE* EntryPoint = 0;
	int NumberOfSections = 0;
	IMAGE_SECTION_HEADER* sectionHeader = NULL;
	// 判断PE文件类型
	if (ntHeader32->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		EntryPoint = (BYTE*)ntHeader32->OptionalHeader.AddressOfEntryPoint;
		NumberOfSections = ntHeader32->FileHeader.NumberOfSections;
		sectionHeader = IMAGE_FIRST_SECTION(ntHeader32);
	}
	else if (ntHeader64->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		EntryPoint = (BYTE*)ntHeader64->OptionalHeader.AddressOfEntryPoint;
		NumberOfSections = ntHeader64->FileHeader.NumberOfSections;
		sectionHeader = IMAGE_FIRST_SECTION(ntHeader64);
	}


	for (int i = 0; i <NumberOfSections; ++i)
	{
		if ((DWORD)EntryPoint >= sectionHeader->VirtualAddress && (DWORD)EntryPoint < (sectionHeader->VirtualAddress + sectionHeader->SizeOfRawData))
		{
			// 计算入口点的文件偏移量
			EntryPoint = (BYTE*)(sectionHeader->PointerToRawData + ((DWORD)EntryPoint - sectionHeader->VirtualAddress))+(QWORD)hMap->lpImageBase;
			break;
		}
		sectionHeader++;
	}
	if (EntryPoint == 0) {
		CFileApi::CloseFileMapping(hMap);
		return result;
	}
	BYTE oldCode[5] = { 0 };
	oldCode[0] = *(BYTE*)(EntryPoint);
	*(DWORD*)(oldCode + 1) = *(DWORD*)(EntryPoint + 1);
	if (bLock == TRUE) {
		if (*(DWORD*)oldCode != 0x909090C3) {//如果已经禁用则不再写入
			*(BYTE*)(EntryPoint) = 0xC3;//ret
			*(DWORD*)(EntryPoint + 1) = 0x90909090;//写入4个90防止指令混乱

			CFileApi::SaveBuffToFile(filepath, hMap->lpImageBase, hMap->fileSize, 0, 0);
			CFileApi::SaveBuffToFile(filepath, oldCode, sizeof(oldCode), 0, 1);
			result = TRUE;
		}
	}
	else {
		//unlock
		if (*(DWORD*)oldCode == 0x909090C3) {//如果已经禁用则修复入口
			BYTE* pRestoreCode = hMap->lpImageBase+ hMap->fileSize-5;

			*(BYTE*)(EntryPoint) = pRestoreCode[0];//ret
			*(DWORD*)(EntryPoint + 1) = *(DWORD*)(pRestoreCode+1);//写入4个90防止指令混乱

			CFileApi::SaveBuffToFile(filepath, hMap->lpImageBase, hMap->fileSize-5, 0, 0);
			result = TRUE;
		}
	}

	CFileApi::CloseFileMapping(hMap);
	return result;
}

CPEEngine::~CPEEngine()
{
}
