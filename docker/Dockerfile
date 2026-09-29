# Docker name 
# charlesdias/stm32_gtest
#
# Docker build
# docker build -t charlesdias/stm32_gtest:<version> -t charlesdias/stm32_gtest:latest .
#
# Docker push
# docker push charlesdias/stm32_gtest --all-tags

FROM ubuntu:24.04

LABEL maintainer="Charles Dias <charlesdias@tecnofuturo.com.br>"

# Avoid interactive prompts (e.g. tzdata) during the image build only
ARG DEBIAN_FRONTEND=noninteractive

ENV TZ=America/Fortaleza
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Download Linux support tools
RUN apt-get update && apt-get clean && apt-get install -y \
      build-essential git wget python3 doxygen graphviz gcovr lcov \
      python3-venv \
      && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install Lizard, Flawfinder and junit2html (HTML test report) in a virtual
# environment, since Ubuntu 24.04 blocks system-wide pip installs (PEP 668).
# Only the three tools go on the PATH.
RUN python3 -m venv /opt/venv \
      && /opt/venv/bin/pip install --no-cache-dir lizard==1.24.0 flawfinder==2.0.20 junit2html==31.1.4 \
      && ln -s /opt/venv/bin/lizard /opt/venv/bin/flawfinder /opt/venv/bin/junit2html /usr/local/bin/

# Get and install the CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.24.2/cmake-3.24.2-linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh

# Set up the CMake path
ENV PATH="/usr/bin/cmake/bin:${PATH}"

# Build and install Cppcheck from source, since Ubuntu 24.04 only ships 2.13.
# Its MISRA addon (misra.py) runs with the system python3.
RUN wget https://github.com/danmar/cppcheck/archive/refs/tags/2.22.0.tar.gz \
      -q -O /tmp/cppcheck.tar.gz \
      && tar -xzf /tmp/cppcheck.tar.gz -C /tmp \
      && cmake -S /tmp/cppcheck-2.22.0 -B /tmp/cppcheck-build -DCMAKE_BUILD_TYPE=Release -DUSE_MATCHCOMPILER=ON \
      && cmake --build /tmp/cppcheck-build -j"$(nproc)" \
      && cmake --install /tmp/cppcheck-build \
      && rm -rf /tmp/cppcheck.tar.gz /tmp/cppcheck-2.22.0 /tmp/cppcheck-build

# # Clone and install Google Test framework. Install in /usr/local/ by default
# RUN git clone -b v1.12.1 https://github.com/google/googletest.git /googletest \
#     && cd /googletest \
#     && mkdir -p build && cd build \
#     && cmake .. && make && make install \
#     && cd / && rm -rf /googletest

# Set up the working directory
WORKDIR /home/project