#include "./memory.hpp"

bool memory::open_handle(const std::wstring& proc_name) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 proc_entry;
	proc_entry.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &proc_entry)) {
		while (Process32Next(snapshot, &proc_entry)) {
			if (!proc_name.compare(proc_entry.szExeFile)) { //in this case == 0 means that the strings are 'equal'
				this->m_proc_id = proc_entry.th32ProcessID;
				this->m_proc_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_entry.th32ProcessID);
				break;
			}
		}
	}

	CloseHandle(snapshot);

	if (this->m_proc_handle == NULL)
		return false;

	return true; //handle opened to desired process
}

bool memory::get_module_info(const std::wstring& mod_name) {
	if (!this->m_proc_id)
		throw std::runtime_error("m_proc_id has to be initialized in order to get the module of the process");

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_proc_id);
	MODULEENTRY32 mod_entry;
	mod_entry.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshot, &mod_entry)) {
		while (Module32Next(snapshot, &mod_entry)) {
			if (!mod_name.compare(mod_entry.szModule)) { //in this case == 0 means that the strings are 'equal'
				memory::module_info_t module_info = { reinterpret_cast<std::uintptr_t>(mod_entry.modBaseAddr), mod_entry.modBaseSize };
				this->m_proc_modules[mod_name] = module_info;
				CloseHandle(snapshot);
				return true;
			}
		}
	}
	CloseHandle(snapshot);
	return false; //module not found
}

std::uintptr_t memory::find_pattern(std::uintptr_t module_base_address, std::size_t module_size, const char* mask, const char* wildcards, int offset) { //todo: add some error handling and exceptions
	MEMORY_BASIC_INFORMATION mem_basic_info = { 0 };
	VirtualQueryEx(this->m_proc_handle, reinterpret_cast<LPCVOID>(module_base_address), &mem_basic_info, sizeof(mem_basic_info)); //getting the region size in mem_basic_info

	printf("line = %i    ,", strlen(mask));
	for (int i = 0; i < module_size; i += mem_basic_info.RegionSize) { //going throught the mem pages
		char* page_buffer = new char[mem_basic_info.RegionSize];
		SIZE_T tt = 0;
		ReadProcessMemory(this->m_proc_handle, reinterpret_cast<LPCVOID>(module_base_address + i), page_buffer, mem_basic_info.RegionSize, &tt);



		for (int j = 0; j < mem_basic_info.RegionSize - strlen(wildcards); j += strlen(wildcards)) {
			for (int k = 0; k < strlen(wildcards); k++) {
				if (page_buffer[j + k] == mask[k] || wildcards[k] == '?')
					if (k == strlen(wildcards) - 1)
						return i + j - offset; // mask found !
					else
						continue;


				if (page_buffer[j + k] != mask[k])
					break; //sequence not matching mask, break and go next
			}
		}
		delete[] page_buffer;
	}

	return 0;
}