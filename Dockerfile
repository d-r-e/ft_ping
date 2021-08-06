FROM debian:stretch
LABEL maintainer darodrig
RUN apt update
RUN apt upgrade -y
RUN apt install -y vim gcc make watch git man
RUN apt install -y valgrind
RUN apt install -y inetutils-ping libcap2-bin
RUN useradd --home /home/darodrig -p $MACHINE darodrig
RUN usermod -aG sudo darodrig
WORKDIR /ft_ping
ENV MACHINE "$(uname -n)"
RUN mkdir -p /ft_ping
RUN mkdir -p /home/darodrig
RUN chmod -R 775 /ft_ping
RUN ln -s /ft_ping /home/darodrig
#USER darodrig
RUN git clone https://github.com/d-r-e/ft_ping.git .
#USER root
RUN make
RUN make clean
CMD [ "/bin/bash"]

