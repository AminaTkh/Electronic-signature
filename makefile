CC = g++
CFLAGS = -Iinc -Wall -Werror -pedantic-errors -Wextra -std=c++11 -ftrapv -O2
OBJECTS_HASH =  functions.o 
OBJECTS = arith.o help.o rand.o hash.o 
EXEC1 = main.out
PRIV_KEY = genp.out
PUB_KEY = conv.out
SIGN = sign.out
VERIFY = verify.out
OBJECTS_PRIV_KEY = arith.o genp.o rand.o 
OBJECTS_PUB_KEY = arith.o conv.o rand.o 

OBJECTS_SIGN = arith.o rand.o sign.o 
OBJECTS_VERIFY = arith.o rand.o verify.o 

all : $(PRIV_KEY) $(PUB_KEY) $(SIGN) $(VERIFY)

$(PRIV_KEY) : $(OBJECTS_PRIV_KEY)
	$(CC) $(OBJECTS_PRIV_KEY) -o $(PRIV_KEY)

$(PUB_KEY) : $(OBJECTS_PUB_KEY)
	$(CC) $(OBJECTS_PUB_KEY) -o $(PUB_KEY)

$(SIGN) : $(OBJECTS_SIGN) 
	$(CC) $(OBJECTS_SIGN) -o $(SIGN)

$(VERIFY) : $(OBJECTS_VERIFY) 
	$(CC) $(OBJECTS_VERIFY) -o $(VERIFY)

arith.o : arith.cpp arith.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

help.o :  help.hpp arith.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

rand.o : rand.c
	gcc $< -Iinc -c -o $@

hash.o :  hash.hpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

genp.o : genp.cpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

conv.o : conv.cpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@


sign.o : sign.cpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

verify.o : verify.cpp $(BLOCK)
	$(CC) $< $(CFLAGS) -c -o $@

clean :
	rm -f *.out *.o *.txt *.pub *.prv *.crt
	rm -rf *.dSYM
.PHONY : clean
