#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <signal.h>
#include <algorithm>
#include <stdio.h>
#define sz(x) (int) x.size()

using namespace std;

inline bool shellStatus = true;
const int NUMBER_OF_COMMANDS = 17;

// trang thai cua shell hien tai
// true: live
// false: exit

struct process {
    unsigned int id;
    string cmdName;
    int status;
    PROCESS_INFORMATION pi;
};
inline vector<process> listProcess;
inline process fgProcess; // foreground process, used to terminate process when press CTRL-C
inline string currentDir;

// beautifier
inline string defaultCor = "\033[0m";
inline string color[10] = {"\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m"};
                            // red          green       yellow          blue        magenta         cyan

inline string errorMsg = color[0] + "ERROR: ";
inline string usageMsg = color[5] + "USAGE: ";

inline string cmdLst[NUMBER_OF_COMMANDS] = {
    "help",
    "exit",
    "time",
    "dir",
    "pwd",
    "cat",
    "clear",
    "run",
    "stop",
    "resume",
    "kill",
    "list",
    "path",
    "addpath",
    "cd",
    "delete",
    "move"

};

inline string instuctionLst[NUMBER_OF_COMMANDS] = {
    "provide information to shell commands",
    "exit shell",
    "display current date and time",
    "display a list of files and subdirectories in current directory",
    "display path to current directory",
    "display content of specified file",
    "clear screen",
    "execute .bat file or start a new process to run .exe file",
    "stop a process permanently",
    "resume a stopped process",
    "kill a process",
    "list all background processes",
    "get environment PATH variable",
    "add environment PATH variable",
    "change current directory",
    "delete a file",
    "move a file to other directory"
};

// Command handle
void _help();
void _exit();
void _time();
void _dir(string s);
void _pwd();
void _cat(string s);
void _clear();
void _run(string s);
void _stop(string s);
void _resume(string s);
void _kill(string s);
void _list();
void _path();
void _addpath(string s);
void _cd(string s);
void _delete(string s); 
void _move(string s);
// Other utilities
vector<string> shell_read_command();
string remove_space(string s);
vector<string> remove_space(vector<string> commandList);
void shell_execute(vector<string> commandList);
void shell_execute_single(string command);
void slow_type(string msg);
void runDotBat(string fileName);
vector<string> split_space(string inst);
string getPath();
string getFileName(string s);
string getPreviousPath(string s);
void my_handler(sig_atomic_t s);

#endif
