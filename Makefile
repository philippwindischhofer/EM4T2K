EXEC		=	main
CC		= 	g++-4.9
SOURCES		=	$(wildcard *.cpp)
OBJECTS		=	$(patsubst src%,obj%, $(patsubst %.cpp,%.o, $(SOURCES)))

# for libgem
LIBGEMPATH	=	/home/philipp/Private/T2K/libgem/
LIBGEMLDFLAGS	=	-L$(LIBGEMPATH) -lgem

# for ROOT
ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLDFLAGS	=	$(shell root-config --libs)

LDFLAGS		=	$(LIBGEMLDFLAGS) $(ROOTLDFLAGS)
CFLAGS		=	$(ROOTCFLAGS)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS) $(ROOTFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(ROOTFLAGS) $(LDFLAGS)

clean:
	rm $(EXEC) $(OBJECTS)
