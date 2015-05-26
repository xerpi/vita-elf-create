TARGET = vita-elf-create
OBJS = vita-elf-create.o elf.o

CFLAGS = -Wall -O2
LIBS =

$(TARGET): $(OBJS)
	gcc $^ $(LIBS) -o $@

%.o: %.c
	gcc $(CFLAGS) -c $? -o $@

clean:
	@rm -rf $(OBJS) $(TARGET)
