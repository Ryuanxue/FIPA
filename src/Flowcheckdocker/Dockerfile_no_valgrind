# 使用官方的 Ubuntu 20.04 基础镜像
FROM ubuntu:18.04

# 设置环境变量，以避免在构建过程中出现交互提示
ENV DEBIAN_FRONTEND=noninteractive

# 设置时区（避免在安装过程中出现时区选择提示）
ENV TZ=UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 更新软件包列表并安装基本的工具和 Python 3
RUN dpkg --add-architecture i386 && \
    apt-get update && apt-get install -y \
    libc6:i386 \
    libstdc++6:i386 \
    libgcc1:i386 \
    libstdc++6:i386 \
    linux-libc-dev:i386 \
    gcc-multilib \
    g++-multilib \
    python3 \
    python3-pip \
    libtool \
    gettext \
    autoconf \
    automake \
    autopoint \
    libselinux1-dev:i386 \
    libsemanage1-dev:i386 \
    pkg-config:i386 \
    libgnutls28-dev:i386 \
    zlib1g-dev:i386 \
    libpsl-dev:i386 \
    build-essential \
    libc6-dbg \
    libc6-dbg:i386 \
    vim \
    rpcbind \
    && rm -rf /var/lib/apt/lists/*

# 复制FlowCheck源代码目录到容器（假设flowcheck-1.20与Dockerfile同级）
#COPY flowcheck-1.20 /flowcheck

# 编译 FlowCheck
#WORKDIR /flowcheck/valgrind-r15939+flowcheck
#RUN ./configure --prefix=/flowcheck --enable-only32bit && \
#    make && \
 #   make install && \
  #  make clean

# 切换工作目录
WORKDIR /Desktop

# 暴露应用运行的端口（例如，假设应用使用端口 5000）
EXPOSE 5000

# 设置环境变量
#ENV PATH="/flowcheck/bin:${PATH}"
#ENV VALGRIND_LIB=/flowcheck/lib/valgrind

# 默认进入 bash
CMD ["bash"]
