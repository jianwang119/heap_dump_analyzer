#pragma once

#include <string>
using namespace std;

#include "type_defs.h"
#include "read_utils.h"

enum 
{
	cTagNone = 0,

	cTagType,
	cTagMethod,
	cTagBackTrace,
	cTagGarbageCollect,
	cTagResize,
	cTagMonoObjectNew,
	cTagMonoObjectSizeChange,
	cTagMonoObjectGc,
	cTagHeapSize,
	cTagHeapMemoryStart,
	cTagHeapMemoryEnd,
	cTagHeapMemorySection,
	cTagHeapMemorySectionBlock,
	cTagHeapMemoryRoots,
	cTagHeapMemoryThreads,
	cTagBoehmAlloc,
	cTagBoehmFree,
	cTagMonoVTable,
	cTagMonoClassStatics,
	cTagMonoThreadTableResize,
	cTagMonoThreadStatics,
	cTagBackTraceTypeLink,
	cTagBoehmAllocStacktrace,

	cTagEos = UCHAR_MAX,

	cTagCustomEvent = cTagEos - 1,
	cTagAppResignActive = cTagEos - 2,
	cTagAppBecomeActive = cTagEos - 3,
	cTagNewFrame = cTagEos - 4,
	cTagAppMemoryStats = cTagEos - 5,
};

struct s_total_data
{
	uint32_t gc_count;
	uint32_t type_count;
	uint32_t method_count;
	uint32_t backtrace_count;
	uint32_t resize_count;

	uint64_t frames_count;
	uint64_t object_news_count, object_resizes_count, object_gcs_count;
	uint64_t boehm_news_count, boehm_frees_count;
};

class s_dump_file
{
public:
	FILE* in;
	s_total_data total_data;

	s_dump_file()
	{
		in = NULL;
	}
};

class s_heap_dump_entry
{
public:
	guint8 tag;

	s_heap_dump_entry(guint8 t) : tag(t)
	{}
};

class s_mono_class : public s_heap_dump_entry
{
public:
	enum 
	{
		cIsValueTypeBit,
		cHasReferencesBit,
		cHasStaticReferencesBit,
	};
		
	guint64 pointer;
	string name;
	guint8 flag;
	guint32 size_of;
	guint32 min_aligment;

	s_mono_class(s_dump_file *file) : s_heap_dump_entry(cTagType)
	{
	}
};

inline void read_count(s_dump_file* file, guint64& total_allocated_bytes, guint32& total_allocated_objects, bool read_allocated_totals)
{
	file->total_data.gc_count = read_uint32(file->in);
	file->total_data.type_count = read_uint32(file->in);
	file->total_data.method_count = read_uint32(file->in);
	file->total_data.backtrace_count = read_uint32(file->in);
	file->total_data.resize_count = read_uint32(file->in);

	file->total_data.frames_count = read_uint64(file->in);
	file->total_data.object_news_count = read_uint64(file->in);
	file->total_data.object_resizes_count = read_uint64(file->in);
	file->total_data.object_gcs_count = read_uint64(file->in);
	file->total_data.boehm_news_count = read_uint64(file->in);
	file->total_data.boehm_frees_count = read_uint64(file->in);

	if (read_allocated_totals)
	{
		total_allocated_bytes = read_uint64(file->in);
		total_allocated_objects = read_uint32(file->in);
	}
}

inline s_heap_dump_entry* read_heap_dump_entry(s_dump_file* file)
{
	s_heap_dump_entry* ret = NULL;

	guint8 tag = read_byte(file->in);

	switch (tag)
	{
	case cTagType:
		ret = new s_mono_class(file);
		break;
	default:
		break;
	}

	return ret;
}
