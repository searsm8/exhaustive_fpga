CC = g++
CFLAGS = -I.
OBJS = exhaustive.cpp logging.cpp bookshelf_io.cpp net.cpp cell.cpp node.cpp resource.cpp placer.cpp site.cpp pin.cpp
DEPS = logging.h pin.h cell.h resource.h site.h node.h net.h bookshelf_io.h placer.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exhaustive: exhaustive.cpp
	$(CC) $(OBJS) $(CFLAGS) -o $@ 