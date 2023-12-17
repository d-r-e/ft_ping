FROM debian:bullseye
RUN apt-get update && apt-get install -yq inetutils-ping netcat traceroute git gcc make
