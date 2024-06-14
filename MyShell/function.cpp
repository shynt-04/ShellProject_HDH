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

#include "function.h"

using namespace std;

void init_function() {
    func_map["help"] = tiny_help;
    func_map["exit"] = tiny_exit;
    func_map["time"] = tiny_time;
    func_map["cd"] = tiny_cd;
    func_map["dir"] = tiny_dir;
    func_map["pwd"] = tiny_pwd;
    func_map["clear"] = tiny_clear;
    func_map["cat"] = tiny_cat;
    func_map["run"] = tiny_run;
    func_map["stop"] = tiny_stop;
    func_map["resume"] = tiny_resume;
    func_map["kill"] = tiny_kill;
    func_map["list"] = tiny_list;
    func_map["path"] = tiny_path;
    func_map["addpath"] = tiny_addpath;
    func_map["echo"] = tiny_echo;
    func_map["delete"] = tiny_delete;
    func_map["move"] = tiny_move;
}

/*
    Hien bang huong dan
*/
void tiny_help(string inst) {
    cout << color[2];
    cout <<   "############################\n";
    slow_type("WELCOME TO OUR SHELL PROJECT\n");
    cout <<   "############################\n";
    slow_type("Here is the list of instructions:\n\n");
    for (int i = 0; i < NUMBER_OF_COMMANDS; ++ i) {
        cout << color[5] << cmdLst[i] << "\t-\t" << instuctionLst[i] << defaultCor << "\n";
    }
    cout << "\n";
    cout << color[5] << "<cmd line>" << "\t" << "Execute command line in PATH" << "\n";
    cout << "\n";
    cout << color[2] << "Run mode: \n";
    cout << color[5] << "-f" << "\t-\t" << "Run a process in foreground mode (with CREATE_NEW_CONSOLE attribute)" << "\n";
    cout << color[5] << "-b" << "\t-\t" << "Run a process in background mode" << "\n";
    cout << defaultCor;
}

/*
    Thoat chuong trinh tinyShell
*/
void tiny_exit(string inst) {
    cout << color[0];
    cout << "*** Exit tinyShell ***" << "\n";
    cout << defaultCor;
    Sleep(1000);
    shellStatus = false;
    // Dong tat ca cac process/thread dang thuc hien
    for (int i = (int) listProcess.size() - 1; i >= 0; -- i) {
        TerminateProcess(listProcess[i].pi.hProcess, 0);
        CloseHandle(listProcess[i].pi.hProcess);
        CloseHandle(listProcess[i].pi.hThread);
    }
}

/*
    Ham tra lai thoi gian hien tai
*/
void tiny_time(string inst) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    cout << "Current time: " << st.wHour << ":" << st.wMinute <<":" << st.wSecond << "\n";
    cout << "Current date: " << st.wDay << ":" << st.wMonth << ":" << st.wYear <<"\n";
    cout << "Do you want to change time? (y/n) - ";
    string ans = "";
    while (true){
        char c = getchar();
        if (c == '\n' || c == EOF){
            break;
        }
        ans += c;
    }
    if(ans[0]!='n' && ans[0] !='y'){
        printf("Invalid answer");
    }
    else if(ans[0] == 'n') {}
    else {
    int hour, minute, second;
    cout << "Enter new time (hour:min:sec): ";
    scanf("%d:%d:%d", &hour, &minute, &second);
    char c = getchar();
    if ( c== '\n' || c == EOF){}
    st.wHour = hour;
    st.wMinute = minute;
    st.wSecond = second;

        if(SetLocalTime(&st)){
            cout << "Time successfully updated. \n";
        }
        else{
            cout << "Failed to update time.\n";
        }

    int day, month, year;
    cout << "Enter new date (dd/mm/yyyy):";
    scanf("%d/%d/%d", &day, &month, &year);
    c = getchar();
    if (c == '\n' || c == EOF){}
    st.wDay = day;
    st.wMonth = month;
    st.wYear = year;
        if (SetLocalTime(&st)) {
            cout << "Date successfully updated\n";
        } else {
            cout << "Failed to update date\n";
        }
    }
}

