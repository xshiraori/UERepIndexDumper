#pragma once
#include "../ue_internal/FNamePool.hpp"
#include "../ue_internal/GUObjectArray.hpp"
#include "../ue_internal/UClass.hpp"
#include <Windows.h>
#include "../ConsoleLogger.hpp"
#include <string>
#include <unordered_map>

using namespace UE503;

#define GNAMES_OFFSET 0x16EF4B40
#define GOBJECTS_OFFSET 0x16FCAF80
#define GWORLD_OFFSET 0x171c35c0
#define PROCESS_EVENT_OFFSET 0x1a10de0

static const uintptr_t PROCESS_EVENT_ADDRESS_ABSOLUTE = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)) + PROCESS_EVENT_OFFSET;

class HorizonDumper final
{
public:
    struct HorizonContext
    {
        HorizonContext()
        {
            GlobalNames = reinterpret_cast<FNamePool*>(reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)) + GNAMES_OFFSET);
            GlobalObjects = reinterpret_cast<FUObjectArray*>(reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)) + GOBJECTS_OFFSET);

            ConsoleLogger::Instance().Log("Init NamePoolData: {:x}\n", reinterpret_cast<uintptr_t>(GlobalNames));
            ConsoleLogger::Instance().Log("Init GUObjectArray: {:x}\n", reinterpret_cast<uintptr_t>(GlobalObjects));

            ::NamePoolData = GlobalNames;
        }

        FNamePool* GlobalNames;
        FUObjectArray* GlobalObjects;
    };

    static HorizonDumper& instance()
    {
        static HorizonDumper _;
        return _;
    }

    void forEachClass(auto Callback)
    {
        auto GlobalObjects = HorizonDumper::instance().getContext().GlobalObjects;

        for (int32_t i = 0; i < GlobalObjects->ObjObjects.NumElements; i++) {
            auto item = GlobalObjects->ObjObjects.GetObjectPtr(i);
            if (item->Object != nullptr && item->Object->ClassPrivate)
            {
                auto Object = item->Object;
                auto Class = reinterpret_cast<UE503::UClassInternal*>(Object->ClassPrivate);

                Callback(Class);
            }
        }
    }

    void forEachProperty(UE503::UClassInternal* Class, auto Callback)
    {
        auto PropertyLink = Class->PropertyLink;

        while (PropertyLink)
        {
            Callback(PropertyLink);

            PropertyLink = PropertyLink->PropertyLinkNext;
        }
    }

    void forEachObject(auto Callback)
    {
        auto GlobalObjects = HorizonDumper::instance().getContext().GlobalObjects;

        for (int32_t i = 0; i < GlobalObjects->ObjObjects.NumElements; i++) {
            auto item = GlobalObjects->ObjObjects.GetObjectPtr(i);
            if (item->Object)
            {
                Callback(item->Object);
            }
        }
    }

    void forEachFunction(UE503::UClassInternal* Class, auto Callback)
    {
        auto FuncMap = Class->FuncMap;

        for (auto i = 0; i < FuncMap.Data.Num(); i++)
        {
            auto Element = FuncMap.Data[i];

            auto Key = Element.Value.First;
            auto Value = Element.Value.Second;

            auto FunctionName = Key.GetName();

            Callback(Key.GetName(), Value.Pointer);
        }
    }

    HorizonDumper(HorizonDumper const&) = delete;
    HorizonDumper& operator=(HorizonDumper const&) = delete;

    HorizonContext getContext() const { return m_context; }

private:
    HorizonDumper() = default;
    HorizonContext m_context;
};
