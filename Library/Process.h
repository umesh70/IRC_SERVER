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
        {Command::START, "START"},
        {Command::STOP, "STOP"},
        {Command::RESTART, "RESTART"},
        {Command::STATUS, "STATUS"},
        {Command::HELP, "HELP"},
        {Command::INVALID, "INVALID"}
    };
    //Reverse map for switch case use
    const std::unordered_map<std::string, Process::Command>CommandMap = {
        {"START", Command::START},
        {"STOP", Command::STOP},
        {"RESTART",Command::RESTART},
        {"STATUS", Command::STATUS},
        {"HELP", Command::HELP},
        {"INVALID",Command::INVALID}
    };

    // Function to process command-line arguments
    std::string ArgProcess(int argc, char* argv[]) noexcept;

    // Function to perform post-processing
    void PostProcess(std::string) noexcept;
}