/*
    Liet ke file va subdirectories
*/
void tiny_dir(string inst) {
    vector<string> args = split_space(inst);
    string path;
    if (sz(args) < 2) path = currentDir;
    else path = args[1];
    cout << color[2];
    cout << "\t Directory: " << path << "\n\n";
    path = path + "\\" + "*";
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(path.c_str(), &data);
    if (hFind != INVALID_HANDLE_VALUE) {
        cout << "\n";
        cout << "\tLastWriteTime\t\tLength\t\tName" << "\n";
        cout << "\t-------------\t\t------\t\t----" << "\n";
        SYSTEMTIME tmpTime;
        do {
            FileTimeToSystemTime(&data.ftLastWriteTime, &tmpTime);
            char date[100];
            snprintf(date, 30, "     %02d/%02d/%04d  %02d:%02d:%02d", tmpTime.wDay, tmpTime.wMonth, tmpTime.wYear, tmpTime.wHour, tmpTime.wMinute, tmpTime.wSecond);
            string str(date);
            cout << setw(32) << left << str;
            cout << setw(15) << left << data.nFileSizeLow;
            cout << setw(20) << left << data.cFileName;
            cout << "\n";
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    } else {
        cout << errorMsg << "Unable to access directory" << defaultCor << "\n";
    }
    cout << defaultCor;
}

/*
    Ham nay goi den ham getPath()
*/
void tiny_pwd(string inst) {
    cout << color[2];
    cout << "Current working directory: " << currentDir << "\n";
    cout << defaultCor;
}

/*
    Clear man hinh
*/
void tiny_clear(string inst) {
    system("cls");
}


/*
    In ra file yeu cau, can duong dan day du hoac file nam trong duong dan hien tai
*/
void tiny_cat(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "cat <PATH_TO_FILE>" << defaultCor << "\n";
        return;
    }
    string path = args[1];
    if (path[1] != ':') path = currentDir + "\\" + path;
    ifstream myFile(path);
    if (myFile.is_open()) {
        string line;
        cout << color[4];
        while(getline(myFile, line)) {
            cout << line << "\n";
        }
        cout << defaultCor;
        myFile.close();
    } else {
        cout << errorMsg << "Unable to open file" << defaultCor << "\n";
    }
}

/*
    Chay file .exe va file .bat
*/
void tiny_run(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) == 2) {
        if (sz(args) != 2) {
            cout << errorMsg << "Invalid option" << defaultCor << "\n";
            cout << color[5] << "USAGE for bat file: run <batFile>" << defaultCor << "\n";
            return;
        }
        string fileName = args[1];
        runDotBat(fileName);
    } else if (strstr(inst.c_str(), ".exe") != NULL) {
        if (sz(args) < 3 || (args[1] != "-f" && args[1] != "-b")) {
            cout << errorMsg << "Invalid option" << defaultCor << "\n";
            cout << color[5] << "USAGE for .exe: run <OPTION> <exeFile>" << "\n";
            cout << "OPTION for .exe:" << "\n";
            cout << "  -f: foreground mode" << "\n";
            cout << "  -b: background mode" << defaultCor << "\n";
            return;
        }
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
//        int console_type = 0;
//        if (args[1] == "-b") console_type = CREATE_NEW_CONSOLE;
        string exeFile = args[2];
        if (exeFile[1] != ':') {
            CreateProcess((currentDir + "\\" + exeFile).c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOA) &si, &pi);
            if (pi.dwProcessId == 0) {
                for (auto path : shell_path) {
                    ZeroMemory(&si, sizeof(si));
                    si.cb = sizeof(si);
                    CreateProcess((path + "\\" + exeFile).c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOA) &si, &pi);
                    if (pi.dwProcessId) break;
                }
            }
        } else {
            CreateProcess(exeFile.c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, (LPSTARTUPINFOA) &si, &pi);
        }
        if (pi.dwProcessId == 0) {
            cout << errorMsg << "File does not exist." << defaultCor << "\n";
            return;
        }

        if (args[1] == "-f") {
            fgProcess = {pi.dwProcessId, exeFile, 0, pi};
            // https://stackoverflow.com/a/1641190/23590989

            SetConsoleCtrlHandler(CtrlHandler, TRUE);
            WaitForSingleObject(pi.hProcess, INFINITE);
            TerminateProcess(pi.hProcess, 0);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        } else if (args[1] == "-b") {
            listProcess.push_back({pi.dwProcessId, exeFile, 0, pi});
        }
    } else {
        cout << errorMsg << "Shell only supports .bat and .exe file extension" << defaultCor << "\n";
    }
}

