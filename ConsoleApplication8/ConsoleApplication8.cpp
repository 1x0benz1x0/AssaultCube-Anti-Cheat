#include <iostream>
#include <print>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
int main()
{
	
	DWORD pid = 0;
	HWND hwnd = FindWindowA(NULL, "AssaultCube");

	if (hwnd == NULL) {
		std::println("AssaultCube Is not Open Pls open it ");
		return 1;
	}

	GetWindowThreadProcessId(hwnd, &pid);
	std::println("AssaultCube Was Found");
	std::println("{}", pid);

	HANDLE hproc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
	if (hproc == NULL) {
		std::println("Failed To Open");
		return 1;
	}

	uintptr_t baseAddress = 0;
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	MODULEENTRY32 Entry;
	Entry.dwSize = sizeof(Entry);
	if (Module32First(hsnapshot, &Entry)) {
		baseAddress = (uintptr_t)Entry.modBaseAddr;
	}
	CloseHandle(hsnapshot);

	if (baseAddress == 0) {
		std::cout << "Could not find module base address." << std::endl;
		CloseHandle(hproc);
		return 1;
	}
	std::cout << "Module base: 0x" << std::hex << baseAddress << std::dec << std::endl;

	uintptr_t AmmoAddress;
	ReadProcessMemory(hproc, (LPCVOID)(baseAddress + 0x0017E0A8), &AmmoAddress, sizeof(AmmoAddress), NULL);
	AmmoAddress += 0x140;
	//---------------------------------------------------------------------------------------------------

	uintptr_t PistolAmmoAddress;
	ReadProcessMemory(hproc, (LPCVOID)(baseAddress + 0x0017E0A8), &PistolAmmoAddress, sizeof(PistolAmmoAddress), NULL);
	PistolAmmoAddress += 0x12C;



		while (true) {
			int value = 0;
			SIZE_T bytesRead = 0;
			BOOL s = ReadProcessMemory(hproc, (LPCVOID)AmmoAddress, &value, sizeof(value), &bytesRead);
			std::cout << "Value : " << value << " (" << bytesRead << " bytes read)" << std::endl;
			if (value > 20) {
				std::println("Cheater");
				std::this_thread::sleep_for(std::chrono::seconds(2));
				system("cls");
			}
			else if (value <= 20) {
				std::println("Not A Cheater");
				std::this_thread::sleep_for(std::chrono::seconds(2));
				system("cls");
			}
			int Pistolvalue = 0;
			bytesRead = 0;
			BOOL as = ReadProcessMemory(hproc, (LPCVOID)PistolAmmoAddress, &Pistolvalue, sizeof(Pistolvalue), &bytesRead);
			if (as) {

				std::cout << "Pistol Value : " << Pistolvalue << " (" << bytesRead << " bytes read)" << std::endl;
				if (Pistolvalue > 10) {
					std::println("Cheater");
					std::this_thread::sleep_for(std::chrono::seconds(2));
					system("cls");
					
				}
				else if (Pistolvalue <= 10) {
					std::println("Not A Cheater");
					system("cls");
					
				}
			}
			else {
				std::cout << "ReadProcessMemory failed. Error: " << GetLastError() << std::endl;
			}
		}
	
	


}
	

