# Compiler options
CC = gcc
CFLAGS = -g -Wall -Wno-unused-function -Wno-unused-but-set-variable -O2
WRAP_MALLOC = -DUSE_MALLOC_WRAPPERS
DFLAGS = -DHAVE_PTHREAD $(WRAP_MALLOC)
BUILD = .
C = $(wildcard $(BUILD)/*.c)
OBJS = $(C:.c=.o)

all: $(OBJS) libbwa.a

print-%: ; @echo $* = $($*)

$(BUILD)/%.o: $(INCLUDE)/*/%.cpp
	$(CC) -c $(CFLAGS) $(DFLAGS) -o $@ $^

libbwa.a: $(OBJS)
		$(AR) -csru $@ $(OBJS)

clean:
	@rm -rf *.o
	@rm -rf libbwa.a
