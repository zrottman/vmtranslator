PATHS = src/
PATHB = build/
PATHO = build/objs/
PATHT = test/

OUTFILE = vmtranslator
BUILD_PATHS = $(PATHB) $(PATHO)

COMPILE = cc -c
LINK = cc

SRC = $(wildcard $(PATHS)*.c)
SRCT = $(wildcard $(PATHT)*.c)

OBJS = $(patsubst $(PATHS)%.c, $(PATHO)%.o, $(SRC))
OBJT = $(patsubst $(PATHT)%.c, $(PATHO)%.o, $(SRCT))


# Main program
$(PATHB)$(OUTFILE) : $(BUILD_PATHS) $(OBJS) $(PATHB)test_utils
	$(LINK) -o $@ $(OBJS) 

$(PATHO)%.o :: $(PATHS)%.c
	$(COMPILE) $< -o $@


# Tests
$(PATHB)test_utils : $(PATHO)test_utils.o $(PATHO)utils.o $(PATHO)unity.o
	$(LINK) -o $@ $^

$(PATHO)%.o :: $(PATHT)%.c
	$(COMPILE) $< -o $@

$(PATHO)unity.o :: unity/unity.c
	$(COMPILE) $< -o $@


# Create necessary directories
$(PATHB) : 
	mkdir -p $(PATHB)

$(PATHO) : 
	mkdir -p $(PATHO)


# Cleanup
.PHONY : clean
clean :
	rm $(PATHB)$(OUTFILE)
	rm $(PATHB)test_utils
	rm $(PATHO)unity.o
	rm $(OBJS)
	rm $(OBJT)
	
