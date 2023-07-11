#include "CPEEngine.h"
#include <winnt.h>

CPEEngine::CPEEngine()
{
}
void CPEEngine::LockOEP(const string& filepath) {

	LPHANDLE_MAPPING hMap = CFileApi::OpenFileToMapping(filepath);
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hMap->lpImageBase;
	IMAGE_NT_HEADERS64* ntHeader64 = (IMAGE_NT_HEADERS64*)(dosHeader->e_lfanew + hMap->lpImageBase);
	IMAGE_NT_HEADERS32* ntHeader32 = (IMAGE_NT_HEADERS32*)(dosHeader->e_lfanew + hMap->lpImageBase);
	

	//IMAGE_OPTIONAL_HEADER64 optionalHeader = ntHeader64->OptionalHeader;

	BYTE* EntryPoint = 0;
	int NumberOfSections = 0;
	IMAGE_SECTION_HEADER* sectionHeader = NULL;
	// �ж�PE�ļ�����
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
			// ������ڵ���ļ�ƫ����
			EntryPoint = (BYTE*)(sectionHeader->PointerToRawData + ((DWORD)EntryPoint - sectionHeader->VirtualAddress))+(QWORD)hMap->lpImageBase;
			break;
		}
		sectionHeader++;
	}

	BYTE oldCode[5] = { 0 };
	oldCode[0] = *(BYTE*)(EntryPoint);
	*(DWORD*)(oldCode + 1) = *(DWORD*)(EntryPoint + 1);
	if(*(DWORD*)oldCode!=0x909090C3){//����Ѿ���������д��
		*(BYTE*)(EntryPoint) = 0xC3;//ret
		*(DWORD*)(EntryPoint + 1) = 0x90909090;//д��4��90��ָֹ�����
	
		CFileApi::SaveBuffToFile(filepath, hMap->lpImageBase, hMap->fileSize,0,0);
		CFileApi::SaveBuffToFile(filepath, oldCode, sizeof(oldCode),0,1);
	}
	CFileApi::CloseFileMapping(hMap);
	
}

CPEEngine::~CPEEngine()
{
}
