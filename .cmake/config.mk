#
#   Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 <https://github.com/casm-lang/libstdhl>
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
#   Additional permission under GNU GPL version 3 section 7
#
#   libstdhl is distributed under the terms of the GNU General Public License
#   with the following clarification and special exception: Linking libstdhl
#   statically or dynamically with other modules is making a combined work
#   based on libstdhl. Thus, the terms and conditions of the GNU General
#   Public License cover the whole combination. As a special exception,
#   the copyright holders of libstdhl give you permission to link libstdhl
#   with independent modules to produce an executable, regardless of the
#   license terms of these independent modules, and to copy and distribute
#   the resulting executable under terms of your choice, provided that you
#   also meet, for each linked independent module, the terms and conditions
#   of the license of that module. An independent module is a module which
#   is not derived from or based on libstdhl. If you modify libstdhl, you
#   may extend this exception to your version of the library, but you are
#   not obliged to do so. If you do not wish to do so, delete this exception
#   statement from your version.
#

ifndef TARGET
  $(error TARGET not defined!)
endif

OBJ = obj
BIN = install
.PHONY: $(OBJ)
.NOTPARALLEL: $(OBJ)

CLANG := $(shell clang --version 2> /dev/null)

ifdef C
  ENV_CC=$(C)
else
  ifdef CLANG
    ENV_CC=clang
  endif
endif

ifeq ($(ENV_CC),)
  $(error environment C compiler '$(C)' not defined!)
endif

ifdef X
  ENV_CXX=$(X)
else
  ifdef CLANG
    ENV_CXX=clang++
  endif
endif

ifeq ($(ENV_CXX),)
  $(error environment C++ compiler '$(X)' not defined!)
endif


ifdef ENV_GEN
  G=$(ENV_GEN)
endif

ifndef G
  G=make
endif

# Unix Makefiles
ifeq ($(G),make)
  $(eval ENV_GEN="Unix Makefiles")
endif
ifeq ($(G),make-cb)
  $(eval ENV_GEN="CodeBlocks - Unix Makefiles")
endif
ifeq ($(G),make-cl)
  $(eval ENV_GEN="CodeLite - Unix Makefiles")
endif
ifeq ($(G),make-s2)
  $(eval ENV_GEN="Sublime Text 2 - Unix Makefiles")
endif
ifeq ($(G),make-kp)
  $(eval ENV_GEN="Kate - Unix Makefiles")
endif
ifeq ($(G),make-e4)
  $(eval ENV_GEN="Eclipse CDT4 - Unix Makefiles")
endif

# MinGW Makefiles
ifeq ($(G),make-gw)
  $(eval ENV_GEN="MinGW Makefiles")
endif
ifeq ($(G),make-gw-cb)
  $(eval ENV_GEN="CodeBlocks - MinGW Makefiles")
endif
ifeq ($(G),make-gw-cl)
  $(eval ENV_GEN="CodeLite - MinGW Makefiles")
endif
ifeq ($(G),make-gw-s2)
  $(eval ENV_GEN="Sublime Text 2 - MinGW Makefiles")
endif
ifeq ($(G),make-gw-kp)
  $(eval ENV_GEN="Kate - MinGW Makefiles")
endif
ifeq ($(G),make-gw-e4)
  $(eval ENV_GEN="Eclipse CDT4 - MinGW Makefiles")
endif

# Msys Makefiles
ifeq ($(G),make-ms)
  $(eval ENV_GEN="MSYS Makefiles")
endif

# Watcom Makefiles
ifeq ($(G),make-wc)
  $(eval ENV_GEN="Watcom WMake")
endif

# Borland Makefiles
ifeq ($(G),make-bl)
  $(eval ENV_GEN="Borland Makefiles")
endif

# NMake Makefiles
ifeq ($(G),make-nm)
  $(eval ENV_GEN="NMake Makefiles")
endif
ifeq ($(G),make-nm-cb)
  $(eval ENV_GEN="CodeBlocks - NMake Makefiles")
endif
ifeq ($(G),make-nm-cl)
  $(eval ENV_GEN="CodeLite - NMake Makefiles")
endif
ifeq ($(G),make-nm-s2)
  $(eval ENV_GEN="Sublime Text 2 - NMake Makefiles")
endif
ifeq ($(G),make-nm-kp)
  $(eval ENV_GEN="Kate - NMake Makefiles")
endif
ifeq ($(G),make-nm-e4)
  $(eval ENV_GEN="Eclipse CDT4 - NMake Makefiles")
endif
ifeq ($(G),make-nj)
  $(eval ENV_GEN="NMake Makefiles JOM")
endif
ifeq ($(G),make-nj-cb)
  $(eval ENV_GEN="CodeBlocks - NMake Makefiles JOM")
