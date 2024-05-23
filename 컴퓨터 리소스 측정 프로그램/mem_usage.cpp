#include <Windows.h>
#include <stdio.h>

void try_mem_usage() {
	MEMORYSTATUSEX mem_usage;
	GlobalMemoryStatusEx(&mem_usage);
	printf("MEM_USAGE: %ld%%\n", mem_usage.dwMemoryLoad);
}

int main() {
	while (1) {
		Sleep(1000);
		try_mem_usage();
	}
}


