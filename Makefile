# === Compiler and Flags ===
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Iinclude

# === Directories ===
SRC_DIR = src
BUILD_DIR = build

# === Source Files ===
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:%.c=%.o)

# === Binary Output ===
TARGET = $(BUILD_DIR)/echess

# === Default Rule ===
all: $(TARGET)

# === Link Objects to Binary ===
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# === Compile Each .c to .o ===
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# === Clean Up ===
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# === Run (optional) ===
run: all
	./$(TARGET)

