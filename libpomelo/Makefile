CFLAGS = -g -I./include -I./deps/uv/include -I./deps/jansson/src
LDFLAGS = -L./deps/uv -L./deps/jansson/lib -L. -lm -luv -B static -ljansson -lpomelo -Bdynamic -framework CoreServices
TLDFLAGS = -L. -lm -lpomelo -Bdynamic -framework CoreServices
OBJDIR := out

OBJS += src/client.o
OBJS += src/network.o
OBJS += src/package.o
OBJS += src/pkg-handshake.o
OBJS += src/pkg-heartbeat.o
OBJS += src/map.o
OBJS += src/listener.o
OBJS += src/message.o
OBJS += src/msg-json.o
OBJS += src/msg-pb.o
OBJS += src/common.o
OBJS += src/pb-util.o
OBJS += src/pb-encode.o
OBJS += src/pb-decode.o
OBJS += src/transport.o
OBJS += src/thread.o
OBJS += src/protocol.o
#OBJS += jansson/*.o

OBJS := $(addprefix $(OBJDIR)/,$(OBJS))

all: libpomelo.a

$(OBJDIR)/src/%.o: src/%.c include/pomelo.h \
									 include/pomelo-protocol/package.h \
									 include/pomelo-private/map.h \
									 include/pomelo-private/listener.h \
									 include/pomelo-private/common.h \
									 include/pomelo-protobuf/pb.h \
									 include/pomelo-protobuf/pb-util.h\
									 include/pomelo-private/transport.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/jansson/*.o: deps/jansson/lib/libjansson.a
	@mkdir -p $(OBJDIR)/jansson
	@cd $(OBJDIR)/jansson; \
	$(AR) x ../../deps/jansson/lib/libjansson.a

libpomelo.a: $(OBJS)
	$(AR) rcs $@ $^

test: test/test.c libpomelo.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/test test/test.c

connect: test/connect.c libpomelo.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/connect test/connect.c

client-init: test/client-init.c libpomelo.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/client-init test/client-init.c

pkg: test/pkg.c libpomelo.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/pkg test/pkg.c

msg: test/msg.c libpomelo.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/msg test/msg.c

jtest: test/jtest.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/jtest test/jtest.c

thread: test/thread.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/thread test/thread.c

client: test/client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/client test/client.c

reconn: test/reconn.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/reconn test/reconn.c

stop: test/stop.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/stop test/stop.c

map: test/map.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/map test/map.c

hashtest: test/hashtest.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o test/hashtest test/hashtest.c

run:
	./test/connect

clean:
	rm out -rf
	rm libpomelo.a

.PONY: clean all run touch