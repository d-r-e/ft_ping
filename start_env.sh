#!/bin/sh
set -epu

docker build -t ft_ping .
docker run -it ft_ping e MACHINE=$(uname -n | cut -d. -f1)