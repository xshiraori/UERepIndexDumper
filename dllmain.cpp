#include <Windows.h>
#include "ConsoleLogger.hpp"
#include <string>
#include <iostream>
#include "horizon_dumper/HorizonDumper.hpp"
#include "horizon_dumper/IndexDumper.hpp"

using namespace std;
using namespace UE503;

ConsoleLogger& logger = ConsoleLogger::Instance();

std::vector<std::string> rpcBlacklist =
{
    "Tick",
    "ServerRecieveClientInput",
    "ServerRecieveClientPreferences"
};

void hookedProcessEvent(void* Object, UFunction* Function, void* Parms)
{    
    auto FunctionName = Function->GetName();
    auto ClassName = Function->OuterPrivate->GetName();

    if (std::find(rpcBlacklist.begin(), rpcBlacklist.end(), FunctionName) == std::end(rpcBlacklist) && FunctionName.starts_with("Server"))
    {
        ConsoleLogger::Instance().Log("Class : {} and Function: {}\n", ClassName, FunctionName);
    }

    return reinterpret_cast<ProcessEventType>(PROCESS_EVENT_ADDRESS_ABSOLUTE)(Object, Function, Parms);
}

void InterceptObjectEvents(UObject* object)
{
    constexpr auto VTableEntrySize = sizeof(void*);

    if (std::bit_cast<uintptr_t>(object->VFTable[UObjectProcessEventOffset]) == PROCESS_EVENT_ADDRESS_ABSOLUTE)
    {
        DWORD oldProtection;
        VirtualProtect(&object->VFTable[UObjectProcessEventOffset], VTableEntrySize, PAGE_EXECUTE_READWRITE, &oldProtection);
        object->VFTable[UObjectProcessEventOffset] = hookedProcessEvent;
        VirtualProtect(&object->VFTable[UObjectProcessEventOffset], VTableEntrySize, oldProtection, nullptr);
    }
}


BOOL APIENTRY DllMain( HMODULE moduleBase,
                       DWORD  attachReason,
                       LPVOID lpReserved
                     )
{
    if (attachReason == DLL_PROCESS_ATTACH)
    {
        logger.switchMode(ConsoleLogger::Mode::CONSOLE);
        logger.Log("Injected!\n");

        HorizonDumper::instance().forEachObject([&](UObject* Object)
        {
            InterceptObjectEvents(Object);
        });
    }

    return TRUE;
}

