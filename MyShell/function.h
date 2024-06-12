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
#include <map>
#include <functional>
#define sz(x) (int) x.size()

using namespace std;

inline bool shellStatus = true;
const int NUMBER_OF_COMMANDS = 18;

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

inline vector<string> shell_path;

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
    "cd",
    "dir",
    "pwd",
    "clear",
    "cat",
    "run",
    "stop",
    "resume",
    "kill",
    "list",
    "path",
    "addpath",
    "echo",
    "delete",
    "move"
};

inline string instuctionLst[NUMBER_OF_COMMANDS] = {
    "provide information to shell commands",
    "exit shell",
    "display current date and time",
    "change current directory",
    "display a list of files and subdirectories in current directory",
    "display path to current directory",
    "clear screen",
    "display content of specified file",
    "execute .bat file or start a new process to run .exe file",
    "stop a process permanently",
    "resume a stopped process",
    "kill a process",
    "list all background processes",
    "get environment PATH variable",
    "add environment PATH variable",
    "displays messages, checks echo status",
    "delete a file",
    "move a file to other directory"
};

inline map<string, function<void(string s)> > func_map;

// Command handle
void tiny_help(string s);
void tiny_exit(string s);
void tiny_time(string s);
void tiny_dir(string s);
void tiny_pwd(string s);
void tiny_cat(string s);
void tiny_clear(string s);
void tiny_run(string s);
void tiny_stop(string s);
void tiny_resume(string s);
void tiny_kill(string s);
void tiny_list(string s);
void tiny_path(string s);
void tiny_addpath(string s);
void tiny_cd(string s);
void tiny_echo(string s);
void tiny_delete(string s);
void tiny_move(string s);
int tiny_run_exe_in_PATH(string s);

// Other utilities
void init_function();
vector<string> init_shell_path();
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
bool checkDir(string s);
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

#endif
