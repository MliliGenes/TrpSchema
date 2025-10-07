# Get current date and time
DATE := $(shell date +"%Y-%m-%d %H:%M:%S")

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -ggdb -std=c++98 -Iinclude -Ilib

INCLUDE_DIR = include

HEADER_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)

SRCDIR = src
TARGET = trpschema

# Get all .cpp files from root and src directory
ROOT_SRC = $(wildcard *.cpp)
SRCDIR_SRC = $(wildcard $(SRCDIR)/*.cpp)

TRP_SRC = $(SRCDIR_SRC)
SRC = $(ROOT_SRC) $(TRP_SRC)

# Library sources (exclude main.cpp for static library)
LIB_SRC = $(TRP_SRC)
STATIC_LIB = lib/libtrpschema.a

OBJDIR = build

# Maintain directory hierarchy in build dir
# Root .cpp files go to build/*.o
# src/*.cpp files go to build/src/*.o  
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(ROOT_SRC)) \
    $(LIB_OBJ)

# Library object files (exclude main.o)
LIB_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/$(SRCDIR)/%.o,$(SRCDIR_SRC))

TOTAL_FILES := $(words $(SRC))

CURRENT_FILE = 0

define print_progress
$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE) + 1))))
$(eval PERCENT=$(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_FILES)))))
echo "[$(DATE)] [Compiling] $< → $@ - $(PERCENT)% complete"
endef

all: $(TARGET)

$(TARGET): $(OBJ) $(HEADER_FILES)
	@echo "[$(DATE)] [Linking] $@"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@ -Llib -ltrpjson
	@echo "[$(DATE)] [Built] $@ - 100% complete"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(call print_progress)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

re: fclean all

clean:
	@echo "[$(DATE)] [Cleaning] removing object files"
	@rm -rf $(OBJDIR)

fclean: clean lib-clean
	@echo "[$(DATE)] [Cleaning] removing binary $(TARGET)"
	@rm -f $(TARGET)
	@rm -f $(STATIC_LIB)












lib: $(STATIC_LIB)

$(STATIC_LIB): $(LIB_OBJ) $(HEADER_FILES)
	@mkdir -p $(dir $@)
	@echo "[$(DATE)] [Creating Static Library] $@"
	@ar rcs $@ $(LIB_OBJ)
	@echo "[$(DATE)] [Built] $@ - Static library ready for use!"

lib-re: lib-clean lib

lib-clean:
	@echo "[$(DATE)] [Cleaning] removing static library"
	@rm -f $(STATIC_LIB)

install: $(STATIC_LIB)
	@echo "[$(DATE)] [Installing] TrpSchema library to system"
	@sudo mkdir -p /usr/local/lib
	@sudo mkdir -p /usr/local/include
	@sudo cp $(STATIC_LIB) /usr/local/lib/
	@sudo cp TrpSchema.hpp /usr/local/include/
	@sudo cp lib/TrpJson.hpp /usr/local/include/
	@echo "[$(DATE)] [Installed] TrpSchema library to /usr/local/"
	@echo "   Use: g++ -std=c++98 -ltrpschema -ltrpjson your_file.cpp"
	@echo "   Include: #include <TrpSchema.hpp>"

uninstall:
	@echo "[$(DATE)] [Uninstalling] TrpSchema library from system"
	@sudo rm -f /usr/local/lib/libtrpschema.a
	@sudo rm -f /usr/local/include/TrpSchema.hpp
	@sudo rm -f /usr/local/include/TrpJson.hpp
	@echo "[$(DATE)] [Uninstalled] TrpSchema library removed"

.PHONY: all re clean fclean lib lib-re lib-clean install uninstall