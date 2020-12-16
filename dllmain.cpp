// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include "vec3.h"

#define F_KEY 0x46 // As long as you press the letter F, the hacke will work    .

using namespace std;

FILE* f;
DWORD WINAPI run(HMODULE hModule)
{
    
#ifdef _DEBUG
    cout << "Debugging" << "\n\r";
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("AC Aimbot!!");
#endif // DEBUG

    static uintptr_t moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe");

    static uintptr_t* LocalPlayerPtr = (uintptr_t*)(moduleBase + 0xE4DBC);
    static uintptr_t* EntityListPtr = (uintptr_t*)(moduleBase + 0xE5F00);
    static int* GameTypePtr = (int*)(moduleBase + 0xE4DB4);
    static int* maxPlayer = (int*)(moduleBase + 0xE4E10);

    static int HealthOffest = 0xF4;
    static int TeamOffest = 0x320;
    static int LocationOffest = 0x4;
    static int AngelOffest = 0x40;

    static int* LocalPlayerTeam = (int*)(*LocalPlayerPtr + TeamOffest);
    static vec3* LocalPlayerLocation = (vec3*)(*LocalPlayerPtr + LocationOffest);
    static vec3* LocalPlayerAngel = (vec3*)(*LocalPlayerPtr + AngelOffest);

    while (true) 
    {
        if (GetAsyncKeyState(VK_DELETE) & 0x80000)
        {
            cout << "Breaked" << "\n\r";
            break;
        }
 
        if (GetAsyncKeyState(F_KEY) & 0x80000) 
        {
            
            //cout << "GameType: " << *GameTypeAddr << "\n\r";
            //cout << "MaxPlayer: " << *maxPlayer << "\n\r";
            if (!*maxPlayer) continue;
            //cout << "EntityListAddr: " << *EntityListAddr << "\n\r";
            if (!*EntityListPtr) continue;
            float closedEntityDist = 9999999;
            int closedEntityIndex = -1;
            for (auto i = 0; i < *maxPlayer - 1; i++) 
            {
                //cout << "Id: " << i << "\n\r";
                uintptr_t entAddr = *(uintptr_t*)(*EntityListPtr + i * 0x4);
                //cout << "Address: " << hex << entAddr << "\n\r";
                if (!entAddr) continue;
                //cout << "Health: " << *(int*)(entAddr + EntityHealthOffest) << "\n\r";
                if (*(int*)(entAddr + HealthOffest) < 0) continue;
                if (*(int*)(*LocalPlayerPtr + HealthOffest) < 0) continue;
                float cDist = vec3::Distance(*(vec3*)(entAddr + LocationOffest), *LocalPlayerLocation);
                if (cDist < closedEntityDist) 
                {

                    //cout << cDist << "\n\r";
                    if(cDist > 80.0f) continue;
                    //cout << *PlayerTeam << " " << *(int*)(entAddr + TeamOffest) << "\n\r";
                    if (*GameTypePtr == 7 && *(int*)(entAddr + TeamOffest) == *LocalPlayerTeam) continue;
                    closedEntityDist = cDist;
                    closedEntityIndex = i;

                }
            }
            if(closedEntityIndex == -1) continue;
            uintptr_t entAddr = *(uintptr_t*)(*EntityListPtr + closedEntityIndex * 0x4);
            vec3 angel = vec3::CalcAngle(*LocalPlayerLocation, *(vec3*)(entAddr + LocationOffest));
            float absDiffX = abs(angel.x - LocalPlayerAngel->x);
            float absDiffY = abs(angel.y - LocalPlayerAngel->y);
            if (absDiffX > 120 || absDiffY > 90) continue;
            
            cout << absDiffX << "\n\r";
            if (absDiffX <= 10.f)
            {
                LocalPlayerAngel->x = angel.x * 0.12f + LocalPlayerAngel->x * 0.88f;
                LocalPlayerAngel->y = angel.y * 0.12f + LocalPlayerAngel->y * 0.88f;
            }
            else if (absDiffX <= 40.f) 
            {
                LocalPlayerAngel->x = angel.x * 0.05f + LocalPlayerAngel->x * 0.95f;
                LocalPlayerAngel->y = angel.y * 0.05f + LocalPlayerAngel->y * 0.95f;
            }
            else if (absDiffX <= 90.f)
            {
                LocalPlayerAngel->x = angel.x * 0.03f + LocalPlayerAngel->x * 0.97f;
                LocalPlayerAngel->y = angel.y * 0.03f + LocalPlayerAngel->y * 0.97f;
            }
            else if (absDiffX <= 120.f)
            {
                LocalPlayerAngel->x = angel.x * 0.02f + LocalPlayerAngel->x * 0.98f;
                LocalPlayerAngel->y = angel.y * 0.02f + LocalPlayerAngel->y * 0.98f;
            }

          //  cout << "Angel Is: " << PlayerAngel->x << " " << PlayerAngel->y << "\n\r";
            Sleep(1);

        }
        
    }
    FreeLibraryAndExitThread(hModule, 0);
}
VOID WINAPI closeAll() {
#ifdef _DEBUG
    /*fclose(f);
    FreeConsole();*/

#endif // DEBUG
}
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    HANDLE handle = NULL;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);    
        handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)run, hModule, NULL, NULL);
        if(handle) CloseHandle(handle);
        break;
    case DLL_PROCESS_DETACH:
        closeAll();
    default:
        break;
    }
    return TRUE;
}

