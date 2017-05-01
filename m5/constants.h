#define SECTOR_SIZE 512
#define DIRECTORY_RECORD_SIZE 32
#define DIRECTORY_FILENAME_SIZE 6
#define DIRECTORY_SECTOR_RECORD_SIZE 26
#define MAXIMUM_FILE_SIZE 13312
#define READSTRING_MAX_BUFFER_SIZE 80
#define PROCESS_TABLE_SIZE 8

typedef struct ProcessTableEntry {
	int isActive;
	int sp;
} ProcessTableEntry;
