# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.158.0/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version: bionic, focal
ARG VARIANT="focal"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# [Optional] Uncomment this section to install additional OS packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y install --no-install-recommends g++ gdb make cmake

# Google Test
RUN apt-get -y install libgtest-dev && \
    cd /usr/src/gtest && \
    cmake CMakeLists.txt && \
    make && \
    cp lib/libgtest.a lib/libgtest_main.a /usr/lib

ENV GCC_COLORS "error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01"