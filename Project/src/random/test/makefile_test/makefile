TARGETS += clean

main:
	-@cd tmp && make

all: $(TARGETS) main

$(TARGETS):
	-@cd tmp && make $@

.PHONY: all main $(TARGETS)
