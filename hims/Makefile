CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

TARGET = hims.exe

SRCS = main.c auth.c patient.c doctor.c department.c billing.c report.c file.c utils.c pharmacy.c appointment.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q /F *.o $(TARGET) 2>nul || rm -f $(OBJS) $(TARGET)
