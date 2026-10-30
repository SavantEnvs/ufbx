// ufbx's own correctly-rounded strtod over an exactly-sized heap buffer (ASan catches over-reads).
#define UFBX_DEV
#include "../../ufbx.c"

#include <stdlib.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	char *value = (char*)malloc(size ? size : 1);
	if (!value) return -1;
	if (size) memcpy(value, data, size);
	char *end = NULL;
	volatile double d = ufbxi_parse_double(value, size, &end, 0);
	(void)d;
	free(value);
	return 0;
}
