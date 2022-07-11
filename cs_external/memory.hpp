#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <stdexcept>
#include <unordered_map>

//simple memory helper class

class memory {
private:
	DWORD m_proc_id = 0; //used for the get_module_handle
	HANDLE m_proc_handle = NULL;
	std::unordered_map<std::wstring, std::uintptr_t> m_proc_modules;

public:
	bool open_handle(const std::wstring& proc_name); //attach to process
	std::uintptr_t get_module_address(const std::wstring& mod_name);

	template <typename TYPE>
	TYPE read_memory(const std::uintptr_t& address) { //ReadProcessMemory wrapper
		if (!this->m_proc_handle)
			throw std::runtime_error("m_proc_handle has to be initialized with open_handle() in order to rpm the process");

		TYPE out = { 0 };
		SIZE_T buf = 0;
		if (!ReadProcessMemory(this->m_proc_handle, reinterpret_cast<LPCVOID>(address), &out, sizeof(TYPE), &buf))
			return NULL; //read failed

		return out;
	}

	~memory() {
		if (this->m_proc_handle)
			CloseHandle(this->m_proc_handle);
	}
};