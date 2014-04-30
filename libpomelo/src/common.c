#include <stdlib.h>
#include "uv.h"
#include "pomelo-private/jansson-memory.h"

void pc__handle_close_cb(uv_handle_t* handle) {
  pc_jsonp_free(handle);
}