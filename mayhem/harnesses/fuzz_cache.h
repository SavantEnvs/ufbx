// Shared body of the ufbx_load_geometry_cache() harnesses; the input is served for the root cache file only.
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../../ufbx.h"

#ifndef FUZZ_CACHE_PATH
#error "define FUZZ_CACHE_PATH"
#endif

static const uint8_t *g_data;
static size_t g_size;

static bool open_file(void *user, ufbx_stream *stream, const char *path, size_t path_len, const ufbx_open_file_info *info)
{
	(void)user; (void)info;
	if (path_len != strlen(FUZZ_CACHE_PATH) || memcmp(path, FUZZ_CACHE_PATH, path_len) != 0) return false;
	return ufbx_open_memory(stream, g_data, g_size, NULL, NULL);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	g_data = data;
	g_size = size;

	ufbx_geometry_cache_opts opts = { 0 };
	opts.open_file_cb.fn = &open_file;
	opts.temp_allocator.memory_limit = 0x4000000;   // 64MB
	opts.result_allocator.memory_limit = 0x4000000; // 64MB
	opts.temp_allocator.huge_threshold = 1;
	opts.result_allocator.huge_threshold = 1;

	ufbx_geometry_cache *cache = ufbx_load_geometry_cache(FUZZ_CACHE_PATH, &opts, NULL);
	if (!cache) return -1;
	ufbx_free_geometry_cache(cache);
	return 0;
}
