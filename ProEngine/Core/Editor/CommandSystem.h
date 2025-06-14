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

        void PrintRegisteredCommands()
        {
            // 1. Collect the names of the commands
            std::vector<std::string> commandNames;
            for (const auto& [name, _] : commands_)
                commandNames.push_back(name);

            // 2. Determine the length of the longest command
            size_t maxCommandLength = 0;
            for (const auto& cmd : commandNames)
                maxCommandLength = std::max(maxCommandLength, cmd.size());

            // 3. Configuration
            const int padding = 4;  // total padding (2 spaces before and 2 after the command)
            const std::string border = "###############";  // side border (15 characters)
            const int contentWidth = static_cast<int>(maxCommandLength + padding);  // center area width
            const int lineWidth = border.size() * 2 + 2 + 1 + contentWidth + 1 + 2; // border + spaces + pipes + content

            // 4. Header
            {
                std::string title = " REGISTERED COMMANDS ";
                int sideWidth = (lineWidth - static_cast<int>(title.size())) / 2;
                std::string line = std::string(sideWidth, '#') + title + std::string(lineWidth - title.size() - sideWidth, '#');
                PENGINE_CORE_INFO("{}", line);
            }

            // 5. Commands with absolute alignment
            for (const auto& cmd : commandNames)
            {
                int space = contentWidth - static_cast<int>(cmd.size());
                int left = space / 2;
                int right = space - left;

                std::string centered = std::string(left, ' ') + cmd + std::string(right, ' ');

                std::string line =
                    border + " |" +
                    centered +
                    "| " + border;

                PENGINE_CORE_INFO("{}", line);
            }

            // 6. Footer
            PENGINE_CORE_INFO("{0:#^{1}}", "", lineWidth);

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
