# Jukebox

This simple project is meant to drive [VLC](https://www.videolan.org/vlc/index.html) using a numeric keypad.
Uses hidraw to acquire and decode input data from keypad.
Uses STDOUT (or TCP/IP with netcat) to communicate to VLC running in server mode.

## Usage
Create a root directory where all media files to played are located
(e.g. /home/pi/music). Put all contents in such directory organizing them in
sub-directories with numeric names (e.g. 1, 45, 345,...). Use symbolic links
for convenience.
To play all media contained in one of the directories, just enter the
corresponding number with the keypad and press Enter.
If pressing Enter twice, the playback is stopped and playlist is cleared.
Use the other keypad keys for controlling volume (+, -), fast forward (numlock), and playlist navigation (/, *), pause (.).

## How to install and run on a Raspberry board
Install vlc and optionally netcat. 

Edit /etc/rc.local and add the following line to it to launch VLC at startup

/home/pi/jukebox/jukebox /home/pi/music | runuser -l pi -c "/usr/bin/vlc" &

If using netcat, add instead:
runuser -l pi -c "/usr/bin/vlc --intf rc --rc-host :50000 &"
sleep 5
/home/pi/jukebox/jukebox /home/pi/music | /bin/nc localhost 50000 &

