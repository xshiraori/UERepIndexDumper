#include "HorizonDumper.hpp"

class IndexDumper {
public:
    static IndexDumper& instance()
    {
        static IndexDumper _;
        return _;
    }

    IndexDumper(IndexDumper const&) = delete;
    IndexDumper& operator=(IndexDumper const&) = delete;

    ConsoleLogger& logger = ConsoleLogger::Instance();

    struct FPropertyInformation {
        FProperty Property;
        uint16_t RepIndex;
    };

    struct UFunctionInformation {
        UFunction Function;
        uint16_t RepIndex;
    };

    struct PropertyAndFunctionDump {
        std::vector<std::pair<std::string, FPropertyInformation>> properties;
        std::vector<std::pair<std::string, UFunctionInformation>> functions;
    };

    using PropertyMapping = std::unordered_map<std::string, FProperty>;
    using FunctionMapping = std::unordered_map<std::string, UFunction>;

    // actually makes sense to hash the object but who cares
    struct ObjectInfo {
        PropertyMapping ObjectPropertyMap;
        FunctionMapping ObjectFunctionMap;
        std::string className;
    };

    std::unordered_map<std::uintptr_t, ObjectInfo> ObjectInformationMap;

    std::unordered_map<std::string, PropertyAndFunctionDump> commit()
    {
        std::unordered_map<std::string, PropertyAndFunctionDump> SavedClassInfoMap;

        for (auto& [k, v] : ObjectInformationMap)
        {
            uint16_t index = 0;

            auto& Properties = v.ObjectPropertyMap;
            auto& Functions = v.ObjectFunctionMap;

            std::vector<std::pair<PropertyMapping::key_type, PropertyMapping::mapped_type>> propertyVector(Properties.begin(), Properties.end());
            std::vector<std::pair<FunctionMapping::key_type, FunctionMapping::mapped_type>> functionVector(Functions.begin(), Functions.end());

            std::sort(propertyVector.begin(), propertyVector.end(), [&](const auto& p1, const auto p2) { return p1.first < p2.first; });
            std::sort(functionVector.begin(), functionVector.end(), [&](const auto& f1, const auto f2) { return f1.first < f2.first; });

            PropertyAndFunctionDump savedInfo;

            for (auto& p : propertyVector)
            {
                FPropertyInformation propInfo;
                propInfo.Property = p.second;
                propInfo.RepIndex = index++;

                savedInfo.properties.emplace_back(p.first, propInfo);
            }

            for (auto& f : functionVector)
            {
                UFunctionInformation funcInfo;
                funcInfo.Function = f.second;
                funcInfo.RepIndex = index++;

                savedInfo.functions.emplace_back(f.first, funcInfo);
            }

            SavedClassInfoMap[v.className] = savedInfo;
        }

        logger.Log("Done!");
        return SavedClassInfoMap;
    }

    void DumpIndexes()
    {
        auto Result = commit();

        logger.switchMode(ConsoleLogger::Mode::FILE);
        for (auto [k, v] : Result)
        {
            if (v.functions.size() == 0)
                continue;

            logger.Log("Class : {}\n", k);
            for (auto f : v.functions)
            {
                logger.Log("\tFunction: {}, repIndex = {}\n", f.first, f.second.RepIndex);
            }
        }
    }


private:
    IndexDumper()
    {
        auto const isNetFunction = [&](UFunction* Function) { return static_cast<uint32_t>(Function->FunctionFlags) & static_cast<uint32_t>(EFunctionFlags::FUNC_Net); };
        auto const isNetProperty = [&](FProperty* Property) {return static_cast<uint32_t>(Property->PropertyFlags) & static_cast<uint32_t>(EPropertyFlags::CPF_Net); };

        auto& dumper = HorizonDumper::instance();

        dumper.forEachClass([&](UE503::UClassInternal* Class)
            {
                ObjectInfo oi;

                FunctionMapping functionMapping;
                dumper.forEachFunction(Class, [&](std::string functionName, UFunction* functionPointer)
                    {
                        if (isNetFunction(functionPointer))
                        {
                            functionMapping.insert({ functionName, *functionPointer });
                        }
                    });

                PropertyMapping propertyMapping;
                dumper.forEachProperty(Class, [&](FProperty* Property)
                    {
                        if (isNetProperty(Property))
                        {
                            propertyMapping.insert({ Property->GetName(), *Property, });
                        }
                    });

                oi.ObjectFunctionMap = functionMapping;
                oi.ObjectPropertyMap = propertyMapping;
                oi.className = Class->NamePrivate.GetName();

                ObjectInformationMap.insert({ reinterpret_cast<uintptr_t>(Class), oi });
            }
        );
    }
};