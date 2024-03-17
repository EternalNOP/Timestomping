#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[]) {
	//Display usage
	if (argc != 4) {
		printf("Usage: timestomp.exe <method> <input file> <output file>\n");
		printf("method\t1 = All available timestamps\n\t2 = CreationTime\n\t3 = AccessTime\n\t4 = LastWriteTime");
		return(0);
	}

	//Open handle to input file
	HANDLE inFile = CreateFileA(argv[2], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (inFile == INVALID_HANDLE_VALUE) {
		printf("[+] Handle to input file could not be opened with error: %d\n", GetLastError());
	}

	//Open a handle to output file
	HANDLE outFile = CreateFileA(argv[3], GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (outFile == INVALID_HANDLE_VALUE) {
		printf("[+] Handle to output file could not be opened: %d\n", GetLastError());
	}

	int method = atoi(argv[1]);
	LPFILETIME creationTime = NULL, lastAccessTime = NULL, lastWriteTime = NULL;
	int returnValue = NULL;

	//Determine method and retreive appropriate file time information
	switch (method)
	{
	case 1:
		printf("[+] Retreiving all file times\n");
		if (GetFileTime(inFile, &creationTime, &lastAccessTime, &lastWriteTime) == 0) {
			printf("Error retreiving file information: %d\n", GetLastError());
			return 1;
		}

		printf("[+] Successfully retreived file times\n");


		printf("[+] Writing all times to output file\n");
		if (SetFileTime(outFile, &creationTime, &lastAccessTime, &lastWriteTime) == 0) {
			printf("[+] Error writing to output file: %d\n", GetLastError());
			return 2;
		}

		break;

	case 2:
		printf("[+] Retreiving file creation time\n");
		if (GetFileTime(inFile, &creationTime, NULL, NULL) == 0) {
			printf("Error retreiving file information: %d\n", GetLastError());
			return 1;
		}

		printf("[+] Successfully retreived file times\n");

		printf("[+] Writing all times to output file\n");
		if (SetFileTime(outFile, &creationTime, NULL, NULL) == 0) {
			printf("[+] Error writing to output file: %d\n", GetLastError());
			return 2;
		}

		break;

	case 3:
		printf("[+] Retreiving file access time\n");
		if (GetFileTime(inFile, NULL, &lastAccessTime, NULL) == 0) {
			printf("[+] Error retreiving file information: %d\n", GetLastError());
			return 1;
		}

		printf("[+] Successfully retreived file times\n");

		printf("[+] Writing all times to output file\n");
		if (SetFileTime(outFile, NULL, &lastAccessTime, NULL) == 0) {
			printf("[+] Error writing to output file: %d\n", GetLastError());
			return 2;
		}

		break;

	case 4:
		printf("[+] Retreiving file write time\n");
		if (GetFileTime(inFile, NULL, NULL, &lastWriteTime) == 0) {
			printf("[+] Error retreiving file information: %d\n", GetLastError());
			return 1;
		}

		printf("[+] Successfully retreived file times\n");

		printf("[+] Writing all times to output file\n");
		if (SetFileTime(outFile, NULL, NULL, &lastWriteTime) == 0) {
			printf("[+] Error writing to output file: %d\n", GetLastError());
			return 2;
		}

		break;

	default:
		printf("[+] Invalid choice\n");
		break;
	}

	//Closing handles
	CloseHandle(inFile);
	CloseHandle(outFile);

	printf("[+] Finished");
	return 0;
}