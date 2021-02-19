#include <stdio.h>
#include <Windows.h>
#include <winbase.h>
#include <stdint.h>
#include <strsafe.h>
#include <iostream>

/* Written by Charles T.W. Truscott */

int main(void) {
	HANDLE AccessDisk = CreateFileW(L"\\\\.\\D:", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (AccessDisk == INVALID_HANDLE_VALUE) {
		printf("Cannot access disk, quitting\n");
		exit(1);
	}

/*	PLARGE_INTEGER disk_size;
	BOOL find_size = GetFileSizeEx(AccessDisk, disk_size);
	printf("Disk is %I64u bytes\n", disk_size);
*/
	int sectors_to_scan;
	printf("Scan how many sectors? (an integer value)\n, Enter now:\t");
	scanf("%d", sectors_to_scan);
	int limiting_value;

	for(limiting_value = 0; limiting_value <= sectors_to_scan; ++limiting_value){
		LARGE_INTEGER position = { limiting_value * 512 };
		BOOL get_file_pointer = SetFilePointerEx(AccessDisk, position, NULL, FILE_BEGIN);
		printf("\nScanning sector %I64u \n\n", position);
		BYTE read_buffer[512];
		DWORD read;
		printf("\n############################## BYTES CONTAINED ##############################\n");
		BOOL read_disk = ReadFile(AccessDisk, read_buffer, 512, &read, NULL);
		int read_limiting_value;
		for(read_limiting_value = 0; read_limiting_value < 512; ++read_limiting_value) {
			printf("%x", read_buffer[read_limiting_value]);
		}
		BYTE zero_buffer[513];
		memset(zero_buffer, 0, 513);
		printf("\n############################ ERASING 512KB SECTOR ############################\n");
		BOOL write_to_disk = WriteFile(AccessDisk, zero_buffer, 512, NULL, NULL);
		if (!write_to_disk) {
			printf("Writing to sector %I64u failed\n", position);
			printf("%d\n", GetLastError());
		}
		printf("\n############################## BYTES CONTAINED ##############################\n");
		BYTE new_read_buffer[512];
		DWORD read_again;
		BOOL read_disk_again = ReadFile(AccessDisk, new_read_buffer, 512, &read_again, NULL);
		int read_limiting_value_again;
		for(read_limiting_value_again = 0; read_limiting_value_again < 512; ++read_limiting_value_again) {
			printf("%x", new_read_buffer[read_limiting_value_again]);
/*			if (new_read_buffer[read_limiting_value_again] != 0) {
				printf("\n\nCYCLE CONTAMINATED, FOUND NON-ZERO BYTE\n");
				exit(1);
			}*/

		}
		Sleep(3);
	
			
	}
	CloseHandle(AccessDisk);
	return 0;
}