endif

# Green Hills MULTI
ifeq ($(G),multi)
  $(eval ENV_GEN="Green Hills MULTI")
endif

# Ninja
ifeq ($(G),ninja)
  $(eval ENV_GEN="Ninja")
endif
ifeq ($(G),ninja-cb)
  $(eval ENV_GEN="CodeBlocks - Ninja")
endif
ifeq ($(G),ninja-cl)
  $(eval ENV_GEN="CodeLite - Ninja")
endif
ifeq ($(G),ninja-s2)
  $(eval ENV_GEN="Sublime Text 2 - Ninja")
endif
ifeq ($(G),ninja-kp)
  $(eval ENV_GEN="Kate - Ninja")
endif
ifeq ($(G),ninja-e4)
  $(eval ENV_GEN="Eclipse CDT4 - Ninja")
endif

# Visual Studio 2017 (Version 15)
ifeq ($(G),vs17)
  $(eval ENV_GEN="Visual Studio 15 2017")
endif
ifeq ($(G),vs17w64)
  $(eval ENV_GEN="Visual Studio 15 2017 Win64")
endif
ifeq ($(G),vs17arm)
  $(eval ENV_GEN="Visual Studio 15 2017 ARM")
endif

# Visual Studio 2015 (Version 14)
ifeq ($(G),vs15)
  $(eval ENV_GEN="Visual Studio 14 2015")
endif
ifeq ($(G),vs15w64)
  $(eval ENV_GEN="Visual Studio 14 2015 Win64")
endif
ifeq ($(G),vs15arm)
  $(eval ENV_GEN="Visual Studio 14 2015 ARM")
endif

# Visual Studio 2013 (Version 12)
ifeq ($(G),vs13)
  $(eval ENV_GEN="Visual Studio 12 2013")
endif
ifeq ($(G),vs13w64)
  $(eval ENV_GEN="Visual Studio 12 2013 Win64")
endif
ifeq ($(G),vs13arm)
  $(eval ENV_GEN="Visual Studio 12 2013 ARM")
endif

# Visual Studio 2012 (Version 11)
ifeq ($(G),vs12)
  $(eval ENV_GEN="Visual Studio 11 2012")
endif
ifeq ($(G),vs12w64)
  $(eval ENV_GEN="Visual Studio 11 2012 Win64")
endif
ifeq ($(G),vs12arm)
  $(eval ENV_GEN="Visual Studio 11 2012 ARM")
endif

# Visual Studio 2010 (Version 10)
ifeq ($(G),vs10)
  $(eval ENV_GEN="Visual Studio 10 2010")
endif
ifeq ($(G),vs10w64)
  $(eval ENV_GEN="Visual Studio 10 2010 Win64")
endif
ifeq ($(G),vs10ia64)
  $(eval ENV_GEN="Visual Studio 10 2010 IA64")
endif

# Visual Studio 2008 (Version 10)
ifeq ($(G),vs08)
  $(eval ENV_GEN="Visual Studio 9 2008")
endif
ifeq ($(G),vs08w64)
  $(eval ENV_GEN="Visual Studio 9 2008 Win64")
endif
ifeq ($(G),vs08ia64)
  $(eval ENV_GEN="Visual Studio 9 2008 IA64")
endif

ifeq ($(ENV_GEN),)
  $(error environment generator '$(G)' not supported!, see 'make info-generators')
endif


default: debug

help:
	@echo "TODO"


$(OBJ):
	@mkdir -p $(OBJ)

clean:
ifneq ("$(wildcard $(OBJ)/CMakeCache.txt)","")
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) clean
endif

clean-all:
	@echo "-- Removing build directory" $(OBJ)
	@rm -rf $(OBJ)

TYPES = debug sanitize release

SYNCS = $(TYPES:%=%-sync)
TESTS = $(TYPES:%=%-test)
BENCH = $(TYPES:%=%-benchmark)
INSTA = $(TYPES:%=%-install)
ANALY = $(TYPES:%=%-analyze)
ALL   = $(TYPES:%=%-all)


$(OBJ)/Makefile: $(OBJ)
ifeq ("$(wildcard $(OBJ)/CMakeCache.txt)","")
	@(\
	cd $(OBJ); \
	cmake \
	-G $(ENV_GEN) \
	-D CMAKE_INSTALL_PREFIX=$(BIN) \
	-D CMAKE_BUILD_TYPE=$(TYPE) \
	-D CMAKE_C_COMPILER=$(ENV_CC) \
	-D CMAKE_CXX_COMPILER=$(ENV_CXX) \
	.. \
	)
else
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) rebuild_cache
endif


sync: debug-sync

sync-all: $(TYPES:%=%-sync)

