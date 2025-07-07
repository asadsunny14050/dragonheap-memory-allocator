CC = gcc 
CFLAGS = -Wall -Wextra -g

SRCS = src/dragonheap.c
LIB_OBJ = dragonheap.o
STATIC_LIB = libdragonheap.a

TARGET = use_dragonheap

all: example.c $(STATIC_LIB)
	$(CC) $(CFLAGS) example.c -Isrc -L. -ldragonheap -o $(TARGET) 

lib: $(STATIC_LIB)

$(STATIC_LIB): $(LIB_OBJ)
	ar rcs $@ $<

$(LIB_OBJ): $(SRCS)
	$(CC) $(CFLAGS) -c $< -Isrc -o $@

clean: 
	rm -f $(LIB_OBJ) $(STATIC_LIB) $(TARGET)


