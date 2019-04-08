# Traceroute #

Program displays addresses of routers on the path to destination IP address.

Use `make` command to compile with flags `-std=gnu99 -Wall -Wextra -Werror`.

Program sends ICMP packets with ttl value from 1 to 30 (3 packets for each ttl value). Then wait up to 1 second for response. Finally all IP adresses are shown with time clocked in miliseconds.

Program uses raw sockets - one has to run it with root previleges. Example:

`sudo ./traceroute 216.58.208.46`
