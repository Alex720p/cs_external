#include "./memory.hpp"

bool memory::open_handle(std::wstring& proc_name) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 proc_entry;
	proc_entry.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &proc_entry)) {
		while (Process32Next(snapshot, &proc_entry)) {
			if (!proc_name.compare(proc_entry.szExeFile)) { //in this case == 0 means that the strings are 'equal'
				this->m_proc_id = proc_entry.th32ProcessID;
				this->m_proc_handle = OpenProcess(PROCESS_VM_READ || PROCESS_VM_WRITE, FALSE, proc_entry.th32ProcessID);
				break;
			}
		}
	}

	CloseHandle(snapshot);

	if (this->m_proc_handle == NULL)
		return false;

	return true; //handle opened to desired process
}

bool memory::get_module_handle(std::wstring& mod_name) {
	if (!this->m_proc_id)
		throw std::runtime_error("m_proc_id has to be initialized in order to get the module of the process");

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_proc_id);
	MODULEENTRY32 mod_entry;
	mod_entry.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshot, &mod_entry)) {
		while (Module32Next(snapshot, &mod_entry)) {
			if (!mod_name.compare(mod_entry.szModule)) { //in this case == 0 means that the strings are 'equal'
				this->m_proc_modules.insert(std::pair<std::wstring, std::uintptr_t>(mod_name, reinterpret_cast<std::uintptr_t>(mod_entry.modBaseAddr)));
				CloseHandle(snapshot);
				return true;
			}
		}
	}

	CloseHandle(snapshot);
	return false; //module not found
}

template <typename TYPE>
TYPE memory::read_memory(std::uintptr_t address) {
	if (!this->m_proc_handle)
		throw std::runtime_error("m_proc_handle has to be initialized with open_handle() in order to rpm the process")

	TYPE out;
	if (!ReadProcessMemory(this->m_proc_handle, address, &out, sizeof(TYPE), NULL))
		return NULL; //read failed

	return out;
}