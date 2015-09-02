# Patches welcome to actually make this portable

default: asmjs2wasm

OBJS     := main.o parser.o emitter.o wasm_ast.o cashew/parser.o
CFLAGS   += -g -std=c++11 -Wall -pedantic
SRCDIR   := src
OBJDIR   := obj
OBJFILES := $(addprefix $(OBJDIR)/, $(OBJS))

$(OBJDIR):
	mkdir $(OBJDIR)
$(OBJDIR)/cashew:
	mkdir -p $(OBJDIR)/cashew
$(OBJFILES): | $(OBJDIR) $(OBJDIR)/cashew

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	c++ -c $(CFLAGS) $< -o $@
asmjs2wasm: $(OBJFILES)
	c++ -g $(OBJFILES) -o asmjs2wasm

.PHONY: test
test: asmjs2wasm
	for testjs in test/*.js; do \
		test=$${testjs%.js}; \
		(cat $$testjs | asmjs2wasm | diff - $$test.wasm || \
	     (echo "Failure running: cat $$testjs | asmjs2wasm | diff - $$test.wasm" && false)) \
	done

.PHONY: clean
clean:
	rm -f asmjs2wasm
	rm -rf $(OBJDIR)
