// Differential fuzz of ufbx's inflate against zlib (same oracle as upstream misc/fuzz_deflate_persist.c).
#define UFBX_DEV
#include "../../ufbx.c"

#include <zlib.h>

static char g_dst[65536];

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	ufbx_inflate_retain retain;
	retain.initialized = false;

	ufbx_inflate_input input = { 0 };
	input.data = data;
	input.data_size = size;
	input.total_size = size;
	ptrdiff_t result = ufbx_inflate(g_dst, sizeof(g_dst), &input, &retain);
	if (result < 0) result = -1;

	uLongf zlib_len = sizeof(g_dst);
	int ret = uncompress((Bytef*)g_dst, &zlib_len, (const Bytef*)data, (uLong)size);
	if (ret == Z_OK) {
		if (result != (ptrdiff_t)zlib_len) __builtin_trap();
	} else {
		if (result >= 0) __builtin_trap();
		return -1;
	}
	return 0;
}
