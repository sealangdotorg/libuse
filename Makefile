#   
#   Copyright (c) 2014-2016 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/libstdhl
#   
#   This file is part of libstdhl.
#   
#   libstdhl is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   libstdhl is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with libstdhl. If not, see <http://www.gnu.org/licenses/>.
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

INCLUDE += -I ../
# INCLUDE += -I cpp

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
