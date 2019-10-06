CC=g++
SRC=./src/a.cpp
DBPATH=./db
DBPORT=5000
DBHOST=127.0.0.1
DBNAM=test

BUILDDIR=bin
SOURCEDIR=./src
HEADERDIR=./include
SOURCES:=$(shell find $(SOURCEDIR) -name '*.cpp')
HEADERS:=$(shell find $(HEADERDIR) -name '*.h')
MACHINE=$(shell $(CXX) -dumpmachine)
ifneq (,$(findstring mingw,$(MACHINE)))
	BINEXT=exe
else
	BINEXT=out
endif
BIN=./bin/a.$(BINEXT)
OBJECTS=$(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))
CXXFLAGS=\
	-I$(HEADERDIR)\
	`pkg-config --cflags libpqxx`\
	`pkg-config --cflags libpq`
LDFLAGS=\
	`pkg-config --libs libpqxx`\
	`pkg-config --libs libpq`

BIN=./bin/a.$(BINEXT)
all: $(BIN)
$(BIN): $(OBJECTS)
	@mkdir -p $(@D)
	@printf "generating $@...\n"
	$(CXX)\
		$(CXXFLAGS)\
		$(OBJECTS)\
		$(LDFLAGS)\
		-o $(BIN)
$(BUILDDIR)/%.o: %.cpp $(HEADERS)
	@printf "generating $@...\n"
	@mkdir -p $(@D)
	$(CXX)\
		$(CXXFLAGS)\
		-I$(HEADERDIR)\
		-I$(dir $<)\
		-c $<\
		$(LDFLAGS)\
		-o $@


.phony:		\
	run	\
	dbrm	\
	dbmk	\
	dbinit	\
	dbstart	\
	dbstop	\
	dbpsql	\
	dbsize	\
	dbls	\
	dbdrop	\
	clean 
run: $(BIN)
	$(BIN)
dbmk:
	initdb \
		$(DBPATH)
dbrm:
	rm \
		-r $(DBPATH)
dbstart:
	pg_ctl \
		-D $(DBPATH) \
		-o "-p $(DBPORT)" \
		start
dbstop:
	pg_ctl \
		-D $(DBPATH) \
		-o "-p $(DBPORT)" \
		stop 
dbpsql:
	psql \
		-h $(DBHOST) \
		-p $(DBPORT) \
		-d postgres 
#		$(DBNAM)
dbvacuum:
	psql \
		-P pager=off\
		-h $(DBHOST) \
		-p $(DBPORT) \
		-d $(DBNAM)\
		-c "vacuum full;"
dbsize:
	psql \
		-P pager=off\
		-h $(DBHOST)\
		-p $(DBPORT)\
		-d $(DBNAM)\
		-c "select pg_database_size('$(DBNAM)'),pg_size_pretty( pg_database_size('$(DBNAM)'));"
dbls:
	psql \
		-P pager=off\
		-h $(DBHOST) \
		-p $(DBPORT) \
		-d postgres \
		-c "SELECT * FROM pg_catalog.pg_tables;"

dbdrop:
	psql \
		-P pager=off\
		-h $(DBHOST) \
		-p $(DBPORT) \
		-d postgres \
		-c "drop database $(DBNAM);"
dbinit:
	psql \
		-P pager=off\
		-h $(DBHOST) \
		-p $(DBPORT) \
		-d postgres\
		-c "create database $(DBNAM);"
clean:
	rm -r ./bin
