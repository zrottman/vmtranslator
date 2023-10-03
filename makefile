PATHS = src/
PATHB = build/
PATHO = build/objs/
PATHR = build/results/
PATHT = test/

.PHONY : test
.PHONY : clean

OUTFILE = vmtranslator
BUILD_PATHS = $(PATHB) $(PATHO) $(PATHR)

COMPILE = cc -c
LINK = cc

SRC = $(wildcard $(PATHS)*.c)
SRCT = $(wildcard $(PATHT)*.c)

OBJS = $(patsubst $(PATHS)%.c, $(PATHO)%.o, $(SRC))  # build/objs/main.o build/objs/utils.o ...
#OBJT = $(patsubst $(PATHT)%.c, $(PATHO)%.o, $(SRCT)) # build/objs/test_utils.o build/objs/test_parser.o ...
TESTS = $(patsubst $(PATHT)%.c, $(PATHB)%, $(SRCT))  # build/test_utils build/test_parser ...
MAIN = $(PATHO)main.o


# Main program
$(PATHB)$(OUTFILE) : $(BUILD_PATHS) $(OBJS) $(PATHB) $(TESTS) test
	$(LINK) -o $@ $(OBJS) 

$(PATHO)%.o :: $(PATHS)%.c
	$(COMPILE) $< -o $@


# Build tests
$(PATHB)test_% : $(PATHO)test_%.o $(filter-out $(MAIN), $(OBJS)) $(PATHO)unity.o
	$(LINK) -o $@ $^

$(PATHO)%.o : $(PATHT)%.c
	$(COMPILE) $< -o $@

$(PATHO)unity.o : unity/unity.c
	$(COMPILE) $< -o $@

# Run tests
RESULTS = $(patsubst $(PATHT)test_%.c, $(PATHR)test_%.txt, $(SRCT))

test : $(RESULTS)
	@echo "$(RESULTS)"
	@echo "----------\nIGNORES:\n----------"
	@echo `grep -s IGNORE $(PATHR)*.txt`
	@echo "----------\nFAIL:\n----------"
	@echo `grep -s FAIL $(PATHR)*.txt`
	@echo "\nDONE"

$(PATHR)test_%.txt : $(PATHB)test_%
	-./$< > $@ 2>&1


# Create necessary directories
$(PATHB) : 
	mkdir -p $(PATHB)

$(PATHO) : 
	mkdir -p $(PATHO)

$(PATHR) :
	mkdir -p $(PATHR)


# Cleanup: no need to rm $(OBJT) since these files are apparently intermediaries and auto-removed
clean :
	rm $(PATHB)$(OUTFILE)
	rm $(PATHB)test_utils
	rm $(PATHO)unity.o
	rm $(OBJS)
	rm $(RESULTS)