$(SYNCS):%-sync: $(OBJ)
	@$(MAKE) $(MFLAGS) --no-print-directory TYPE=$(patsubst %-sync,%,$@) $(OBJ)/Makefile


$(TYPES):%: %-sync
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) ${TARGET}

all: debug-all

$(ALL):%-all: %-sync
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ)


test: debug-test

test-all: $(TYPES:%=%-test)

$(TESTS):%-test: %
	@$(MAKE) $(MFLAGS) --no-print-directory \
	-C $(OBJ) $(TARGET)-check
	@echo "-- Running unit test"
	@$(ENV_FLAGS) ./$(OBJ)/$(TARGET)-check --gtest_output=xml:obj/report.xml $(ENV_ARGS)


benchmark: debug-benchmark

benchmark-all: $(TYPES:%=%-benchmark)

$(BENCH):%-benchmark: %
	@$(MAKE) $(MFLAGS) --no-print-directory \
	-C $(OBJ) $(TARGET)-run
	@echo "-- Running benchmark"
	@$(ENV_FLAGS) ./$(OBJ)/$(TARGET)-run -o console -o json:obj/report.json $(ENV_ARGS)


install: debug-install

install-all: $(TYPES:%=%-install)

$(INSTA):%-install: %
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) install


format: $(FORMAT:%=%-format-cpp)

%-format-cpp:
	@echo "-- Formatting Code C++: $(patsubst %-format-cpp,%,$@)"
	@clang-format -i \
	`ls $(patsubst %-format-cpp,%,$@)/*.h 2> /dev/null | grep -e .h` 2> /dev/null
	@clang-format -i \
	`ls $(patsubst %-format-cpp,%,$@)/*.cpp 2> /dev/null | grep -e .cpp` 2> /dev/null


update: $(UPDATE_FILE:%=%-update)

%-update:
	@echo "-- Updating: $(patsubst %-update,%,$@)"
	@for i in $(UPDATE_PATH); \
	  do \
	    cp -v \
	    $(UPDATE_ROOT)/$(patsubst %-update,%,$@) \
	    $$i/$(patsubst %-update,%,$@); \
	  done


license: $(UPDATE_ROOT:%=%-license) $(UPDATE_PATH:%=%-license)

%-license:
	@echo "-- Relicense: $(patsubst %-update,%,$@)"
	@(cd $(patsubst %-update,%,$@); \
	  python2 $(UPDATE_ROOT)/src/py/Licenser.py \
	)

license-info:
	@grep LICENSE.txt -e "---:" | sed "s/---://g"
	@head LICENSE.txt -n `grep -B1 -ne "---" LICENSE.txt | head -n 1 | sed "s/-//g"` > $(OBJ)/notice.txt
	@cat $(OBJ)/notice.txt | sed "s/^/  /g" | sed "s/$$/\\\n/g" | tr -d '\n' > $(OBJ)/notice


analyze: debug-analyze

analyze-all: $(TYPES:%=%-analyze)

$(ANALY):%-analyze: %
	@echo "-- Running analysis tools"
	$(MAKE) $(MFLAGS) $@-cppcheck
	$(MAKE) $(MFLAGS) $@-iwyu
	$(MAKE) $(MFLAGS) $@-scan-build


analyze-cppcheck: debug-analyze-cppcheck

CPPCHECK_REPORT = ./$(OBJ)/.cppcheck.xml

%-analyze-cppcheck:
	@echo "-- Running 'cppcheck' $(patsubst %-analyze-cppcheck,%,$@)"
	@echo -n "" > $(CPPCHECK_REPORT)
	cppcheck \
	-v \
	--template=gcc \
	--force \
	--report-progress \
	--enable=all \
	-I . \
	./src/c**

	cppcheck \
	-v \
	--template=gcc \
	--errorlist \
	--xml-version=2 \
	--force \
	--enable=all \
	-I . \
	./src/c** > $(CPPCHECK_REPORT)


analyze-iwyu: debug-analyze-iwyu

IWYU_REPORT = ./$(OBJ)/.iwyu.txt

%-analyze-iwyu:
	@echo "-- Running 'iwyu' $(patsubst %-analyze-iwyu,%,$@)"
	@echo -n "" > $(IWYU_REPORT)
	@for i in `find ./c*`; do include-what-you-use $$i; done
	@for i in `find ./c*`; do include-what-you-use $$i >> $(IWYU_REPORT); done


analyze-scan-build: debug-analyze-scan-build

SCAN_BUILD_REPORT = ./$(OBJ)/.scan-build
SCAN_BUILD_REPORT_ATTIC = $(SCAN_BUILD_REPORT).attic

