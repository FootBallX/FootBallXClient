#include <stdlib.h>
#include <string.h>
#include "pomelo-private/listener.h"
#include "pomelo-private/jansson-memory.h"

pc_listener_t *pc_listener_new() {
  pc_listener_t *listener = (pc_listener_t *)pc_jsonp_malloc(sizeof(pc_listener_t));
  if(listener == NULL) {
    return NULL;
  }

  if(pc_listener_init(listener)) {
    pc_jsonp_free(listener);
    return NULL;
  }

  return listener;
}

int pc_listener_init(pc_listener_t *listener) {
  memset(listener, 0, sizeof(pc_listener_t));
  ngx_queue_init(&listener->queue);
  return 0;
}

void pc_listener_destroy(pc_listener_t *listener) {
  ngx_queue_remove(&listener->queue);
  pc_jsonp_free(listener);
}