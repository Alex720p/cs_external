// cs_external.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "memory.hpp"

//todo: runtime string obfuscation

int main() //catch exceptions as const
{
    std::cout << "Hello World!\n";
    memory mem;
    if (mem.open_handle(L"csgo.exe"))
        std::cout << "handle opened :)";

}
