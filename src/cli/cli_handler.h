#ifndef CLI_HANDLER_H
#define CLI_HANDLER_H

#include <string>

using namespace std ;

class CLIHandler {
public:
    void run() ;
private:
    void showMenu() ;
    void processCommand(string command) ;
};

#endif // CLI_HANDLER_H