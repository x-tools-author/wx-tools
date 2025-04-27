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
    git \
    libxcb-xinerama0 \
    libxcb-cursor-dev \
    libudev-dev \
    ninja-build \
    libopengl-dev \
    libglu1-mesa-dev \
    mesa-common-dev \
    wget \
    p7zip-full \
    libicu-dev \
    tree \
    grep \
    git \
    libglib2.0-dev \
    libxkb* \
    libdbus* \
    libegl1-mesa-dev \
    libfontconfig1-dev  \
    libfreetype6-dev  \
    libfreetype-dev\
    unzip \
    kmod
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

# CMake
RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh
RUN chmod +x cmake-3.30.8-linux-x86_64.sh
RUN ./cmake-3.30.8-linux-x86_64.sh --skip-license --prefix=/usr/local
RUN rm cmake-3.30.8-linux-x86_64.sh
