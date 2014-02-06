#!/bin/sh
#
# Raspberry Pi Video Prank Script
# Use chmod +x ~/autorun.sh to enable.

case "$1" in
   *) # Normal start up of autorun.sh script
 
     mjpg_streamer -b -i "input_uvc.so -r 640x480 -f 30" -o "output_http.so -w /usr/www" # Start our webcam stream
     sudo su -l -c  "/usr/bin/python /home/pi/sound3.py" pi & 
    ;;
esac
