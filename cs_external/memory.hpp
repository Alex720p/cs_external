#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <stdexcept>
#include <map>

//simple memory helper class

class memory {
private:
	DWORD m_proc_id = 0; //used for the get_module_handle
	HANDLE m_proc_handle = NULL;
	std::map<std::wstring, std::uintptr_t> m_proc_modules;

public:
	bool open_handle(std::wstring& proc_name); //attach to process
	bool get_module_handle(std::wstring& mod_name);
	//add module getter

	template <typename TYPE>
	TYPE read_memory(std::uintptr_t address); //ReadProcessMemory wrapper

	~memory() {
		if (this->m_proc_handle)
			CloseHandle(this->m_proc_handle);
	}
};