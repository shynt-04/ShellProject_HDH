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
    shell_path = init_shell_path();
    currentDir = getPath();
    do {
        cout << color[1] << "tinyShell "<< color[3] << "<" << currentDir << ">" << defaultCor << " $ ";
        SetCurrentDirectory(currentDir.c_str());
        // xu li multiple command duoc ngan cach bang ;
        vector<string> commandList = shell_read_command();
        commandList = remove_space(commandList);
        shell_execute(commandList);
        cout << "\n";

    } while (shellStatus == true);
}

/*
    Khoi tao bien moi truong cho shell (path_env variable)
*/
vector<string> init_shell_path() {
    char *path_chr = getenv("PATH");
    string path_str = path_chr;
    string cur_path = "";
    vector<string> path;
    for (auto c : path_str) {
        if (c == ';') {
            if (cur_path != "") 
                path.push_back(cur_path);
            cur_path = "";
        } else cur_path += c;
    }
    return path;
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
    if(strncmp(command_chr , "echo" , 4)==0){
        _echo(command);
        return;
    }
	if(strncmp(command_chr,"delete",6)==0){
        _delete(command);
        return;
    }
    if (strncmp(command_chr, "move", 4) == 0) {  
        _move(command);
        return;
    }
    cout << errorMsg << "Invalid command" << defaultCor << "\n";
    cout << color[2] << "Please use 'help' command for more information" << defaultCor << "\n";
}

/*
    Ham tra lai path hien tai
*/
string getPath() {
    char buff[MAX_PATH];
    int n = GetCurrentDirectory(MAX_PATH, buff);
    if (n) return buff;
    return "";
}

/*
    Tach dong lenh thanh cac arguments
*/
vector<string> split_space(string inst) {
    vector<string> args;
    string s = "";
    inst += " ";
    for (int i = 0; i < sz(inst); ++ i) {
        if (inst[i] == ' ') {
            if (s != "") args.push_back(s), s = "";
        } else s += inst[i];
    }
    return args;
}

/*
    Chay tung dong trong file .bat
*/
void runDotBat(string fileName) {
    ifstream myFile(fileName);
    if (myFile.is_open()) {
        string line;
        while(getline(myFile, line)) {
            line = remove_space(line);
            shell_execute_single(line);
        }
    } else {
        cout << errorMsg << "Unable to open file" << defaultCor << "\n";
    }
}

/*
    lay file name tu mot cai path dai
    C:/abc/xyz/cde.exe -> cde.exe
*/
string getFileName(string dir) {
    if (dir[1] != ':') return dir;
    int cnt = 0;
    int len = sz(dir);
    for (int i = len - 1; i >= 0; -- i) {
        if (dir[i] == (TCHAR) '\\') {
            len = i;
            break;
        }
        cnt ++;
    }
    return dir.substr(len + 1, cnt);
}

/*
    Check ctrl-C co duoc press ko bang signal
*/
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            cout << color[4] << "Ctrl-C pressed. Terminating process..." << defaultCor  << "\n";
            TerminateProcess(fgProcess.pi.hProcess, 0);
            CloseHandle(fgProcess.pi.hProcess);
            CloseHandle(fgProcess.pi.hThread);
            return TRUE;

        default:
            return FALSE;
    }
}

/*
    Ham lay thu muc cha
*/
string getPreviousPath(string dir) {
    int len = dir.size();
    for (int i = len - 1; i >= 0; -- i) {
        if (dir[i] == (TCHAR) '\\') {
            len = i;
            break;
        }
    }
    return dir.substr(0, len);
}

/*
    Kiem tra xem directory co ton tai khong
*/
bool checkDir(string dirName) {
    WORD ftyp = GetFileAttributesA(dirName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}