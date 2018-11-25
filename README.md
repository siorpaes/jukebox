# Jukebox

This simple project is meant to drive [VLC](https://www.videolan.org/vlc/index.html) using a numeric keypad.
Uses hidraw to acquire and decode input data from keypad.
Uses STDOUT (or TCP/IP with netcat) to communicate to VLC running in server mode.

## How to install and run on a Raspberry board
Install vlc and netcat

Edit /etc/rc.local and add the following line to it to launch VLC at startup

runuser -l pi -c "/usr/bin/vlc --intf rc --rc-host :50000 &"
sleep 5
/home/pi/jukebox/jukebox /home/pi/music | /bin/nc localhost 50000 &

