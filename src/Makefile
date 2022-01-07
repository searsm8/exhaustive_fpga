CC = g++ -std=c++11 -pthread
CFLAGS = -I. -I./includes -DBOOST_NO_CXX11_SCOPED_ENUMS -L/localdisk/mrs171030/local/lib
BOOST = -lboost_system -lboost_filesystem
OBJS = exhaustive.cpp logging.cpp bookshelf.cpp net.cpp cell.cpp node.cpp resource.cpp placer.cpp site.cpp pin.cpp utils.cpp
DEPS = includes/logging.h includes/pin.h includes/cell.h includes/resource.h includes/site.h includes/sitetype.h includes/node.h includes/net.h includes/bookshelf.h includes/placer.h includes/common.h includes/utils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exhaustive: $(OBJS) $(DEPS)
	$(CC) $(OBJS) $(CFLAGS) -o $@ $(BOOST)