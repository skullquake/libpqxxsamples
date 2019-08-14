CC=g++
SRC=./src/a.cpp
BIN=./bin/a.out
CXXFLAGS=
CSSFLAGS=\
     `pkg-config --cflags libpq`\
     `pkg-config --cflags libpqxx` 
LIBS=\
     `pkg-config --libs libpq`\
     `pkg-config --libs libpqxx`
DBPATH=./db
DBPORT=5432
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
	dbdrop	\
	clean 
run:
	$(BIN)
dbmk:
	initdb \
		$(DBPATH)
dbdrop:
	printf "drop database $(DBNAM);"|\
		psql \
		-h $(DBHOST) \
		-p $(DBPORT) \
		postgres
dbinit:
	printf "create database $(DBNAM);"|\
		psql \
		-h $(DBHOST) \
		-p $(DBPORT) \
		postgres
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
	printf "vacuum full;"|\
		psql \
		-h $(DBHOST) \
		-p $(DBPORT) \
		$(DBNAM)
dbsize:
	printf "\
		select pg_database_size('$(DBNAM)'),pg_size_pretty( pg_database_size('$(DBNAM)'));\
		"|\
		psql \
		-h $(DBHOST) \
		-p $(DBPORT) \
		$(DBNAM)
clean:
	rm -r ./bin
