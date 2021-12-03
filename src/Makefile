CC = g++
CFLAGS = -I.
OBJS = exhaustive.cpp logging.cpp bookshelf.cpp net.cpp cell.cpp node.cpp resource.cpp placer.cpp site.cpp pin.cpp
DEPS = logging.h pin.h cell.h resource.h site.h sitetype.h node.h net.h bookshelf.h placer.h common.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exhaustive: $(OBJS) $(DEPS)
	$(CC) $(OBJS) $(CFLAGS) -o $@ 