/*
    Dung mot tien trinh background
*/
void tiny_stop(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "stop <PROCESS_ID>" << defaultCor << "\n";
        return;
    }
    unsigned int process_id = (unsigned int) atoi(args[1].c_str());
    for (auto &c : listProcess) {
        if (c.pi.dwProcessId == process_id) {
            if (c.status == 1) {
                cout << color[2] << "This process is currently stopping" << defaultCor << "\n";
                return;
            }
            c.status = 1;
            SuspendThread(c.pi.hThread);
            cout << color[2] << "Stop process " << process_id << " successfully" << defaultCor << "\n";
            return;
        }
    }
    cout << errorMsg << "Process " << process_id << " does not exist" << defaultCor << "\n";
}

/*
    Tiep tuc mot tien trinh background dang stop
*/
void tiny_resume(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "resume <PROCESS_ID>" << defaultCor << "\n";
        return;
    }
    unsigned int process_id = (unsigned int) atoi(args[1].c_str());
    for (auto &c : listProcess) {
        if (c.pi.dwProcessId == process_id) {
            if (c.status == 0) {
                cout << color[2] << "This process is currently running" << defaultCor << "\n";
                return;
            }
            c.status = 0;
            ResumeThread(c.pi.hThread);
            cout << color[2] << "Resume process " << process_id << " successfully" << defaultCor << "\n";
            return;
        }
    }
    cout << errorMsg << "Process " << process_id << " does not exist" << defaultCor << "\n";
}

/*
    Dung vinh vien mot tien trinh background xoa khoi list
*/
void tiny_kill(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) > 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "kill <PROCESS_ID>" << defaultCor << "\n";
        return;
    }
    unsigned int process_id = (unsigned int) atoi(args[1].c_str());
    for (int i = 0; i < sz(listProcess); ++ i) {
        process &c = listProcess[i];
        if (c.pi.dwProcessId == process_id) {
            TerminateProcess(c.pi.hProcess, 0);
            CloseHandle(c.pi.hProcess);
            CloseHandle(c.pi.hThread);
            cout << color[2] << "Kill process " << process_id << " successfully" << defaultCor << "\n";
            listProcess.erase(listProcess.begin()+i); // delete from vector
            return;
        }
    }
    cout << errorMsg << "Process " << process_id << " does not exist" << defaultCor << "\n";
}

/*
    Hien thi tat ca cac process dang chay ngam
*/
void tiny_list(string inst) {
    cout << color[2];
    cout << "\tID\t\t\tNAME\t\t\tSTATUS\n\n";
    for (int i = listProcess.size() - 1; i >= 0; -- i) {
        int processID = listProcess[i].id;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processID);
        if (hProcess == NULL) {
            listProcess.erase(listProcess.begin()+i);
            continue;
        }
        DWORD exitCode;
        if (!GetExitCodeProcess(hProcess, &exitCode)) {
            listProcess.erase(listProcess.begin()+i);
            CloseHandle(hProcess);
            continue;
        }
        CloseHandle(hProcess);
        if (exitCode == STILL_ACTIVE) continue;
        listProcess.erase(listProcess.begin()+i);
    }
    for (auto c : listProcess) {
        cout << "      " << setw(10) << left << c.id;
        cout << setw(20) << right << getFileName(c.cmdName);
        cout << setw(27) << (c.status == 0 ? "running":"stopped");
        cout << "\n";
    }
    cout << defaultCor;
}

/*
    bien PATH trong shell hien tai
*/
void tiny_path(string inst) {
    cout << color[2];
    cout << "Value of PATH is:\n";
    for (auto path : shell_path) {
        cout << path << "\n";
    }
    cout << defaultCor;
}

