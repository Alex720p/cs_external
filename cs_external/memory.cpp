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

std::uintptr_t memory::get_module_address(const std::wstring& mod_name) {
	if (!this->m_proc_id)
		throw std::runtime_error("m_proc_id has to be initialized in order to get the module of the process");

	if (this->m_proc_modules.count(mod_name)) //already found the module once
		return this->m_proc_modules[mod_name];

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_proc_id);
	MODULEENTRY32 mod_entry;
	mod_entry.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshot, &mod_entry)) {
		while (Module32Next(snapshot, &mod_entry)) {
			if (!mod_name.compare(mod_entry.szModule)) { //in this case == 0 means that the strings are 'equal'
				this->m_proc_modules.insert(std::pair<std::wstring, std::uintptr_t>(mod_name, reinterpret_cast<std::uintptr_t>(mod_entry.modBaseAddr))); //switch to unordered map
				CloseHandle(snapshot);
				return (std::uintptr_t)mod_entry.modBaseAddr;
			}
		}
	}

	CloseHandle(snapshot);
	return 0; //module not found
}