FROM debian:bullseye
LABEL maintainer darodrig
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y vim gcc make watch git man
RUN apt-get install -y valgrind
RUN apt-get install -y inetutils-ping libcap2-bin tcpdump traceroute
RUN useradd darodrig
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
ENTRYPOINT [ "/bin/bash"]

