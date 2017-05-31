C_FILES := $(shell find ./ -name '*.c')
H_FILES := $(shell find ./ -name '*.h')

BASE_DIR := $(shell pwd )
TARGET_DIR := $(BASE_DIR)/../uncomment

C_OBJ_FILES := $(patsubst %.c, $(BASE_DIR)/../uncomment/%.c, $(C_FILES))
H_OBJ_FILES := $(patsubst %.h, $(BASE_DIR)/../uncomment/%.h, $(H_FILES))

UNCOMMENT_CMD := gcc -fpreprocessed -dD -E -P
.PHONY:prebuild all

all:$(C_OBJ_FILES) $(H_OBJ_FILES)| prebuild
$(C_OBJ_FILES):$(TARGET_DIR)/%.c:$(BASE_DIR)/%.c
	@-mkdir -p $(dir $@)
	# @echo BASE_DIR $(BASE_DIR)
	# @echo TARGET_DIR $(TARGET_DIR)
	@echo C_FILES $(C_FILES)
	@echo C_OBJ_FILES $(C_OBJ_FILES)
	-$(UNCOMMENT_CMD) $^  > $@
$(H_OBJ_FILES):$(TARGET_DIR)/%.h:$(BASE_DIR)/%.h
	@-mkdir -p $(dir $@)
	@echo H_FILES $(H_FILES)
	@echo H_OBJ_FILES $(H_OBJ_FILES)
	-$(UNCOMMENT_CMD) $^  > $@

prebuild:
	@-mkdir -p $(TARGET_DIR)
