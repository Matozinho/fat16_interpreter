FROM archlinux:base

# Install dependencies (clang, xmake, git, make, cmake, ninja)
RUN pacman -Syu --noconfirm --needed clang xmake git make cmake ninja unzip && \
  useradd -m -G wheel -s /bin/bash cppdev

USER cppdev
WORKDIR /home/cppdev

# Install 
RUN xrepo add-repo gnussy https://github.com/gnussy/gnussy-repos
RUN xrepo install -y fmt gtest prepucio cxxopts

# Copy the project files
COPY --chmod=0755 --chown=cppdev:cppdev . .
# RUN rm -rf build .cache .xmake

RUN xmake clean -a 
RUN xmake build -a
