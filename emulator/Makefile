


BUILD_DIR = build


ifeq ($(PLATFORM),WINDOWS) #build for windows platform (cross compiling from linux docker)

  TARGET_LIB = $(BUILD_DIR)/libbeeline.dll
  GXX = /usr/bin/x86_64-w64-mingw32-g++
  GCC = /usr/bin/x86_64-w64-mingw32-gcc
  COMMONFLAGS += -fsingle-precision-constant -Wimplicit-fallthrough=2
  LDFLAGS += -static


else ifeq ($(PLATFORM),LINUX) #build for a native linux platform (compiling on linux)

  TARGET_LIB = $(BUILD_DIR)/libbeeline.so
  GXX = g++
  GCC = gcc
  COMMONFLAGS += -fsingle-precision-constant -Wimplicit-fallthrough=2
 
else ifeq ($(PLATFORM),MACOS) #build for a macOS platform (compiling on macOS)

  TARGET_LIB = $(BUILD_DIR)/libbeeline.dylib
  GXX = clang++
  GCC = clang
  COMMONFLAGS += -cl-single-precision-constant
  LDFLAGS = -shared

else 
  $(error "Platform not supported, please specify PLATFORM=WINDOWS, PLATFORM=LINUX or PLATFORM=MACOS")
endif


DEP_DIRECTORY = $(BUILD_DIR)/dep
OBJECT_DIRECTORY := $(BUILD_DIR)/obj


COMMONFLAGS += -fPIC
COMMONFLAGS += -Wall -Wno-error=format-security


# Generate dependency information, to prevent unnecessary recompilation
COMMONFLAGS += -MD -MP -MF $(DEP_DIRECTORY)/$(@F).d

LDFLAGS += -shared 

APP_DIR = ../app
EMU_DIR = .

SRC_FILES = \
  $(APP_DIR)/src/main.cpp \
  $(APP_DIR)/src/drivers/lcd_colour.c \
  $(APP_DIR)/src/drivers/time.c \
  $(APP_DIR)/src/drivers/ble.c \
  $(APP_DIR)/src/graphics/graphics.cpp \
  $(APP_DIR)/src/graphics/fonts/font_bariol_36.c \
  $(APP_DIR)/src/graphics/fonts/font_bariol_64.c \
  $(APP_DIR)/src/graphics/fonts/font_data_common.c \
  $(APP_DIR)/src/graphics/fonts/fonts.c \
  $(APP_DIR)/src/graphics/font_renderer.c \


SRC_FILES += \
  $(EMU_DIR)/src/interface.cpp

INC_FOLDERS = \
  $(APP_DIR)/src 


OBJECTS :=  $(addprefix $(OBJECT_DIRECTORY)/, $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.bin,%.o,$(notdir $(SRC_FILES)) ))) )


#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))
PATHS = $(call remduplicates, $(dir $(SRC_FILES) ) )

vpath %.cpp $(PATHS)
vpath %.c $(PATHS)
vpath %.bin $(PATHS)

INC_PATHS = $(foreach var,$(INC_FOLDERS),-I$(var))


# Create objects from CPP SRC files
$(OBJECT_DIRECTORY)/%.o: %.cpp Makefile
	@echo Compiling CPP file: $(notdir $<)
	@mkdir -p $(dir $@)
	@mkdir -p $(DEP_DIRECTORY)
	$(GXX) $(COMMONFLAGS) -std=gnu++14 $(INC_PATHS) -c -o $@ $<

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c Makefile
	@echo Compiling C file: $(notdir $<)
	@mkdir -p $(dir $@)
	@mkdir -p $(DEP_DIRECTORY)
	@$(GCC) $(COMMONFLAGS) $(INC_PATHS) -c -o $@ $<


$(TARGET_LIB): $(OBJECTS)
	@echo Linking...
	$(GXX) $(LDFLAGS) -o $(TARGET_LIB) $(OBJECTS)

.PHONY: all clean
all: $(TARGET_LIB)

clean:
	@rm -rf $(BUILD_DIR)

-include $(DEP_DIRECTORY)/*.d #use -include as it doesn't error when no dependency files exist
