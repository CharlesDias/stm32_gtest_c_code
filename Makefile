all: build
	@echo ""
	@echo "Done!"

config:
	@echo "-------------------- Configure CMake ---------------------"
	cmake -S . -B build
	@echo ""

build: config
	@echo "-------------------- Build CMake--------------------------"
	cmake --build build --config Debug --target coverage -j4
	@echo ""

test:
	@echo "-------------------- Run CTest ---------------------------"
	cd build && pwd && ctest --verbose
	@echo ""

dependency:
	@echo "-------------------- Create Graph Dependecy --------------"
	cd build && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graph_image.png
	@echo ""

clean:
	@echo ""
	@echo "-------------------- Clean build folder ------------------"
	rm -rf build
	@echo ""