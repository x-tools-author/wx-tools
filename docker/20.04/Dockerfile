FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive && apt-get -y update && apt-get install -y \
    ninja-build \
    automake \
    libtool \
    libgtk-3-dev \
    pkg-config \
    software-properties-common \
    coreutils \
    make \
    pkg-config \
    xvfb \
    fuse \
    libfuse2 \
    build-essential \
    libfuse-dev \
    wget \
    git
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

# CMake
RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh
RUN chmod +x cmake-3.30.8-linux-x86_64.sh
RUN ./cmake-3.30.8-linux-x86_64.sh --skip-license --prefix=/usr/local
RUN rm cmake-3.30.8-linux-x86_64.sh
