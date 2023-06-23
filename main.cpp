// https://www.youtube.com/watch?v=d4gHLx3bklc 
// https://youtu.be/RK1FNI9bztQ 
#include <Windows.h> // VOID
#include <iostream>// вывод в консоль 
#include <time.h>// 
using namespace std;//

#define COUNT_THREADS 4
#define ITERATIONS 10

//               структура 
CRITICAL_SECTION section = { 0 }; // критическая секция

VOID NotCriticalSection(VOID);
VOID CriticalSection(VOID);

DWORD WINAPI NoCritSecFunc(LPVOID lpParam);
DWORD WINAPI CritSecFunc(LPVOID lpParam);

// Mutex - объект взаимного исключения.
DWORD iVal = 0;// переменная значения 
HANDLE hMutex = NULL;// дескриптор Mutex-а

VOID Mutex(VOID);
DWORD WINAPI Func_0(LPVOID lpParam);
DWORD WINAPI Func_1(LPVOID lpParam);
DWORD WINAPI Func_2(LPVOID lpParam);
DWORD WINAPI Func_3(LPVOID lpParam);

int main(int argc, char* args[])//--------------------------------------//
{
	srand((unsigned)time(0));
	//NotCriticalSection();
	//CriticalSection();
	Mutex();
	//---------
	return 0;//
	system("pause"); // задержка консоли до нажатия любой клавиши
}
VOID NotCriticalSection(VOID)
{
	HANDLE threads[COUNT_THREADS];// массив 4 потоков 

	for (DWORD i = 0; i < COUNT_THREADS; ++i)
	{
		DWORD *tmp = new DWORD;// для корректного взятия ++i
		*tmp = i;// для корректного взятия ++i          // i 
		threads[i] = CreateThread(NULL, 0, NoCritSecFunc, tmp, 0, 0);
	}
	// ожидает несколько потоков
	// WARNING! Не INFINITY , а INFINITE.============================
	WaitForMultipleObjects(COUNT_THREADS, threads, TRUE, INFINITE);
	// 1 - кол-во потоков.
	// 2 - указатель на массив.
	// 3 - TRUE дождаться всех потоков. FALSE дождаться одного потока.
	// 4 - INFINITE ждать до бесконечности, или в милисекундах.

	for (size_t i = 0; i < COUNT_THREADS; ++i)
	{
		if(threads[i] != INVALID_HANDLE_VALUE)// закрываем потоки 
		{ CloseHandle(threads[i]); }
	}
}
DWORD WINAPI NoCritSecFunc(LPVOID lpParam)
{
	for (DWORD i = 0; i < ITERATIONS; ++i)
	{
		wcout << L"Thread " << *(DWORD*)lpParam << L" : " << i << endl;
		Sleep(rand() % 16 + 10);
	}
	ExitThread(0);
}
//--------------------------------------------------------------------------------
// синхронизирует потоки в нутри одного процесса !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
VOID CriticalSection(VOID)// Критическая секция 
{
	HANDLE threads[COUNT_THREADS];// массив 4 потоков 

	InitializeCriticalSection(&section);// Initialize !!!!!!!!!!!!!!!!!!!!!

	for (DWORD i = 0; i < COUNT_THREADS; ++i)
	{
		DWORD* tmp = new DWORD;// для корректного взятия ++i
		*tmp = i;// для корректного взятия ++i          // i 
		threads[i] = CreateThread(NULL, 0, CritSecFunc, tmp, 0, 0);
	}
	// ожидает несколько потоков
	// WARNING! Не INFINITY , а INFINITE.============================
	WaitForMultipleObjects(COUNT_THREADS, threads, TRUE, INFINITE);
	// 1 - кол-во потоков.
	// 2 - указатель на массив.
	// 3 - TRUE дождаться всех потоков. FALSE дождаться одного потока.
	// 4 - INFINITE ждать до бесконечности, или в милисекундах.

	DeleteCriticalSection(&section);// Delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (size_t i = 0; i < COUNT_THREADS; ++i)
	{
		if (threads[i] != INVALID_HANDLE_VALUE)// закрываем потоки 
		{ CloseHandle(threads[i]); }
	}
}
DWORD WINAPI CritSecFunc(LPVOID lpParam)// Критическая секция 
{
	for (DWORD i = 0; i < ITERATIONS; ++i)
	{
		EnterCriticalSection(&section);// заблокировать другие потоки

		wcout << L"Thread " << *(DWORD*)lpParam << L" : " << i << endl;

		LeaveCriticalSection(&section);// разблокировать другие потоки

		Sleep(rand() % 16 + 10);
	}
	ExitThread(0);
}
//--------------------------------------------------------------------------------
// синхронизирует потоки в нутри нескольких процессов !!!!!!!!!!!!!!!!!!!!!!!!!!!!
VOID Mutex(VOID)// Каждый поток выполнил iVal++;
{
	//     структура 
	HANDLE globalThreads[COUNT_THREADS] = { 0 };// массив 4 потоков 

	hMutex = CreateMutex(NULL, FALSE, L"Same name");
	// 1 - указатель на структуру безопасности
	// 2 - FALSE ждущий. TRUE активный
	// 3 - имя Mutex-а против множественного открытия 
	globalThreads[0] = CreateThread(NULL, 0, Func_0, NULL, NULL, 0);
	globalThreads[1] = CreateThread(NULL, 0, Func_1, NULL, NULL, 0);
	globalThreads[2] = CreateThread(NULL, 0, Func_2, NULL, NULL, 0);
	globalThreads[3] = CreateThread(NULL, 0, Func_3, NULL, NULL, 0);
	// ожидает несколько потоков
	// WARNING! Не INFINITY , а INFINITE.============================
	WaitForMultipleObjects(COUNT_THREADS, globalThreads, TRUE, INFINITE);
	// 1 - кол-во потоков.
	// 2 - указатель на массив.
	// 3 - TRUE дождаться всех потоков. FALSE дождаться одного потока.
	// 4 - INFINITE ждать до бесконечности, или в милисекундах.

	// закрываем потоки 
	if(globalThreads[0] != INVALID_HANDLE_VALUE)
	{ CloseHandle(globalThreads[0]); }
	if (globalThreads[1] != INVALID_HANDLE_VALUE)
	{ CloseHandle(globalThreads[1]); }
	if (globalThreads[2] != INVALID_HANDLE_VALUE)
	{ CloseHandle(globalThreads[2]); }
	if (globalThreads[3] != INVALID_HANDLE_VALUE)
	{ CloseHandle(globalThreads[3]); }
}
DWORD WINAPI Func_0(LPVOID lpParam)
{
	while (iVal < ITERATIONS)// while (iVal < ITERATIONS)
	{// ожидаем разморозки Mutex. Другие потоки заморожены.
		WaitForSingleObject(hMutex, INFINITE);
		wcout << L"Func0: " << iVal << endl;
		iVal++;
		// Другие потоки разморожены.
		ReleaseMutex(hMutex);
	}
	ExitThread(0);
}
DWORD WINAPI Func_1(LPVOID lpParam)
{
	while (iVal < ITERATIONS)// while (iVal < ITERATIONS)
	{// ожидаем разморозки Mutex. Другие потоки заморожены.
		WaitForSingleObject(hMutex, INFINITE);
		wcout << L"Func1: " << iVal << endl;
		iVal++;
		// Другие потоки разморожены.
		ReleaseMutex(hMutex);
	}
	ExitThread(0);
}
DWORD WINAPI Func_2(LPVOID lpParam)
{
	while (iVal < ITERATIONS)// while (iVal < ITERATIONS)
	{// ожидаем разморозки Mutex. Другие потоки заморожены.
		WaitForSingleObject(hMutex, INFINITE);
		wcout << L"Func2: " << iVal << endl;
		iVal++;
		// Другие потоки разморожены.
		ReleaseMutex(hMutex);
	}
	ExitThread(0);
}
DWORD WINAPI Func_3(LPVOID lpParam)
{
	while (iVal < ITERATIONS)// while (iVal < ITERATIONS)
	{// ожидаем разморозки Mutex. Другие потоки заморожены.
		WaitForSingleObject(hMutex, INFINITE);
		wcout << L"Func3: " << iVal << endl;
		iVal++;
		// Другие потоки разморожены.
		ReleaseMutex(hMutex);
	}
	ExitThread(0);
}