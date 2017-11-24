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

# for INGRID
LIBINGRIDPATH	=	/home/philipp/Private/T2K/GUI_indep/lib
LIBINGRID	=	$(LIBINGRIDPATH)/INGRIDEVENTSUMMARY.so $(LIBINGRIDPATH)/IngridHitSummary.so \
			$(LIBINGRIDPATH)/IngridSimHitSummary.so $(LIBINGRIDPATH)/IngridSimVertexSummary.so \
		        $(LIBINGRIDPATH)/IngridSimParticleSummary.so $(LIBINGRIDPATH)/BeamInfoSummary.so \
		        $(LIBINGRIDPATH)/IngridBasicReconSummary.so $(LIBINGRIDPATH)/Ingrid1stReducSummary.so \
		        $(LIBINGRIDPATH)/IngridTrackSummary.so $(LIBINGRIDPATH)/NeutInfoSummary.so \
		        $(LIBINGRIDPATH)/PMReconSummary.so $(LIBINGRIDPATH)/PMAnaSummary.so

INGRIDLDFLAGS	=	-L$(LIBINGRIDPATH) $(LIBINGRID)

LDFLAGS		=	$(INGRIDLDFLAGS) $(LIBGEMLDFLAGS) $(ROOTLDFLAGS)
CFLAGS		=	$(ROOTCFLAGS)
IFLAGS		=	-I$(LIBINGRIDPATH)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) -o $(EXEC) $(OBJECTS) $(ROOTFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@ $(ROOTFLAGS)

clean:
	rm $(EXEC) $(OBJECTS)
