// CommandSystem.h

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <iostream>

#include "Core/Log/PELog.h"

namespace ProEngine
{
    using CommandCallback = std::function<void(const std::vector<std::string>&)>;

    class CommandSystem
    {
    public:
        static CommandSystem& Get()
        {
            static CommandSystem instance;
            return instance;
        }

        void RegisterCommand(const std::string& name, CommandCallback callback)
        {
            commands_[name] = callback;
        }

        void Execute(const std::string& input)
        {
            std::istringstream stream(input);
            std::string command;
            stream >> command;

            std::vector<std::string> args;
            std::string arg;
            while (stream >> arg)
                args.push_back(arg);

            auto it = commands_.find(command);
            if (it != commands_.end())
            {
                it->second(args);
            }
            else
            {
                PENGINE_CORE_CRITICAL("Unknown command: {}", command);
            }
        }

        bool HasCommand(const std::string& name) const
        {
            return commands_.find(name) != commands_.end();
        }

    private:
        std::unordered_map<std::string, CommandCallback> commands_;
        CommandSystem() = default;
    };
}
