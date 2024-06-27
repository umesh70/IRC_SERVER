#include "Process.h"
#include "StringUtil.h"

#include <algorithm>
#include <iostream>

std::string Process::ArgProcess(int argc, char* argv[]) noexcept {
    Command ReturnVal = Command::INVALID;

    if (argc != 2)
        return ServerMap.at(ReturnVal);
    
    std::string tmp = Util::RemoveWhitespace(argv[1]);
    tmp = Util::IsLowerCase(tmp); // Assuming there's a ToLower function to handle case conversion.

    if (tmp == "start")
        ReturnVal = Process::Command::START;
    else if (tmp == "stop") 
        ReturnVal = Process::Command::STOP;
    else if (tmp == "restart") 
        ReturnVal = Process::Command::RESTART;
     else if (tmp == "status") 
        ReturnVal = Process::Command::STATUS;
     else if (tmp == "help") 
        ReturnVal = Process::Command::HELP;
    
    if (ServerMap.find(ReturnVal) != ServerMap.end()) {
        return ServerMap.at(ReturnVal);
    } else {
        std::cerr << "Invalid command provided." << std::endl;
        return "Error: Invalid Command"; // or any other error handling
    }
}


void Process::PostProcess(std::string ParsedCommand) noexcept
{

    switch (Process::CommandMap.at(ParsedCommand))
    {

    case Process::Command::START:
        // IRCServer server;
        // server.listenOnSocket();
        std::cout << "Server Start";
        break;

    case Process::Command::STOP:
        std::cout << "Server stop";
        break;
    case Process::Command::INVALID:
        std::cout << "Invalid";
        break;
    default:
        break;
    }
}
