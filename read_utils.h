#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <string>
using namespace std;

#include "type_defs.h"

inline guint8 read_byte(FILE *in)
{
	guint8 x;
	fread(&x, 1, sizeof(x), in);
	return x;
}

template<typename TUInt>
TUInt read_vuint(FILE *in)
{
	TUInt x = 0;

	int shift = 0;
	guint8 y;

	do
	{
		y = read_byte(in);
		TUInt b = (TUInt)(y & 0x7f);
		b = b << shift;
		x = x | b;
		shift += 7;
		if ((y & 0x80) == 0)
		{
			break;
		}
	} while (true);

	return x;
}

inline gint16 read_int16(FILE *in)
{
	gint16 x;
	fread(&x, sizeof(x), 1, in);
	return x;
}


inline guint16 read_uint16(FILE *in)
{
	guint16 x;
	fread(&x, sizeof (x), 1, in);
	return x;
}

inline gint32 read_int32(FILE *in)
{
	gint32 x;
	fread(&x, sizeof (x), 1, in);
	return x;
}

inline guint32 read_uint32(FILE *in)
{
	guint32 x;
	fread(&x, sizeof (x), 1, in);
	return x;
}

inline gint64 read_int64(FILE *in)
{
	gint64 x;
	fwrite(&x, sizeof (x), 1, in);
	return x;
}

inline guint64 read_uint64(FILE *in)
{
	guint64 x;
	fwrite(&x, sizeof (x), 1, in);
	return x;
}

inline guint64 read_pointer(FILE *in)
{
	guint64 x = read_vuint<guint64>(in);
	return x;
}

inline uint64_t read_time(FILE *in)
{
	uint64_t x = read_vuint<guint64>(in);
	return x;
}

inline uint64_t read_time_offset(FILE *in)
{
	uint64_t x = read_vuint<guint64>(in);
	return x;
}

inline string read_string(FILE *in)
{
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	char* x = &buf[0];

	size_t len = read_vuint<size_t>(in);
	if (len >= 1024)
	{
		x = new char[len + 1];
		memset(x, 0, sizeof(char)* (len + 1));
	}
	fread((void*)x, sizeof(char), len, in);
	string ret(x);
	if (len >= 1024)
	{
		delete[] x;
	}
	return ret;
}

inline string read_unixtime(uint64_t timestamp)
{
	time_t tt = timestamp / 1000;
	struct tm tm;
	localtime_s(&tm, &tt);
	char buf[64];
	strftime(buf, 64, "%Y-%m-%d_%H;%M;%S", &tm);
	return string(buf);
}