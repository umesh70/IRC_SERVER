#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "Library/StringUtil.h"
#include "Library/Process.h"

int main(int argc ,char* argv[]) {

    std::string Parsed = Process::ArgProcess(argc , argv);

    Process::PostProcess(Parsed);
    

    return 0;
}
