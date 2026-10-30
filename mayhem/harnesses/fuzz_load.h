// Shared body of the ufbx_load_memory() harnesses (fbx / obj / mtl).
#include <stdint.h>
#include <stddef.h>

#include "../../ufbx.h"
#include "../../test/check_scene.h"

#ifndef FUZZ_FILE_FORMAT
#error "define FUZZ_FILE_FORMAT"
#endif

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	ufbx_load_opts opts = { 0 };
	opts.file_format = FUZZ_FILE_FORMAT;
	opts.temp_allocator.memory_limit = 0x4000000;   // 64MB
	opts.result_allocator.memory_limit = 0x4000000; // 64MB
	// Every allocation gets its own block so ASan sees out-of-bounds accesses inside ufbx's arenas.
	opts.temp_allocator.huge_threshold = 1;
	opts.result_allocator.huge_threshold = 1;

	ufbx_scene *scene = ufbx_load_memory(data, size, &opts, NULL);
	if (!scene) return -1;
	ufbxt_check_scene(scene);
	ufbx_free_scene(scene);
	return 0;
}
