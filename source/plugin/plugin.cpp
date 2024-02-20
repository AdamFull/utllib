#include "plugin.h"
#include <utils/hash.h>

using namespace utl::plugin_system;

#ifdef _WIN32
using dllmain = BOOL(WINAPI*)(HINSTANCE dll, DWORD reason, LPVOID reserved);

//Structure relocation entry based on : https://docs.microsoft.com/fr-fr/windows/win32/debug/pe-format#the-reloc-section-image-only
typedef struct IMAGE_RELOCATION_ENTRY {
	WORD Offset : 12;
	WORD Type : 4;
} IMAGE_RELOCATION_ENTRY, * PIMAGE_RELOCATION_ENTRY;

#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")
#endif

uplugin::~uplugin()
{
	if (m_pModule)
		unload();
}

bool uplugin::load(const stl::vector<u8>& data)
{
	if (data.empty())
		return false;

#ifdef _WIN32
	const HANDLE hDLLData = HeapAlloc(GetProcessHeap(), 0, data.size());
	RtlCopyMemory((LPVOID)hDLLData, data.data(), data.size());

	const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)(hDLLData);
	const auto lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)hDLLData + lpImageDOSHeader->e_lfanew);

	if (lpImageNTHeader->Signature != IMAGE_NT_SIGNATURE)
		return false;

	if (!(lpImageNTHeader->FileHeader.Characteristics & IMAGE_FILE_DLL))
		return false;

	if (lpImageNTHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC)
		return false;

	m_pModule = VirtualAlloc(nullptr, lpImageNTHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!m_pModule)
		return false;

	const auto lpImageSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)lpImageNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) + lpImageNTHeader->FileHeader.SizeOfOptionalHeader);

	const DWORD_PTR dDeltaAddress = (DWORD_PTR)m_pModule - lpImageNTHeader->OptionalHeader.ImageBase;

	lpImageNTHeader->OptionalHeader.ImageBase = (DWORD_PTR)m_pModule;

	RtlCopyMemory(m_pModule, hDLLData, lpImageNTHeader->OptionalHeader.SizeOfHeaders);

	const IMAGE_DATA_DIRECTORY ImageDataReloc = lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	const IMAGE_DATA_DIRECTORY ImageDataImport = lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	PIMAGE_SECTION_HEADER lpImageRelocHeader{ nullptr };
	PIMAGE_SECTION_HEADER lpImageImportHeader{ nullptr };
	for (int i = 0; i < lpImageNTHeader->FileHeader.NumberOfSections; i++)
	{
		const auto lpCurrentSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)lpImageSectionHeader + (i * sizeof(IMAGE_SECTION_HEADER)));
		if (ImageDataReloc.VirtualAddress >= lpCurrentSectionHeader->VirtualAddress && ImageDataReloc.VirtualAddress < (lpCurrentSectionHeader->VirtualAddress + lpCurrentSectionHeader->Misc.VirtualSize))
			lpImageRelocHeader = lpCurrentSectionHeader;

		if (ImageDataImport.VirtualAddress >= lpCurrentSectionHeader->VirtualAddress && ImageDataImport.VirtualAddress < (lpCurrentSectionHeader->VirtualAddress + lpCurrentSectionHeader->Misc.VirtualSize))
			lpImageImportHeader = lpCurrentSectionHeader;

		RtlCopyMemory((LPVOID)((DWORD_PTR)m_pModule + lpCurrentSectionHeader->VirtualAddress), (LPVOID)((DWORD_PTR)hDLLData + lpCurrentSectionHeader->PointerToRawData), lpCurrentSectionHeader->SizeOfRawData);
	}

	if (!lpImageRelocHeader || !lpImageImportHeader)
		return false;

	DWORD_PTR RelocOffset = 0;

	while (RelocOffset < ImageDataReloc.Size)
	{
		const auto lpImageBaseRelocation = (PIMAGE_BASE_RELOCATION)((DWORD_PTR)hDLLData + lpImageRelocHeader->PointerToRawData + RelocOffset);
		RelocOffset += sizeof(IMAGE_BASE_RELOCATION);
		const DWORD_PTR NumberOfEntries = (lpImageBaseRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_RELOCATION_ENTRY);
		for (DWORD_PTR i = 0; i < NumberOfEntries; i++)
		{
			const auto lpImageRelocationEntry = (PIMAGE_RELOCATION_ENTRY)((DWORD_PTR)hDLLData + lpImageRelocHeader->PointerToRawData + RelocOffset);
			RelocOffset += sizeof(IMAGE_RELOCATION_ENTRY);

			if (lpImageRelocationEntry->Type == 0)
				continue;

			const DWORD_PTR AddressLocation = (DWORD_PTR)m_pModule + lpImageBaseRelocation->VirtualAddress + lpImageRelocationEntry->Offset;

			DWORD_PTR PatchedAddress = 0;

			RtlCopyMemory((LPVOID)&PatchedAddress, (LPVOID)AddressLocation, sizeof(DWORD_PTR));

			PatchedAddress += dDeltaAddress;

			RtlCopyMemory((LPVOID)AddressLocation, (LPVOID)&PatchedAddress, sizeof(DWORD_PTR));
		}
	}

	auto lpImageImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)m_pModule + lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	if (!lpImageImportDescriptor)
		return false;


	// Import other symbols
	while (lpImageImportDescriptor->Name != 0)
	{
		const auto lpLibraryName = (LPSTR)((DWORD_PTR)m_pModule + lpImageImportDescriptor->Name);
		const HMODULE hModule = LoadLibraryA(lpLibraryName);
		if (!hModule)
			return false;

		auto lpThunkData = (PIMAGE_THUNK_DATA)((DWORD_PTR)m_pModule + lpImageImportDescriptor->FirstThunk);
		while (lpThunkData->u1.AddressOfData != 0)
		{
			if (IMAGE_SNAP_BY_ORDINAL(lpThunkData->u1.Ordinal))
			{
				const auto functionOrdinal = (UINT)IMAGE_ORDINAL(lpThunkData->u1.Ordinal);
				lpThunkData->u1.Function = (DWORD_PTR)GetProcAddress(hModule, MAKEINTRESOURCEA(functionOrdinal));
			}
			else
			{
				const auto lpData = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)m_pModule + lpThunkData->u1.AddressOfData);
				const auto functionAddress = (DWORD_PTR)GetProcAddress(hModule, lpData->Name);
				lpThunkData->u1.Function = functionAddress;
			}

			lpThunkData++;
		}

		lpImageImportDescriptor++;
	}

	if (lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress)
	{
		const auto lpImageTLSDirectory = (PIMAGE_TLS_DIRECTORY)((DWORD_PTR)m_pModule + lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		auto lpCallbackArray = (PIMAGE_TLS_CALLBACK*)lpImageTLSDirectory->AddressOfCallBacks;

		while (*lpCallbackArray != nullptr)
		{
			const auto lpImageCallback = *lpCallbackArray;
			lpImageCallback((PVOID)hDLLData, DLL_PROCESS_ATTACH, nullptr);
			lpCallbackArray++;
		}
	}

	// Load function table
	if (lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
	{
		const auto lpImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)m_pModule + lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		m_vFunctionTable.resize(lpImageExportDirectory->NumberOfNames);

		char lpCurrentFnctionNameDemangled[1024]{ 0 };
		for (int i = 0; i < (int)lpImageExportDirectory->NumberOfNames; i++)
		{
			const auto lpCurrentFunctionName = (LPSTR)(((DWORD*)(lpImageExportDirectory->AddressOfNames + (DWORD_PTR)m_pModule))[i] + (DWORD_PTR)m_pModule);
			const auto lpCurrentOridnal = ((WORD*)(lpImageExportDirectory->AddressOfNameOrdinals + (DWORD_PTR)m_pModule))[i];
			const auto addRVA = ((DWORD*)((DWORD_PTR)m_pModule + lpImageExportDirectory->AddressOfFunctions))[lpCurrentOridnal];
			UnDecorateSymbolName(lpCurrentFunctionName, lpCurrentFnctionNameDemangled, sizeof(lpCurrentFnctionNameDemangled), UNDNAME_COMPLETE);

			m_tableMap[utils::fnv1a_64_hash(lpCurrentFnctionNameDemangled)] = lpCurrentOridnal;
			m_vFunctionTable.at(lpCurrentOridnal) = ((DWORD_PTR)m_pModule + addRVA);
		}
	}

	const auto main = (dllmain)((DWORD_PTR)m_pModule + lpImageNTHeader->OptionalHeader.AddressOfEntryPoint);
	if (!main((HINSTANCE)m_pModule, DLL_PROCESS_ATTACH, nullptr))
		return false;

	HeapFree(GetProcessHeap(), 0, hDLLData);
#endif

	return true;
}

