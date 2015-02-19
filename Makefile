
#
# Copyright (c) 2015 Philipp Paulweber
# 
# This file is part of the 'libstdhl' project which is released under a NCSA
# open source software license. For more information, see the LICENSE.txt 
# file in the project root directory.
#

AR=ar

CC=clang
CCFLAG += -std=c11
CCFLAG += -g
CCFLAG += -Wall

CPP=clang
CPPFLAG += -std=c++11
CPPFLAG += -g
CPPFLAG += -Wall
#CPPFLAG += -Wextra

TARGET += libstdhlc.a
TARGET += libstdhlcpp.a

COBJECTS += obj/args.o

CPPOBJECTS += obj/Args.o

INCLUDE += -I c
INCLUDE += -I cpp

default: obj $(TARGET)

obj:
	mkdir -p obj

obj/%.o: c/%.c
	@echo "CC  " $<
	@$(CC) $(CCFLAG) $(INCLUDE) -c $< -o $@

obj/%.o: cpp/%.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

libstdhlc.a: $(COBJECTS)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
	@ranlib $@

libstdhlcpp.a: $(CPPOBJECTS)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
	@ranlib $@

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " $(TARGET)
	@rm -f $(TARGET)
