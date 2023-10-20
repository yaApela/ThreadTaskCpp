#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>
#include <vector>
#include <chrono>
using namespace std;

mutex mtx;
string cmd;
STARTUPINFO si;
PROCESS_INFORMATION pi;
wchar_t wcmd_current[256] = L"";

void getData(string proccessName[3])
{
    system("cls");
    for (int i = 0; i < 3; i++)
    {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        cmd = "sc query " + proccessName[i];
        MultiByteToWideChar(CP_ACP, 0, cmd.c_str(), -1, wcmd_current, 256);
        CreateProcess(NULL, wcmd_current, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    this_thread::sleep_for(chrono::seconds(1));
    getData(proccessName);
}

void openServices(string proccessName)
{
    mtx.lock();

    cout << this_thread::get_id() << endl;
    proccessName = "net start " + proccessName;
    system(proccessName.c_str());
    
    mtx.unlock();
};

int main()
{
    setlocale(0,"");
    string proccessName[3] = { "aspnet_state","Schedule" ,"RasAuto" };
    thread t1(openServices, proccessName[0]);
    thread t2(openServices, proccessName[1]);
    thread t3(openServices, proccessName[2]);
    t1.join();
    t2.join();
    t3.join();

    getData(proccessName);    
    return 0;
}
