#!/bin/bash
docker build . -t ft_ping
docker run -e MACHINE=$(uname -n | cut -d. -f1) -it ft_ping