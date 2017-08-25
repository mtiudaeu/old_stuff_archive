# Build docker :
# docker build -t cpp_simple .
#
# Usage :
# Proposed alias name : dockerCpp
# docker run -ti --rm -v `pwd`:/srv -w /srv cpp_simple bash

FROM debian
RUN apt-get update && apt-get install -y \
  #Install GCC
  build-essential \
  #Install google test
  libgtest-dev \
  cmake && \
  cd /usr/src/gtest && \
  cmake . && \
  make && \
  mv libg* /usr/lib/

