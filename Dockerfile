FROM debian:stretch
LABEL maintainer darodrig
RUN apt update
RUN apt upgrade -y
RUN apt install -y gcc make watch git man
RUN apt install -y iputils-ping
WORKDIR /ft_ping
RUN git clone https://github.com/d-r-e/ft_ping.git .
# RUN make
# RUN make clean
CMD [ "/bin/bash"]

