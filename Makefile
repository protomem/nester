PROJECT := nester


.PHONY: configure
configure:
	@echo
	@echo "---"
	@echo "Configuring..."
	@echo "---"
	@premake5 gmake2 --file=premake5.lua
	@cd build && compiledb make


.PHONY: build
build:
	@echo
	@echo "---"
	@echo "Building..."
	@echo "---"
	@cd build && make


.PHONY: test
test: build
	@echo
	@echo "---"
	@echo "Testing..."
	@echo "---"
	@./build/Debug/nester-tests


.PHONY: clean
clean:
	@echo
	@echo "---"
	@echo "Cleaning..."
	@echo "---"
	@cd build && make clean
	@rm -rf build
