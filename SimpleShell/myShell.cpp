/*  
    Programmer: Diego Vela
    myShell.cpp: Simple Command Line Interpreter
    Date: 2/11/2023
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <thread>

//Function to thread commands
void basicCommands(std::string);

int main() {
    //Declarations
    std::string userInput;
    std::string firstArg;

    //Intro
    std::cout << "Welcome to myShell!" << "\n";
    
    //Main loop to re-prompt user
    while(1) {
        
        //Get the line from the user
        std::cout << "\n==>";
        std::getline(std::cin, userInput);
        //Create found variable for multiple arguments
        std::size_t found = userInput.find(" ");

        //Less than one argument
        if (userInput.compare("") == 0) { continue; }
        
        //Check if more than one argument
        if (found != std::string::npos) {
            firstArg = userInput.substr(0,found);
            userInput = userInput.substr(found);
        } else { 
            firstArg = userInput; 
            userInput = "";
        }

        //Exit Case
        if (firstArg.compare("exit") == 0) {
            break;
        
        //Single Argument Case
        } else if (firstArg.compare("dir") == 0 || firstArg.compare("help") == 0 || firstArg.compare("vol") == 0 || firstArg.compare("path") == 0 || firstArg.compare("tasklist") == 0 || firstArg.compare("notepad") == 0)  {
            std::thread command(basicCommands, firstArg);
            command.join();

        //Double Argument Case
        } else if (firstArg.compare("echo") == 0 || firstArg.compare("color") == 0 || firstArg.compare("ping") == 0) { 
            std::thread command(basicCommands, firstArg + userInput);
            command.join();
        } else {
            continue;
        }        
    }

    std::cout << "\nThank you for using myShell";
    return EXIT_SUCCESS;
}

void basicCommands(std::string command) {
    system(command.c_str());
}
