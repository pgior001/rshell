#include "Base.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class Terminal
{
    public:
        Terminal() {commandList = 0;}

        void Run()
        {
            //sets up prompt
            GetLogin(login);
            GetHostName(hostName);
            //set up infinite loop
            bool run = true;
            while(run){
                //output prompt then gather input to run
                cout << '[' << login << "@" << hostName << ']' << '$' << ' ';
                ReadInput();
                Execute();
            }
        }

        Base* commandList;

    private:
        char login[256];
        char hostName[256];

        void GetLogin(char* Login)
        {
            //gets user name for extra credit
            getlogin_r(Login, 256);
            return;
        }

        void GetHostName(char* name)
        {
            //gets host name for extra credit
            gethostname(name, 256);
            return;
        }

        void ReadInput()
        {
            //reads user input and calls Decipher to make command tree
            string command;
            getline(cin, command);
            if(command.find("#") != string::npos)
            {
                command = command.substr(0, command.find("#"));
            }
            if (command != "")
            {
                Decipher(command);
            }
        }

        void Decipher(string c)
        {
            //use keystroke to separate keywords from input stored
            Base* tmp;
            Base* tmp2;
            string cmd = c;
            string op;
            string sep;
            //goes through the command input looking through all chars and 
            //finding parsing strings such as && then parses the string
            //creating a command
            for (unsigned i = 0; i < cmd.size() - 1; ++i)
            {
                if (cmd.at(i) == '&' && cmd.at(i+1) == '&')
                {
                    //empty list check to create first node
                    if (commandList == 0)
                    {
                        op = cmd.substr(0, i);
                        cmd = cmd.substr(i+2, cmd.size()-1);
                        i = 0;
                        commandList = new Operand(op);
                        sep = "and";
                    }
                    //creates operator based on what was last seen and
                    //updates what operator was last seen while creating
                    //the operator
                    else
                    {
                        op = cmd.substr(0, i);
                        cmd = cmd.substr(i+2, cmd.size());
                        i = 0;
                        tmp = new Operand(op);
                        if (sep == "and")
                        {
                            tmp2 = new And(commandList, tmp);
                            commandList = tmp2;
                        }
                        else if (sep == "or")
                        {
                            tmp2 = new  Or(commandList, tmp);
                            commandList = tmp2;
                        }
                        else if (sep == "semicolon")
                        {
                            tmp2 = new Semicolon(commandList, tmp);
                            commandList = tmp2;
                        }
                        sep = "and";
                    }
                }
                //all parts same as and parse
                else if (cmd.at(i) == '|' && cmd.at(i+1) == '|')
                {
                    if (commandList == 0)
                    {
                        op = cmd.substr(0, i);
                        cmd = cmd.substr(i+2, cmd.size()-1);
                        i = 0;
                        commandList = new Operand(op);
                        sep = "or";
                    }
                    else
                    {
                        op = cmd.substr(0, i);
                        cmd = cmd.substr(i+2, cmd.size());
                        i = 0;
                        tmp = new Operand(op);
                        if (sep == "and")
                        {
                            tmp2 = new And(commandList, tmp);
                            commandList = tmp2;
                        }
                        else if (sep == "or")
                        {
                            tmp2 = new  Or(commandList, tmp);
                            commandList = tmp2;
                        }
                        else if (sep == "semicolon")
                        {
                            tmp2 = new Semicolon(commandList, tmp);
                            commandList = tmp2;
                        }
                        sep = "or";
                    }

                }
                //all parts same as and parse
                else if (cmd.at(i) == ';')
                {
                    if (commandList == 0)
                    {
                        op = cmd.substr(0, i);
                        cmd = cmd.substr(i+1, cmd.size()-1);
                        i = 0;
                        commandList = new Operand(op);
                        sep = "semicolon";
                    }
                    else
                    {
                        op = cmd.substr(0, i);
                        cmd = cmd.substr(i+1, cmd.size());
                        i = 0;
                        tmp = new Operand(op);
                        if (sep == "and")
                        {
                            tmp2 = new And(commandList, tmp);
                            commandList = tmp2;
                        }
                        else if (sep == "or")
                        {
                            tmp2 = new  Or(commandList, tmp);
                            commandList = tmp2;
                        }
                        else if (sep == "semicolon")
                        {
                            tmp2 = new Semicolon(commandList, tmp);
                            commandList = tmp2;
                        }
                        sep = "semicolon";
                    }

                }
            }
            //creates inital node if none were created
            if (commandList == 0)
            {
                commandList = new Operand(cmd);
            }
            //creates operator if node already exists
            else
            {
                tmp = new Operand(cmd);
                if (sep == "and")
                {
                    tmp2 = new And(commandList, tmp);
                    commandList = tmp2;
                }
                else if (sep == "or")
                {
                    tmp2 = new  Or(commandList, tmp);
                    commandList = tmp2;
                }
                else if (sep == "semicolon")
                {
                    tmp2 = new Semicolon(commandList, tmp);
                    commandList = tmp2;
                }
            }
        }

        void Execute()
        {
            //Read in element from vector (will need to parse them), execute
            // corresponding command, and delete vector entry
            if (this->commandList != 0)
            {
                if (this->commandList->execute() == -1)
                {
                    //commandList->clean();
                    delete commandList;
                    exit(0);
                }
                //commandList->clean();
                delete commandList;
            }
            //make null pointer after delete to ensure seg fault on bad access
            commandList = 0;
        }
};
