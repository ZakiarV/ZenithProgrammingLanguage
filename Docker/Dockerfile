FROM ubuntu:latest
LABEL authors="alexfilippone"

ENTRYPOINT ["top", "-b"]

RUN apt-get update && apt-get install -y \
    nasm \
    build-essential \
    gdb \
    strace \
    ltrace \
    valgrind \
    && rm -rf /var/lib/apt/lists/*