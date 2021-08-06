FROM debian:stretch
LABEL maintainer darodrig
RUN apt update
RUN apt upgrade -y
RUN apt install -y gcc make watch git man
RUN apt install -y valgrind
RUN apt install -y inetutils-ping
RUN useradd --home /home/darodrig -p $MACHINE darodrig
RUN usermod -aG sudo darodrig
RUN chmod -R g+rwx /ft_ping/*
USER darodrig
WORKDIR /ft_ping
ENV MACHINE "$(uname -n)"
RUN git clone https://github.com/d-r-e/ft_ping.git .
# RUN make
# RUN make clean
CMD [ "/bin/bash"]

