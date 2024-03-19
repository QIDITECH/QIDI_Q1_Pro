#!/bin/bash
echo "Start makerbase-client"
time=$(date "+%Y%m%d%H%M%S")
# /root/makerbase-client/build/MakerbaseClient localhost > /root/mksclient/test-$time.log
/root/udp_server &
/root/xindi/build/xindi localhost 