/*
    Khong anh huong den system va chi tac dong trong shell, PATH se reset lai ban dau (init_shell_path) khi khoi tao shell moi
*/
void tiny_addpath(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "addpath <NEW_PATH>" << defaultCor << "\n";
        return;
    }
    string new_path = args[1];
    if (!checkDir(new_path)) {
        cout << errorMsg << "Invalid path" << defaultCor << "\n";
        return;
    }
    for (auto c : shell_path) {
        if (c == new_path) {
            cout << color[2] << "Path already exists in PATH variable." << defaultCor << "\n";
            return;
        }
    }
    shell_path.push_back(new_path);
    cout << color[2] << "PATH updated successfully." << defaultCor << "\n";
}

/*
    chuyen directory
*/

void tiny_cd(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "cd <NEW_PATH/SUB_DIR>" << defaultCor << "\n";
        return;
    }
    string new_dir = args[1];
    if (new_dir == ".") {
        return;
    }
    if (new_dir == "..") {
        new_dir = getPreviousPath(currentDir);
        if (!checkDir(new_dir)) {
            cout << errorMsg << "Invalid directory" << defaultCor << "\n";
        } else {
            currentDir = new_dir;
        }
    } else if (new_dir[1] == ':') { // new dir
        if (!checkDir(new_dir)) {
            cout << errorMsg << "Invalid directory" << defaultCor << "\n";
        } else {
            currentDir = new_dir;
        }
    } else { // sub dir
        new_dir = currentDir + "\\" + new_dir;
        if (!checkDir(new_dir) || !SetCurrentDirectory(new_dir.c_str())) {
            cout << errorMsg << "Invalid directory" << defaultCor << "\n";
        } else {
            currentDir = new_dir;
        }
    }
}
void tiny_delete(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 2) {
        cout << errorMsg << "Invalid option" << defaultCor << "\n";
        cout << usageMsg << "delete <PATH_TO_FILE>" << defaultCor << "\n";
        return;
    }
    string filePath = args[1];
    if (filePath[1] != ':') filePath = currentDir +"\\"+ filePath;
    if (DeleteFile(filePath.c_str())) {
        cout << color[2] << "File " << filePath << " deleted successfully" << defaultCor << "\n";
    } else {
        cout << errorMsg << "Unable to delete file " << filePath << defaultCor << "\n";
    }
}

void tiny_move(string inst) {
    vector<string> args = split_space(inst);
    if (sz(args) != 4 || args[2] != ">") {
        cout << errorMsg << "Invalid command format" << defaultCor << "\n";
        cout << usageMsg << "move <SRC> > <DEST>" << defaultCor << "\n";
        return;
    }
    string src = args[1];
    string dest = args[3];
    if (dest == ".") dest = currentDir;
    else if (dest == "..") dest = getPreviousPath(currentDir);
    else if (dest[1] != ':') dest = currentDir + "\\" + dest;
    if (src[1] != ':') src = currentDir + "\\" + src;
    if (!checkDir(dest)) {
        cout << errorMsg << dest << " is not a directory" << defaultCor << "\n";
        return;
    }
    string target = dest + "\\" + getFileName(src);

    if (MoveFileEx(src.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING)) {
        cout << color[2] << "Move " << src << " to " << dest << " successfully" << defaultCor << "\n";
    } else {
        cout << errorMsg << "Unable to move " << src << " to " << dest << defaultCor << "\n";
    }
}

// vong lai
void tiny_echo(string inst){
    vector<string>args = split_space(inst);
    if(sz(args) < 2){
        cout << "echo is on";
        return;
    }else{
        auto it = args.begin() +1;
        for( ; it != args.end() ; it++){
            cout << *it << " " ;
        }
        return;
    }
}

/*
    chay exe co trong PATH
*/
int tiny_run_exe_in_PATH(string inst) {
    vector<string> args = split_space(inst);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    CreateProcess(NULL, const_cast<char *>((currentDir + "\\" + inst).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA) &si, &pi);
    for (auto path : shell_path) {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        CreateProcess(NULL, const_cast<char *>((path + "\\" + inst).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA) &si, &pi);
        if (pi.dwProcessId) break;
    }
    if (pi.dwProcessId == 0) return 0;
    fgProcess = {pi.dwProcessId, args[0], 0, pi};
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    WaitForSingleObject(pi.hProcess, -1);
    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 1;
}
