CC=g++
SRC=./src/a.cpp
BIN=./bin/a.out
CXXFLAGS=\
     `pkg-config --cflags libpqxx`\
     `pkg-config --cflags libpq`
LIBS=\
     `pkg-config --libs libpqxx`\
     `pkg-config --libs libpq`
DBPATH=./db
DBPORT=5000
DBHOST=127.0.0.1
DBNAM=test
$(BIN): $(SRC)
	mkdir -p $(@D)
	$(CC)\
		$(CXXFLAGS)\
		$(SRC)\
		$(LIBS)\
		-o $(BIN)
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
		$(DBNAM)
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
