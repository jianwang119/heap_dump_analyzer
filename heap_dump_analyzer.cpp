#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <string>
using namespace std;

#include "dump_defs.h"

//cFileSignature = 0x4EABB055,
//cFileVersion = 3,



int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: heap_dump_analyzer logfile.");
		return -1;
	}

	s_dump_file dump_file;

	fopen_s(&dump_file.in, argv[1], "rb");

	guint32 magic = read_uint32(dump_file.in);
	gint32 file_version = read_int32(dump_file.in);
	string file_label = read_string(dump_file.in);
	uint64_t timestamp = read_time(dump_file.in);
	guint8 pointer_size = read_byte(dump_file.in);
	guint8 platform_id = read_byte(dump_file.in);

	string str_timestamp = read_unixtime(timestamp);

	fclose(dump_file.in);

	return 0;
}

