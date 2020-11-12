FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    git \
    binutils \
    libc6-dev \
    vim \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*
