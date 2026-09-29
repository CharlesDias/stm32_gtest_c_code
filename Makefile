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

# Cppcheck with the MISRA C:2012 addon, limited to our code in Libraries/.
# The ST HAL and CMSIS headers are only included so cppcheck knows the HAL
# types; findings inside them are suppressed in cppcheck-suppressions.txt.
# __GNUC__ makes CMSIS pick its GCC code instead of stopping with an #error.
# It writes a text report and a SARIF file for GitHub code scanning; the
# second run reuses the results cached in build/cppcheck.
CPPCHECK_DIR = source/nucleo-f446ze-library
CPPCHECK_FLAGS = --std=c17 --platform=arm32-wchar_t4 --quiet --inline-suppr \
	--enable=warning,style,performance,portability --addon=misra \
	-D__GNUC__ -DSTM32F446xx -DUSE_HAL_DRIVER \
	-I $(CPPCHECK_DIR)/Libraries \
	-I $(CPPCHECK_DIR)/Inc \
	-I $(CPPCHECK_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc \
	-I $(CPPCHECK_DIR)/Drivers/CMSIS/Include \
	-I $(CPPCHECK_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	--suppressions-list=cppcheck-suppressions.txt \
	--cppcheck-build-dir=build/cppcheck

cppcheck:
	@echo "-------------------- Run Cppcheck with MISRA C:2012 ------"
	mkdir -p build/cppcheck
	cppcheck $(CPPCHECK_FLAGS) \
		--template='{file}:{line}:{column}: {severity}: {message} [{id}]\n{code}' \
		--output-file=build/cppcheck-report.txt $(CPPCHECK_DIR)/Libraries
	cppcheck $(CPPCHECK_FLAGS) --output-format=sarif \
		--output-file=build/cppcheck-results.sarif $(CPPCHECK_DIR)/Libraries
	@cat build/cppcheck-report.txt
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