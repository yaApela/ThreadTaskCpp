#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>
#include <vector>
#include <chrono>
using namespace std;

mutex mtx;
mutex mtx2;

void getStatus(bool a)
{
    mtx2.lock();
    if (a)
        cout << "\t Id:" << this_thread::get_id() << " Start proccess \n";
    else
        cout << "\t Id:" << this_thread::get_id() << " End proccess \n";
    mtx2.unlock();
}

void commonResource()
{
    mtx.lock();
    this_thread::sleep_for(chrono::milliseconds(3000)); // что-то долго и упорно делает
    mtx.unlock();
}

int firstProcess(int a,int b)
{
    this_thread::sleep_for(chrono::milliseconds(2000)); // имитация запуска процесса 1
    getStatus(true); // вывод данных о запуске
    a = a + b; // предположим тут выполнение каких то сложных математических операций
    commonResource(); // предположим тут выполняется какая-то общая задача для каждого процесса (это может быть например конвертация данных в нужный формат или запись в БД)
    getStatus(false); // вывод данных об окончании
    return a;
}

int secondProcess(int a, int b)
{
    this_thread::sleep_for(chrono::milliseconds(2000)); // имитация запуска процесса 1
    getStatus(true);
    a = a - b;
    commonResource();
    getStatus(false);
    return a;
}

int thirdProcess(int a, int b)
{
    this_thread::sleep_for(chrono::milliseconds(2000)); // имитация запуска процесса 1
    getStatus(true);
    a = a * b;
    commonResource();
    getStatus(false);
    return a;
}

int main()
{
    setlocale(0,"");

    // Запуская в многопоточном режиме происходит экономия 6-ти секунд на выполнение запуска процессов, а так же n-го количества секунд на расчеты
    int sum = 0;
    int diff = 0;
    int mult = 0;

    thread thread1([&sum]() {
        sum = firstProcess(2,5);
        });
    thread thread2([&diff]() {
        diff = secondProcess(2, 5);
        });
    thread thread3([&mult]() {
        mult = thirdProcess(2, 5);
        });

    for (int i = 0; i < 15; i++)
    {
        cout << "main thread is working" << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
       
    thread1.join();
    thread2.join();
    thread3.join();

    cout << "=======================" << endl;
    cout << "сумма: " << sum << endl;
    cout << "разность: " << diff << endl;
    cout << "произведение: " << mult << endl;

    return 0;
}
