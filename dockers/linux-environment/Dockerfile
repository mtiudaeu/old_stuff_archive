# Build docker :
# docker build -t code_edit --build-arg git_email=<git_email> --build-arg git_name=<git_name> .
#
# Usage :  
# Proposed alias name : dockerEdit
# docker run -ti --rm --name code_edit -e DISPLAY=$DISPLAY -v /tmp/.X11-unix/:/tmp/.X11-unix -v `pwd`:/srv -w /srv code_edit bash
# Proposed alias name : dockerEditAttach
# docker exec -it code_edit bash

FROM debian

ARG git_email
ARG git_name

RUN apt-get update && apt-get install -y \
  tmux \
  git \
  clang-format-3.5 \
  build-essential \
  ncurses-dev \
  gettext && \
  cd /tmp && \
  git clone https://github.com/vim/vim.git && \
  cd vim/src && \
  make && \
  make install

RUN git config --global user.email $git_email && \
  git config --global user.name $git_name && \
  git config --global diff.tool vimdiff && \
  git config --global merge.tool vimdiff && \
  git config --global --add difftool.prompt false && \
  git config --global core.editor vim

COPY . /root
