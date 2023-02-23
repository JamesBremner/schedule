#source file search paths
VPATH = ./gui ../raven-set ../sqlite


# compiler include search paths
INCS = -I./include \
	-I../windex/include \
	-I../raven-set \
	-I../../code_ext/boost/boost1_78

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \
	-L../../code_ext/boost/boost1_78/lib \
		-lboost_system-mgw11-mt-x64-1_78 \
		-lboost_program_options-mgw11-mt-x64-1_78 \
		# -lboost_thread-mgw11-mt-x64-1_7 \
	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static -lComctl32

# folders
ODIR = ./.vscode/obj

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_schedule = \
	$(ODIR)/FleetCrewGUI.o \
	$(ODIR)/cFleet.o \
	$(ODIR)/raven_sqlite.o $(ODIR)/sqlite3.o

### COMPILE

$(ODIR)/sqlite3.o: sqlite3.c
	gcc	 -c -o $@ $<

$(ODIR)/%.o: %.cpp  JobResource.h
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

starter: $(OBJ_schedule)
	g++ $(OPT) \
	-o bin/schedule.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