bool uplugin::unload()
{
#ifdef _WIN32
	const auto lpImageDOSHeader = (PIMAGE_DOS_HEADER)m_pModule;
	const auto lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpImageDOSHeader + lpImageDOSHeader->e_lfanew);

	if (lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress)
	{
		const auto lpImageTLSDirectory = (PIMAGE_TLS_DIRECTORY)((DWORD_PTR)m_pModule + lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		auto lpCallbackArray = (PIMAGE_TLS_CALLBACK*)lpImageTLSDirectory->AddressOfCallBacks;

		while (*lpCallbackArray != nullptr)
		{
			const auto lpImageCallback = *lpCallbackArray;
			lpImageCallback(m_pModule, DLL_PROCESS_DETACH, nullptr);
			lpCallbackArray++;
		}
	}

	const auto main = (dllmain)((DWORD_PTR)m_pModule + lpImageNTHeader->OptionalHeader.AddressOfEntryPoint);
	if (!main((HINSTANCE)m_pModule, DLL_PROCESS_DETACH, nullptr))
		return false;

	if (!VirtualFree(m_pModule, 0, MEM_RELEASE))
		return false;
#endif

	return true;
}

intptr_t uplugin::getFunctionAddress(const char* function_name)
{
	auto hash = utils::fnv1a_64_hash(function_name);
	if (auto found = m_tableMap.find(hash); found != m_tableMap.end())
		return getFunctionAddress(found->second);

	return static_cast<intptr_t>(0);
}

intptr_t uplugin::getFunctionAddress(u64 function_index)
{
	return m_vFunctionTable.at(function_index);
}