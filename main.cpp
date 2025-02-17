#include <stdio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include "Header.h"

namespace offset {
    int health = 0x100;
}

int main(){
	// Get ProcID of the target process
	DWORD procId = GetProcId(L"csgo.exe");

	// Getmodulebaseaddress
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"client.dll");

	// Get Handle of process 
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	// Resolve base address of the pointer chain 
	uintptr_t dynamicPtrBaseAddr = (moduleBase + 0xDEF97C);

    // dynamicptrbaseaddress
	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;
    
    
    // Local player object 

    uintptr_t dwlocalplayer = 0;
    ReadProcessMemory(hProcess, (BYTE*)dynamicPtrBaseAddr, &dwlocalplayer, sizeof(dwlocalplayer), nullptr);
    std::cout << "Local_Player: " << dwlocalplayer << std::endl;

    // health offset value 
    uintptr_t healthAddr = dwlocalplayer + offset::health;

    // Reading memory of health address
    int healthValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
    std::cout << "Current_Health = " << std::dec << healthValue << std::endl;

    // Changing the health value
     int NewhealthAddr = 1337;
     WriteProcessMemory(hProcess, (BYTE*)healthAddr, &NewhealthAddr, sizeof(NewhealthAddr), nullptr);
    
     // Read out again 
    int dash_healthvalue = 0;
    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &dash_healthvalue, sizeof(dash_healthvalue), nullptr);
    std::cout << "New_Health = " << std::dec << dash_healthvalue << std::endl;

	(void)getchar();
	return 0;
}