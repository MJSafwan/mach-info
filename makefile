CC = gcc
ODIR = ./tmp
SDIR = ./src
IDIR = $(SDIR)/include
DEBUG = -glldb
CFLAGS = -Wall -Wextra -pedantic -I$(IDIR)
VPATH = $(SDIR)
SRC = $(wildcard $(SDIR)/*.c)
DDIR = $(ODIR)/dep
_OBJ = $(shell echo $(SRC) | tr "/" " ")
__OBJ = $(filter %.c, $(_OBJ))
___OBJ = $(patsubst %.c,%.o,$(__OBJ))
OBJ = $(addprefix $(ODIR)/,$(___OBJ))
TARGET = mach_info

.PHONY: clean all

all: $(TARGET)

clean:
	@rm -r $(ODIR)

$(TARGET) : $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(OBJ): $(ODIR)/%.o: %.c | $(ODIR) $(DDIR)
	$(CC) $(CFLAGS) -o $@ -MMD -MF $(DDIR)/$*.dep  -c $<
include $(wildcard $(DDIR)/$*.dep)

$(ODIR):
	@mkdir -p $(ODIR)

$(DDIR):
	@mkdir -p $(DDIR)
