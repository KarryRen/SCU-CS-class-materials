#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

unsigned _stdcall simulator(void *);
unsigned _stdcall inspector(void *);

LPVOID BASE_PTR;
int Actnum=0;
MEMORYSTATUSEX ms;
int DIV = 1024*1024*1024;

int main(int argc,  char* argv[]) {
	unsigned ThreadID[2];
	int i=0;
	_beginthreadex(NULL, 0, simulator, NULL, 0, &ThreadID[0]);
	_beginthreadex(NULL, 0, inspector, NULL, 0, &ThreadID[1]);
	// getchar();
	system("pause");
	return 0;
}

unsigned _stdcall simulator(void *) {
	DWORD OldProtect;
	int randnum;
	printf("Now the simulator procedure has been started.\n");
	srand((unsigned)time(NULL));
	randnum=-1;

	while(1) {
		Sleep(500);
		while(Actnum != 0) {
			Sleep(500);
		}

		randnum=7&rand();
		switch(randnum) {
			case 0:
				if (BASE_PTR=VirtualAlloc(NULL, 1024*32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE)) {
					Actnum=1;
				}
				break;

			case 1:
				if (VirtualFree(BASE_PTR, 1024*32, MEM_DECOMMIT)) {
					Actnum=2;
				}
				break;

			case 2:
				if (VirtualFree(BASE_PTR, 0, MEM_RELEASE)) {
					Actnum=3;
				}
				break;

			case 3:
				if (VirtualProtect(BASE_PTR, 1024*32, PAGE_READONLY, &OldProtect)) {
					Actnum=4;
				}
				break;

			case 4:
				if (VirtualLock(BASE_PTR, 1024*12)) {
					Actnum=5;
				}
				break;

			case 5:
				if (BASE_PTR=VirtualAlloc(NULL, 1024*32, MEM_RESERVE, PAGE_READWRITE)) {
					Actnum=6;
				}
				break;

			default:
				break;
		}

		printf("\n in simulator : Actnum = %d\n", Actnum);
	}
	return 0;
}

unsigned _stdcall inspector(void *) {
	char para1[3000];
	char tempstr[100];

	MEMORYSTATUS Vmeminfo;
	MEMORY_BASIC_INFORMATION inspectorinfo1;

	int QuOut=0;
	int structsize = sizeof(MEMORY_BASIC_INFORMATION);

	printf("Hi,  now inspector begin to work\n");

	while(1) {
		Sleep(1000);
		printf("\n in spector: Actnum = %d\n", Actnum);
		if(Actnum!=0) {
			switch(Actnum) {
				case 1:
					memset(&inspectorinfo1, 0, structsize);
					VirtualQuery((LPVOID)BASE_PTR, &inspectorinfo1, structsize);
					strcpy(para1, "??????????????????????????????\n");
					break;
				case 2:
					memset(&inspectorinfo1, 0, structsize);
					VirtualQuery((LPVOID)BASE_PTR, &inspectorinfo1, structsize);
					strcpy(para1, "????????????????????????\n");
					break;
				case 3:
					memset(&inspectorinfo1, 0, structsize);
					VirtualQuery((LPVOID)BASE_PTR, &inspectorinfo1, structsize);
					strcpy(para1, "??????????????????????????????????????\n");
					break;
				case 4:
					memset(&inspectorinfo1, 0, structsize);
					VirtualQuery((LPVOID)BASE_PTR, &inspectorinfo1, structsize);
					strcpy(para1, "??????????????????????????????????\n");
					break;
				case 5:
					memset(&inspectorinfo1, 0, structsize);
					VirtualQuery((LPVOID)BASE_PTR, &inspectorinfo1, structsize);
					strcpy(para1, "????????????????????????????\n");
					break;
				case 6:
					memset(&inspectorinfo1, 0, structsize);
					VirtualQuery((LPVOID)BASE_PTR, &inspectorinfo1, structsize);
					strcpy(para1, "????????????????????????\n");
					break;
				default:
					break;
			}

			sprintf(tempstr, "????????:0X%x\n", inspectorinfo1.BaseAddress);
			strcat(para1, tempstr);

			sprintf(tempstr, "????????:0X%x\n", inspectorinfo1.RegionSize);
			strcat(para1, tempstr);

			sprintf(tempstr, "????????:0X%x\n", inspectorinfo1.State);
			strcat(para1, tempstr);

			sprintf(tempstr, "??????????????:0X%x\n", inspectorinfo1.AllocationProtect);
			strcat(para1, tempstr);

			sprintf(tempstr, "????????????:0X%x\n", inspectorinfo1.Protect);
			strcat(para1, tempstr);

			strcat(para1, "(????:10000????????????1000??????????2000??????????)\n");
			strcat(para1, "(??????????0??????????1??????????????2??????????4????????;\n10????????");
			strcat(para1, "??;20??????????????;40????????????????);\n********************************\n");

			printf("%s", para1);

			// ??????????????????????????????????????????????
			GlobalMemoryStatusEx(&ms);
			ms.dwLength = sizeof(ms);
			DWORDLONG dwTPSize = (DWORDLONG)ms.ullTotalPhys / DIV;
			DWORD dwTotalPhys = dwTPSize;
			DWORDLONG dwAPSize = (DWORDLONG)ms.ullAvailPhys / DIV;
			DWORD dwAvailPhys = dwAPSize;
			DWORDLONG dwTpaSize = (DWORDLONG)ms.ullTotalPageFile / DIV;
			DWORD dwTotalPageSize = dwTpaSize;
			DWORDLONG dwApaSize = (DWORDLONG)ms.ullAvailPageFile / DIV;
			DWORD dwAvaPageSize = dwApaSize;
			DWORDLONG dwTVSize = (DWORDLONG)ms.ullTotalVirtual / DIV;
			DWORD dwTotalVSize = dwTVSize;
			DWORDLONG dwAVSize = (DWORDLONG)ms.ullAvailVirtual / DIV;
			DWORD dwAvalVSize = dwAVSize;

			printf("dwLength:           %d\n", ms.dwLength);
			printf("??????????????:     %d GB\n", dwTotalPhys);
			printf("????????????????:   %d GB\n", dwAvailPhys);
			printf("??????????????: 	%d GB\n", dwTotalPageSize);
			printf("????????????????: 	%d GB\n", dwAvaPageSize);
			printf("??????????????:  	%d GB\n", dwTotalVSize);
			printf("????????????????:  	%d GB\n", dwAvalVSize);

			Actnum=0;
			Sleep(500);
		}
	}
	return 0;
}


