PATHS = src/
PATHB = build/
PATHO = build/objs/
PATHR = build/results/
PATHT = test/

.PHONY : test
.PHONY : clean
.PHONY : translate

OUTFILE = vmtranslator
BUILD_PATHS = $(PATHB) $(PATHO) $(PATHR)

COMPILE = cc -c
LINK = cc

SRC = $(wildcard $(PATHS)*.c)  # src/main.c src/parser.c ...
SRCT = $(wildcard $(PATHT)*.c) # test/test_utils.c test/test_parser.c ...

OBJS = $(patsubst $(PATHS)%.c, $(PATHO)%.o, $(SRC))  # build/objs/main.o build/objs/utils.o ...
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

# Assembler sample .vm projects
VM_DIR = vm # vm
VM_DIR_CONTENTS = $(shell find $(VM_DIR) -maxdepth 1 -type d) # vm vm/BasicLoop vm/FibonacciElements ...
VM_PROJ_DIRS = $(filter-out $(VM_DIR), $(VM_DIR_CONTENTS)) # vm/BasicLoop vm/FibonacciElements ...
ASM_TARGETS = $(join $(addsuffix /, $(VM_PROJ_DIRS)), $(addsuffix .asm, $(notdir $(VM_PROJ_DIRS))))


translate : $(ASM_TARGETS)


vm/BasicLoop/BasicLoop.asm : vm/BasicLoop/BasicLoop.vm
	-./$(PATHB)$(OUTFILE) -b $@ $^

vm/StaticsTest/StaticsTest.asm : vm/StaticsTest/Class1.vm vm/StaticsTest/Class2.vm vm/StaticsTest/Sys.vm
	-./$(PATHB)$(OUTFILE) $@ $^

vm/SimpleFunction/SimpleFunction.asm : vm/SimpleFunction/SimpleFunction.vm
	-./$(PATHB)$(OUTFILE) -b $@ $^

vm/FibonacciSeries/FibonacciSeries.asm : vm/FibonacciSeries/FibonacciSeries.vm
	-./$(PATHB)$(OUTFILE) -b $@ $^

vm/FibonacciElement/FibonacciElement.asm : vm/FibonacciElement/Main.vm vm/FibonacciElement/Sys.vm
	-./$(PATHB)$(OUTFILE) $@ $^

vm/NestedCall/NestedCall.asm : vm/NestedCall/Sys.vm
	-./$(PATHB)$(OUTFILE) $@ $^
	

# Cleanup: no need to rm $(OBJT) since these files are apparently intermediaries and auto-removed
clean :
	rm $(PATHB)$(OUTFILE)
	rm $(TESTS)
	rm $(PATHO)unity.o
	rm $(OBJS)
	rm $(RESULTS)
	rm $(ASM_TARGETS)
