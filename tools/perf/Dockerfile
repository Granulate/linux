FROM ubuntu:xenial-20200619

WORKDIR /code
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    curl \
    autoconf \
    asciidoc \
    libssl-dev \
    zlib1g-dev \
    libaudit-dev \
    binutils-dev \
    libiberty-dev \
    libcap-dev \
    libdwarf-dev \
    liblzma-dev \
    libnuma-dev \
    libbabeltrace-ctf-dev \
    systemtap-sdt-dev \
    libslang2-dev \
    libzstd-dev \
    libbz2-dev \
    flex \
    bison \
    ;

# Installing a new version of elfutils and libunwind
RUN curl -L ftp://sourceware.org/pub/elfutils/0.179/elfutils-0.179.tar.bz2 -o /tmp/elfutils-0.179.tar.bz2
RUN cd /tmp && tar -xf elfutils-0.179.tar.bz2
RUN cd /tmp/elfutils-0.179 && ./configure --disable-debuginfod --prefix=/usr && make && make install

RUN curl -L http://download.savannah.nongnu.org/releases/libunwind/libunwind-1.4.0.tar.gz -o /tmp/libunwind-1.4.0.tar.gz
RUN cd /tmp && tar -xf libunwind-1.4.0.tar.gz
RUN cd /tmp/libunwind-1.4.0 && ./configure --prefix=/usr && make install

CMD bash
