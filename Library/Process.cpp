#include "Process.h"
#include "StringUtil.h"
#include "Socket.h"
#include <algorithm>
#include <iostream>

std::string Process::ArgProcess(int argc, char* argv[]) noexcept {
    Command ReturnVal = Command::INVALID;

    if (argc != 2)
        return ServerMap.at(ReturnVal);
    
    std::string tmp = Util::RemoveWhitespace(argv[1]);
    bool check = Util::IsLowerCase(tmp); // Assuming there's a ToLower function to handle case conversion.
    if(check == false){
        exit(5);
    }
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
        return "Error: Invalid Command";
    }
}


void Process::PostProcess(std::string ParsedCommand) noexcept
{
    auto it = Process::CommandMap.find(ParsedCommand);
    if (it == Process::CommandMap.end())
    {
        std::cout << "Invalid" << std::endl;
        return;
    }

    switch (it->second)
    {
    case Process::Command::START:
        try
        {
            Socket::Socket SockObj(Socket::DEFAULT_PORT);
            SockObj.ListenSocket();
            std::cout << "Server Start" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to start server: " << e.what() << std::endl;
        }
        break;

    case Process::Command::STOP:
        std::cout << "Server stop" << std::endl;
        break;
    
    case Process::Command::INVALID:
        std::cout << "Invalid" << std::endl;
        break;
    
    default:
        std::cerr << "Unhandled command" << std::endl;
        break;
    }
}
