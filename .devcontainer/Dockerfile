FROM ubuntu:26.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    sudo \
    ca-certificates \
    wget \
    curl \
    unzip \
    git \
    make \
    cmake \
    doxygen \
    gcc-multilib \
    gdb-multiarch \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    picolibc-arm-none-eabi \
    clang \
    clangd \
    clang-tidy \
    usbutils \
    python3-serial \
    python3-psutil \
    openocd \
    fish \
    && rm -rf /var/lib/apt/lists/* \
    && chsh -s /usr/bin/fish ubuntu \
    && echo "ubuntu ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/ubuntu \
    && chmod 0440 /etc/sudoers.d/ubuntu

ENV PATH="/usr/lib/arm-none-eabi/bin:${PATH}"
WORKDIR /workspace
USER ubuntu