%-analyze-scan-build: clean
	@echo "-- Running 'scan-build' $(patsubst %-analyze-scan-build,%,$@)"
	@mkdir -p $(SCAN_BUILD_REPORT_ATTIC)

	scan-build \
	-v \
	-o $(SCAN_BUILD_REPORT).attic \
	-stats \
	-plist-html \
	-analyzer-config stable-report-filename=true \
	-enable-checker llvm.Conventions \
	--force-analyze-debug-code \
	--keep-going \
	--keep-empty \
	$(MAKE) $(MFLAGS) $(patsubst %-analyze-scan-build,%,$@)

	@ln -f -s \
	$(SCAN_BUILD_REPORT_ATTIC)/`ls -t $(SCAN_BUILD_REPORT_ATTIC) | head -1` \
	$(SCAN_BUILD_REPORT)


info:
	@echo "-- Environment Configuration"
	@echo "   G =" $(ENV_GEN)
	@echo "   C =" $(ENV_CC)
	@echo "   X =" $(ENV_CXX)


info-variables:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))


info-generators:
	@echo "-- Enviroment CMake Generator Aliases"
	@echo "   make       = 'Unix Makefiles'"
	@echo "   make-cb    = 'CodeBlocks - Unix Makefiles'"
	@echo "   make-cl    = 'CodeLite - Unix Makefiles'"
	@echo "   make-s2    = 'Sublime Text 2 - Unix Makefiles'"
	@echo "   make-kp    = 'Kate - Unix Makefiles'"
	@echo "   make-e4    = 'Eclipse CDT4 - Unix Makefiles'"
	@echo "   make-gw    = 'MinGW Makefiles'"
	@echo "   make-gw-cb = 'CodeBlocks - MinGW Makefiles'"
	@echo "   make-gw-cl = 'CodeLite - MinGW Makefiles'"
	@echo "   make-gw-s2 = 'Sublime Text 2 - MinGW Makefiles'"
	@echo "   make-gw-kp = 'Kate - MinGW Makefiles'"
	@echo "   make-gw-e4 = 'Eclipse CDT4 - MinGW Makefiles'"
	@echo "   make-ms    = 'MSYS Makefiles'"
	@echo "   make-wc    = 'Watcom WMake'"
	@echo "   make-bl    = 'Borland Makefiles'"
	@echo "   make-nm    = 'NMake Makefiles'"
	@echo "   make-nm-cb = 'CodeBlocks - NMake Makefiles'"
	@echo "   make-nm-cl = 'CodeLite - NMake Makefiles'"
	@echo "   make-nm-s2 = 'Sublime Text 2 - NMake Makefiles'"
	@echo "   make-nm-kp = 'Kate - NMake Makefiles'"
	@echo "   make-nm-e4 = 'Eclipse CDT4 - NMake Makefiles'"
	@echo "   make-nj    = 'NMake Makefiles JOM'"
	@echo "   make-nj-cb = 'CodeBlocks - NMake Makefiles JOM'"
	@echo "   multi      = 'Green Hills MULTI'"
	@echo "   ninja      = 'Ninja'"
	@echo "   ninja-cb   = 'CodeBlocks - Ninja'"
	@echo "   ninja-cl   = 'CodeLite - Ninja'"
	@echo "   ninja-s2   = 'Sublime Text 2 - Ninja'"
	@echo "   ninja-kp   = 'Kate - Ninja'"
	@echo "   ninja-e4   = 'Eclipse CDT4 - Ninja'"
	@echo "   vs17       = 'Visual Studio 15 2017'"
	@echo "   vs17w64    = 'Visual Studio 15 2017 Win64'"
	@echo "   vs17arm    = 'Visual Studio 15 2017 ARM'"
	@echo "   vs15       = 'Visual Studio 14 2015'"
	@echo "   vs15w64    = 'Visual Studio 14 2015 Win64'"
	@echo "   vs15arm    = 'Visual Studio 14 2015 ARM'"
	@echo "   vs13       = 'Visual Studio 12 2013'"
	@echo "   vs13w64    = 'Visual Studio 12 2013 Win64'"
	@echo "   vs13arm    = 'Visual Studio 12 2013 ARM'"
	@echo "   vs12       = 'Visual Studio 11 2012'"
	@echo "   vs12w64    = 'Visual Studio 11 2012 Win64'"
	@echo "   vs12arm    = 'Visual Studio 11 2012 ARM'"
	@echo "   vs10       = 'Visual Studio 10 2010'"
	@echo "   vs10w64    = 'Visual Studio 10 2010 Win64'"
	@echo "   vs10ia64   = 'Visual Studio 10 2010 IA64'"
	@echo "   vs08       = 'Visual Studio 9 2008'"
	@echo "   vs08w64    = 'Visual Studio 9 2008 Win64'"
	@echo "   vs08ia64   = 'Visual Studio 9 2008 IA64'"
