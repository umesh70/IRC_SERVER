#pragma once
#include <string>
#include <string_view>
#include <map>
#include <unordered_map>
#include <array>

namespace Process {

    // Enum for command types
    enum class Command {
        START,
        STOP,
        RESTART,
        STATUS,
        HELP,
        INVALID
    };

    // Mapping of Command enum to server action strings
    const std::unordered_map<Command, std::string> ServerMap{
        {Command::START, "start"},
        {Command::STOP, "stop"},
        {Command::RESTART, "restart"},
        {Command::STATUS, "status"},
        {Command::HELP, "help"},
        {Command::INVALID, "invalid_command"}
    };
    //Reverse map for switch case use
    const std::unordered_map<std::string, Process::Command>CommandMap = {
        {"start", Command::START},
        {"stop", Command::STOP},
        {"restart",Command::RESTART},
        {"status", Command::STATUS},
        {"help", Command::HELP},
        {"invalid_command",Command::INVALID}
    };

    // Function to process command-line arguments
    std::string ArgProcess(int argc, char* argv[]) noexcept;

    // Function to perform post-processing
    void PostProcess(std::string) noexcept;
}