#!/bin/bash
docker build . -t ft_ping
if [ "${1}" == "--no-icmp" ]; then
    docker run -e MACHINE=$(uname -n | cut -d. -f1) -it --sysctl net.ipv4.icmp_echo_ignore_all=1 ft_ping
else
    docker run -e MACHINE=$(uname -n | cut -d. -f1) -it ft_ping
fi
