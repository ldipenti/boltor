ARG base_tag=bookworm
ARG base_img=mcr.microsoft.com/vscode/devcontainers/base:dev-${base_tag}

FROM --platform=linux/arm64 ${base_img} AS builder-install

RUN apt-get update --fix-missing && apt-get -y upgrade
RUN apt-get install -y --no-install-recommends \
    apt-utils \
    curl \
    cmake \
    build-essential \
    gcc \
    gdb \
    clang-format-16 \
    clang-tidy-16 \
    locales \
    make \
    ruby \
    gcovr \
    wget \
    git \
    libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
    && apt-get -y --purge autoremove
RUN ln -s /usr/bin/clang-format-16 /usr/bin/clang-format && \
    ln -s /usr/bin/clang-tidy-16 /usr/bin/clang-tidy
RUN cd /usr/src && \
    git clone https://github.com/raysan5/raylib.git raylib && \
    cd raylib && \
    mkdir build && cd build && \
    cmake .. && \
    make && \
    make install

VOLUME ["/builder/mnt"]
WORKDIR /builder/mnt