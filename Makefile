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

.PHONY:

default: debug

help:
	@echo "TODO"


REPO = libstdhl

TARGET  = $(REPO)c.a
TARGET += $(REPO)cpp.a

TEST_TARGET = test-$(REPO)


CP  = $(shell find cpp -name '*.cpp' | cut -d'.' -f1)
CO  = $(CP:%=obj/%.o)

XP  = $(shell find c -name '*.c' | cut -d'.' -f1)
XO  = $(XP:%=obj/%.o)

CI  = -I ./

CL  =

CC  =
CF  =

LIB = ../..
GT  = lib/gtest

ifeq ("$(wildcard $(LIB)/$(GT))","")
LIB = .
endif



  %-gcc: CC = gcc
%-clang: CC = clang

  debug-%: CF += -O0 -g
release-%: CF += -O3 -DNDEBUG

linux%:  CF += -Wall -std=c++11
linux%:  XF += -Wall -std=c11
linux3%: CF += -m32
linux6%: CF += -m64


build: config $(TARGET)
check: build $(TEST_TARGET)

linux32-build: build
linux64-build: build

linux32-check: check
linux64-check: check


  debug-build-linux32-gcc:   linux32-build
  debug-check-linux32-gcc:   linux32-check
release-build-linux32-gcc:   linux32-build
release-check-linux32-gcc:   linux32-check

  debug-build-linux64-gcc:   linux64-build
  debug-check-linux64-gcc:   linux64-check
release-build-linux64-gcc:   linux64-build
release-check-linux64-gcc:   linux64-check

  debug-build-linux32-clang: linux32-build
  debug-check-linux32-clang: linux32-check
release-build-linux32-clang: linux32-build
release-check-linux32-clang: linux32-check

  debug-build-linux64-clang: linux64-build
  debug-check-linux64-clang: linux64-check
release-build-linux64-clang: linux64-build
release-check-linux64-clang: linux64-check


  debug:   debug-build-linux64-clang
release: clean release-build-linux64-clang

test:           debug-check-linux64-clang
test-release: release-check-linux64-clang


config: CFG=CC="$(CC)" CF="$(CF)" XF="$(XF)"
config:
	@echo "CFG  $(CFG)"


obj/%.o: %.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(CI) -c $< -o $@

obj/%.o: %.c
	@mkdir -p `dirname $@`
	@echo "C   " $<
	@$(CC) $(XF) $(CI) -c $< -o $@


libstdhlc.a: $(XO)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
	@ranlib $@

libstdhlcpp.a: $(CO)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
	@ranlib $@

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " $(TARGET)
	@rm -f $(TARGET)
	@echo "RM  " $(TEST_TARGET)
	@rm -f $(TEST_TARGET)


#TEST_TARGET = $(TARGET:%.a=%-test.a)


TF   = $(shell find uts -name '*.cpp' | cut -d'.' -f1)
TO = $(TF:%=obj/%.o)

TI  = -I $(LIB)/$(GT)/googletest/include
TI += -I $(LIB)/$(GT)/googletest

TL  = -lstdc++
TL += -lm
TL += -lpthread

obj/uts/%.o: uts/%.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	$(CC) $(CF) $(TI) $(CI) -c $< -o $@

$(TEST_TARGET): $(CO) $(TO)
	@echo "LD " $@
	$(CC) \
	  $(CF) \
	  $(TI) \
	  $(CI) \
	  $(TL) \
	  -o $@ \
	  $(TO) \
	  libstdhlcpp.a  \
	  $(LIB)/$(GT)/googletest/src/gtest-all.cc \
	  $(LIB)/$(GT)/googletest/src/gtest_main.cc 
	@echo "RUN " $@
	@./$@


# test: #$(TEST_TARGET)
# 	@rm -f $@
# 	@echo "LD  " $@
# 	@$(CC) \
# 	  $(CF) \
# 	  $(TI) \
# 	  $(CI) \
# 	  $(TL) \
# 	  -o $@ \
# 	  libstdhlcpp.a  \
# 	  $(TEST_OBJECTS) \
# 	  ../gtest/googletest/src/gtest-all.cc \
# 	  ../gtest/googletest/src/gtest_main.cc 
# 	@echo "RUN " $@
# 	@./$@

# $(TEST_TARGET): $(TEST_OBJECTS)
#	@echo "AR  " $@
#	@$(AR) rsc $@ $(filter %.o,$^)
#	@ranlib $@


