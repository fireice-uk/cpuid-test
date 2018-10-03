#include <iostream>
#include <string.h>
#include <cpuid.h>
#include <inttypes.h>
#include <stdio.h>

void cpuid(uint32_t eax, int32_t ecx, int32_t val[4])
{
	memset(val, 0, sizeof(int32_t)*4);

#ifdef _WIN32
	__cpuidex(val, eax, ecx);
#else
	__cpuid_count(eax, ecx, val[0], val[1], val[2], val[3]);
#endif
}

/// intel
// family == 0x6  and model number >= 0x3A
// amd
// family == 23 (017h)

// Mask bits between h and l and return the value
// This enables us to put in values exactly like in the manual
// For example EBX[31:22] is get_masked(cpu_info[1], 31, 22)

inline int32_t get_masked(int32_t val, int32_t h, int32_t l)
{
	val &= (0x7FFFFFFF >> (31-(h-l))) << l;
	return val >> l;
}

int main(int argc, char **argv) {
	int32_t cpu_info[4];
	char cpustr[13] = {0};

	cpuid(0, 0, cpu_info);
	memcpy(cpustr, &cpu_info[1], 4);
	memcpy(cpustr+4, &cpu_info[3], 4);
	memcpy(cpustr+8, &cpu_info[2], 4);

	cpuid(1, 0, cpu_info);
	uint32_t model = 0, family = 0;

	family = get_masked(cpu_info[0], 12, 8);
	model = get_masked(cpu_info[0], 8, 4) | get_masked(cpu_info[0], 20, 16) << 4;

	if(strcmp(cpustr, "GenuineIntel") == 0)
	{
		printf("Intel family %u model %u\n", family, model); 
		
		if(model >= 0x3A)
			printf("hit!\n");
	}
	else if(strcmp(cpustr, "AuthenticAMD") == 0)
	{
		if(family == 0xF)
			family += get_masked(cpu_info[0], 28, 20);
	
		printf("AMD family %u model %u\n", family, model); 
		
		if(family >= 0x17)
			printf("hit!\n");
	}
	else
	{
	}
    return 0;
}
