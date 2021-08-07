#!/bin/bash
BIN=ft_ping
 make

./$BIN -c 1 google.es
./$BIN -c 1 8.8.8.8
./$BIN -c 1 localhost
./$BIN -c 1 192.168.1.1
./$BIN -c 1 -v elpais.com

exit

/*
 * Old tests (without root) (not mandatory)
*/


su darodrig -c "./ft_ping -c 2 google.es"
su darodrig -c "./ft_ping -c 2 8.8.8.8"
su darodrig -c "./ft_ping -c 2 localhost"
su darodrig -c "./ft_ping -c 2 192.168.1.1"
su darodrig -c "./ft_ping -c 2 elpais.com"
exit 0

#  internship 11.91
# drquine 12.22
# nm otool 12.66
# dslr
# linear regression
# snow crash
# ft ping
# rainfall
# multilayer perceptron