all: build
	@echo ""
	@echo "Done!"

build:
	@echo "-------------------- Configure and Build CMake -----------"
	cmake -S . -B build
	cmake --build build -- -j4
	@echo ""

test: build
	@echo "-------------------- Run CTest ---------------------------"
	cd build && pwd && ctest --verbose
	@echo ""

coverage:
	@echo "-------------------- Build Coverage--------------------------"
	cmake -DENABLE_COVERAGE=ON -S . -B build
	cmake --build build --config Debug --target coverage -j4
	@echo ""

doxygen: build
	@echo "-------------------- Build Coverage--------------------------"
	cmake --build build --config Debug --target docs -j4
	@echo ""

gtest_report:
	cd build-artifacts/gtest_report && xsltproc gtest2html.xslt out/*.xml > gtest_report.html
	# cd report && xsltproc gtest2html.xslt *.xml > gtest_report.html
	# cd report && xsltproc test.xslt *.xml > gtest_report.html
	# cd report && xsltproc newgtest2html.xsl *.xml > gtest_report.html
# Don't work!!!!
# report:
# 	@echo "-------------------- Coverage Report ---------------------"
# 	lcov --capture --directory build/coverage --output-file coverage.info
# 	genhtml coverage.info --output-directory test/
# 	@echo ""

dependency:
	@echo "-------------------- Create Graph Dependecy --------------"
	cd build && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graph_image.png
	@echo ""

clean:
	@echo ""
	@echo "-------------------- Clean build folder ------------------"
	rm -rf build
	@echo ""