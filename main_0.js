// https://www.youtube.com/watch?v=d4gHLx3bklc 
//#include "stdafx.h" // не находит
#include <Windows.h> // VOID
//#include <stdio.h> // wprintf()
//#include <malloc.h>
//------------------
#include <iostream>// вывод в консоль 
//using namespace std;//

int countth = 0;

#define COUNT_THREADS 3
//
VOID MultiThread(VOID);
DWORD WINAPI WorkThread_0(LPVOID param);
DWORD WINAPI WorkThread_1(LPVOID param);
DWORD WINAPI WorkThread_2(LPVOID param);

int main()//-----------------------------------------------------------------//
{
	MultiThread();
	/*while(true)
	{
		if (countth == 3) { break; }
	}*/
	//---------
	return 1;//
	system("pause"); // задержка консоли до нажатия любой клавиши
}
VOID MultiThread(VOID)
{
	HANDLE hArr[COUNT_THREADS];
	//HANDLE hArr[3];
	
	hArr[0] = CreateThread(NULL, 0, WorkThread_0, NULL, 0, 0);
	hArr[1] = CreateThread(NULL, 0, WorkThread_1, NULL, 0, 0);
	hArr[2] = CreateThread(NULL, 0, WorkThread_2, NULL, 0, 0);
	// ожидает несколько потоков
	// WARNING! Не INFINITY , а INFINITE.============================
	WaitForMultipleObjects(COUNT_THREADS, hArr, TRUE, INFINITE);
	wprintf(L"--- WaitForMultipleObjects() --- \n");
	// 1 - кол-во потоков.
	// 2 - указатель на массив.
	// 3 - TRUE дождаться всех потоков. FALSE дождаться одного потока.
	// 4 - INFINITE ждать до бесконечности, или в милисекундах.

	// ожидает один поток
	//WaitForSingleObject(hArr[0], INFINITE);
}
// wprintf(L"i = ", i) // выодит всё сообщение
DWORD WINAPI WorkThread_0(LPVOID lpParameter)
{
	for (int i = 0; i < 10; ++i)
	{
		wprintf(L"Message 0: %d\n", i);
	}
	++countth;
	wprintf(L"--- 0 --- \n");
	ExitThread(0);
}
DWORD WINAPI WorkThread_1(LPVOID param)
{
	for (int i = 0; i < 10; ++i)
	{
		wprintf(L"Message 1: %d\n", i);
	}
	++countth;
	wprintf(L"--- 1 --- \n");
	ExitThread(0);
}
DWORD WINAPI WorkThread_2(LPVOID param)
{
	for (int i = 0; i < 10; ++i)
	{
		wprintf(L"Message 2: %d\n", i);
	}
	++countth;
	wprintf(L"--- 2 --- \n");
	ExitThread(0);
}