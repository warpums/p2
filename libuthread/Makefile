# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o preempt.o

CC := gcc
RR := ar
RFLAGS := rcs
CFLAGS := -Wall -Werror
CFLAGS += -g
PANDOC := pandoc

ifneq ($(V),1)
Q = @
endif

all : $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a : $(objs)
        @echo "CC $@"
        $(Q)$(RR) $(RFLAGS) $(lib) $(objs)

%.o : %.c
        @echo "CC $@"
        $(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
        @echo "clean"
        $(Q)rm -f $(lib) $(objs) $(deps)
