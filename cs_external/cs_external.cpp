// cs_external.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "memory.hpp"

//todo: runtime string obfuscation

int main() //catch exceptions as const, implement try catch
{
    std::cout << "Hello World!\n";
    memory mem;
    if (mem.open_handle(L"csgo.exe"))
        std::cout << "handle opened :)";

    std::uintptr_t local = mem.read_memory<std::uintptr_t>(mem.get_module_address(L"client.dll")+ 0xDBF4CC);
    std::cout << GetLastError();
    int test = mem.read_memory<int>(local + 0x100);
    std::cout << test << '//';
    std::cout << GetLastError();
}
