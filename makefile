PATHS = src/
PATHB = build/
PATHO = build/objs/
PATHT = test/

OUTFILE = vmtranslator
BUILD_PATHS = $(PATHB) $(PATHO)

COMPILE = cc -c
LINK = cc

SRC = $(wildcard $(PATHS)*.c)  # src/main.c src/parser.c ...
SRCT = $(wildcard $(PATHT)*.c) # test/test_utils.c test/test_parser.c ...

OBJS = $(patsubst $(PATHS)%.c, $(PATHO)%.o, $(SRC))  # build/objs/main.o build/objs/utils.o ...
OBJT = $(patsubst $(PATHT)%.c, $(PATHO)%.o, $(SRCT)) # build/objs/test_utils.o build/objs/test_parser.o ...
TESTS = $(patsubst $(PATHT)%.c, $(PATHB)%, $(SRCT))  # build/test_utils build/test_parser ...


# Main program
$(PATHB)$(OUTFILE) : $(BUILD_PATHS) $(OBJS) $(PATHB) $(TESTS)
	$(LINK) -o $@ $(OBJS) 

$(PATHO)%.o :: $(PATHS)%.c
	$(COMPILE) $< -o $@


# Tests
$(PATHB)test_% : $(PATHO)test_%.o $(PATHO)%.o $(PATHO)unity.o
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
	rm $(TESTS)
	rm $(PATHO)unity.o
	rm $(OBJS)
	rm $(OBJT)
	
