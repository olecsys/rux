#!/bin/bash
 
#Replace with your actual VPN server IP
server=192.168.90.1
ping -w 2 -c 1 $server >/dev/null 2>&1
if [ ! $? = 0 ]; then
	service openvpn stop >/dev/null 2>&1
	killall openvpn # just to be REALLY CERTAIN
	sleep 5
	service openvpn start >/dev/null 2>&1
	sleep 5
fi