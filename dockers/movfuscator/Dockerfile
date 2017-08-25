# Build docker :
# docker build -t movfuscator .
# Usage : Open container with movfuscator with access to your current directory. 
# docker run -ti -v `pwd`:/srv -w=/srv movfuscator bash

FROM debian

WORKDIR /tmp

RUN apt-get update && apt-get install -y \
      build-essential \
      git \
      libc6-dev-i386 &&\
    git clone https://github.com/xoreaxeaxeax/movfuscator && \
    cd movfuscator && \
    ./build.sh && \
    ./install.sh


