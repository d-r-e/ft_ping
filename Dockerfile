FROM debian:bookworm-slim
RUN apt-get update && \
    apt-get install -yqq git make gcc inetutils-ping tcpdump locales man-db manpages && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
RUN sed -i '/es_ES.UTF-8/s/^# //g' /etc/locale.gen && \
    locale-gen

ENV LANG es_ES.UTF-8
ENV LANGUAGE es_ES:en
ENV LC_ALL es_ES.UTF-8
