CC = clang

CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I. -g

TARGET = gsh

VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

SRCS = parser/main.c \
       lexer/lexer.c \
       parser/parser.c

OBJS = $(SRCS:.c=.o)

# --- REGOLE DI COMPILAZIONE ---

all: $(TARGET)

# Linking: Unisce tutti i file .o per creare l'eseguibile
# (Nota: assicurati che la rientranza qui sotto sia un TAB, non spazi)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per avviare il programma sotto Valgrind
# Assicura prima che l'eseguibile sia aggiornato (dipendenza $(TARGET))
valgrind: $(TARGET)
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

# Pulizia dei file generati
clean:
	rm -f $(OBJS) $(TARGET)

# Regola per ripulire e ricompilare da zero
re: clean all

# Dichiarazione delle regole che non sono file reali
.PHONY: all clean re valgrind