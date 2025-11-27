# Makefile para IBILCE Block C com suporte a texturas

CC = gcc
CFLAGS = -Wall -O2 -Iinclude
LIBS = -lGL -lGLU -lglut -lm

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
EXAMPLES_DIR = examples

# Arquivos fonte
MAIN_SRC = $(SRC_DIR)/main.c
LOADER_SRC = $(SRC_DIR)/obj_loader.c

# Arquivos objeto
OBJS = $(BUILD_DIR)/obj_loader.o

# Executáveis
TARGET = ibilce_blockC
TEST = test_texture

.PHONY: all clean test dirs

all: dirs $(TARGET)

# Cria diretórios necessários
dirs:
	@mkdir -p $(BUILD_DIR)

# Compila o projeto principal
$(TARGET): $(MAIN_SRC) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "✓ Compilado com sucesso: $(TARGET)"
	@echo "Execute com: ./$(TARGET)"

# Compila obj_loader.o
$(BUILD_DIR)/obj_loader.o: $(LOADER_SRC) $(INCLUDE_DIR)/obj_loader.h $(INCLUDE_DIR)/stb_image.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa arquivos compilados
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET) $(TEST)
	@echo "✓ Arquivos compilados removidos"

# Executa o projeto principal
run: $(TARGET)
	./$(TARGET)

# Executa o programa de teste
run-test: $(TEST)
	./$(TEST)

# Informações sobre uso
help:
	@echo "Makefile para IBILCE Block C"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto principal"
	@echo "  make test     - Compila o programa de teste"
	@echo "  make run      - Compila e executa o projeto principal"
	@echo "  make run-test - Compila e executa o programa de teste"
	@echo "  make clean    - Remove arquivos compilados"
	@echo "  make help     - Mostra esta mensagem"
