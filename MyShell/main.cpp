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
#include "function.h"

#define sz(x) (int) x.size()

using namespace std;

/******************************/
int main(int argc, char *argv[]) {
    SetConsoleTitle("Project: Tiny Shell");
    _clear();
    currentDir = getPath();
    do {
        cout << color[1] << "tinyShell "<< color[3] << "<" << currentDir << ">" << defaultCor << " $ ";
        // xu li multiple command duoc ngan cach bang ;
        vector<string> commandList = shell_read_command();
        commandList = remove_space(commandList);
        shell_execute(commandList);
        cout << "\n";

    } while (shellStatus == true);
}

/*
    Doc dong lenh nhap tu shell va phan tach cac lenh
*/

vector<string> shell_read_command() {
    vector<string> commandList;
    string current_command = "";
    while (true) {
        char c = getchar();
        if (c == '\n' || c == EOF) {
            if (current_command.size()) {
                commandList.push_back(current_command);
            }
            break;
        }
        if (c == ';') {
            commandList.push_back(current_command);
            current_command = "";
            continue;
        }
        current_command += c;
    }
    return commandList;
}

string remove_space(string s) {
    for (int i = (int) s.size() - 1; i >= 0; -- i) {
        if (s[i] == ' ') s.erase(i, 1);
        else break;
    }
    while (s[0] == ' ') s.erase(0, 1);
    return s;
}

vector<string> remove_space(vector<string> commandList) {
    for (auto &s : commandList) {
        s = remove_space(s);
    }
    return commandList;
}

void shell_execute(vector<string> commandList) {
    for (auto command : commandList) {
        shell_execute_single(command);
    }
}

/*
    output cham
*/

void slow_type(string msg) {
	int i = 0;
	while (1) {
		if (i == (int) msg.size())
			return;
		cout << msg[i];
		Sleep(10);
		i += 1;
	}
}

void shell_execute_single(string command) {
    const char* command_chr = command.c_str();
    if (strncmp(command_chr, "help", 4) == 0) {
        _help();
        return;
    }
    if (strncmp(command_chr, "exit", 4) == 0) {
        _exit();
        return;
    }
    if (strncmp(command_chr, "time", 4) == 0) {
        _time();
        return;
    }
    if (strncmp(command_chr, "dir", 3) == 0) {
        _dir(command);
        return;
    }
    if (strncmp(command_chr, "pwd", 3) == 0) {
        _pwd();
        return;
    }
    if (strncmp(command_chr, "cat", 3) == 0) {
        _cat(command);
        return;
    }
    if (strncmp(command_chr, "clear", 5) == 0) {
        _clear();
        return;
    }
    if (strncmp(command_chr, "run", 3) == 0) {
        _run(command);
        return;
    }
    if (strncmp(command_chr, "stop", 4) == 0) {
        _stop(command);
        return;
    }
    if (strncmp(command_chr, "resume", 6) == 0) {
        _resume(command);
        return;
    }
    if (strncmp(command_chr, "kill", 4) == 0) {
        _kill(command);
        return;
    }
    if (strncmp(command_chr, "list", 4) == 0) {
        _list();
        return;
    }
    if (strncmp(command_chr, "path", 4) == 0) {
        _path();
        return;
    }
    if (strncmp(command_chr, "addpath", 7) == 0) {
        _addpath(command);
        return;
    }
    if (strncmp(command_chr, "cd", 2) == 0) {
        _cd(command);
        return;
    }
    cout << errorMsg << "Invalid command" << defaultCor << "\n";
    cout << color[2] << "Please use 'help' command for more information" << defaultCor << "\n";
}
