#   
#   Copyright (c) 2016 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/libstdhl
#   
#   This software is licensed under the Open Software License v. 3.0 (OSL-3.0).
#   You either find the license text below or in the root directory of this 
#   project in the LICENSE.txt file. If you cannot find the LICENSE.txt file,
#   please refer to https://opensource.org/licenses/OSL-3.0.
#   
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
#   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#   CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
#   WITH THE SOFTWARE.
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
