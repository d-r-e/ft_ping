#!/bin/bash
BIN=ft_ping
make

./$BIN -o google.es
./$BIN -o 8.8.8.8
./$BIN -o localhost
./$BIN -o 192.168.1.1
./$BIN -o -v elpais.com