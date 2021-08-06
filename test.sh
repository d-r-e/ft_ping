#!/bin/bash
BIN=ft_ping
make

./$BIN -o google.es
./$BIN -o 8.8.8.8
./$BIN -o localhost
./$BIN -o 192.168.1.1
./$BIN -o -v elpais.com
echo


su darodrig -c "~/ft_ping/ft_ping -o google.es"
su darodrig -c "~/ft_ping/ft_ping -o 8.8.8.8"
su darodrig -c "~/ft_ping/ft_ping -o localhost"
su darodrig -c "~/ft_ping/ft_ping -o 192.168.1.1"
su darodrig -c "~/ft_ping/ft_ping -o -v elpais.com"
exit