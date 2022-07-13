#include <iostream>
#include "memory.hpp"
#include "utils.hpp"

//todo: runtime string obfuscation



int main() //catch exceptions as const, implement try catch
{
    memory mem;
    if (mem.open_handle(L"csgo.exe"))
        std::cout << "handle opened :)" << std::endl;

    if (mem.get_module_info(L"client.dll"))
        std::cout << "client.dll saved :)" << std::endl;

    std::uintptr_t local = mem.read_memory<std::uintptr_t>(mem.get_module_address(L"client.dll") + 0xDBF4CC);
    int health = mem.read_memory<int>(local + 0x100);
    std::cout << "health : " << health << std::endl;

    utils::move_mouse(100, 300);
}
