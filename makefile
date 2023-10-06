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

# Assembler sample .vm projects
VM_DIR = vm # vm
VM_DIR_CONTENTS = $(shell find $(VM_DIR) -maxdepth 1 -type d) # vm vm/BasicLoop vm/FibonacciElements ...
VM_PROJ_DIRS = $(filter-out $(VM_DIR), $(VM_DIR_CONTENTS)) # vm/BasicLoop vm/FibonacciElements ...
ASM_TARGETS = $(join $(addsuffix /, $(VM_PROJ_DIRS)), $(addsuffix .asm, $(notdir $(VM_PROJ_DIRS))))

#VM_PROJ_PATH = $(addsuffix /, $(filter-out $(VM_DIR), $(VM_DIR_CONTENTS)))  # vm/BasicLoop/ vm/FibonacciElements/ ...
#ASM_TARGETS = $(addsuffix /out.asm, $(VM_PROJ_DIRS))
#PROJS = $(notdir $(VM_PROJ_DIRS))
#ASM_TARGETS = $(patsubst %/, %/$(notdir %).asm, $(dir $(wildcard vm/*/)))


#define generate_asm =
#$(1) : $(wildcard $(dir $(1))*.vm)
#endef

#VM_FILES = $(foreach asm_target, $(ASM_TARGETS), $(wildcard $(dir $(asm_target))*.vm))
#$(foreach asm_target, $(ASM_TARGETS), $(eval $(call generate_asm, $(asm_target))))

#get_vm_files = $(shell find vm/$(1) -type f -maxdepth 1 -name *.vm)

#$(ASM_TARGETS) :
	#@echo "$@ -> $^\n"

translate : $(ASM_TARGETS)

vm/BasicLoop/BasicLoop.asm : vm/BasicLoop/BasicLoop.vm
	-./$(PATHB)$(OUTFILE) $@ $^

vm/StaticsTest/StaticsTest.asm : vm/StaticsTest/Class1.vm vm/StaticsTest/Class2.vm vm/StaticsTest/Sys.vm
	-./$(PATHB)$(OUTFILE) $@ $^

vm/SimpleFunction/SimpleFunction.asm : vm/SimpleFunction/SimpleFunction.vm
	-./$(PATHB)$(OUTFILE) $@ $^

vm/FibonacciSeries/FibonacciSeries.asm : vm/FibonacciSeries/FibonacciSeries.vm
	-./$(PATHB)$(OUTFILE) $@ $^

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
