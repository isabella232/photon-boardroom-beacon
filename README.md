# photon-boardroom-beacon
The boardroom beacon is a physical device mounted to the exterior wall of a boardroom. It has 3 lights indicating meeting room availability in half hour allotments. The first light indicates the boardroom status for the current half hour, the second indicates the next half hour and the third, the half hour after that. So any casual observer can see what the boardroom’s status is, for the next hour and a half. Statuses include “available” (green), “booked” (red) and “illegal” (yellow). An illegal status indicates that a person is in the room but it is not booked. Malfunction will be indicated by all three lights flashing yellow.
##Run
You will need to flash this code to your photon and use the component configurations in the diagram below. When the Photon first plugged in, all the LEDs should blink blue which means it is connected to wifi and waiting for a status message from Mosquitto. Beneath the schematic are instructions on how to send updates to the Photon. The 3D model for the enclosure is also included in this repo as a Sketchup file.
![Fritzing](/photon-boardroom-beacon_bb.png)
<table>
  <thead>
	<tr>
    <th>Amount</th>
    <th>Part Type</th>
    <th>Properties</th>
    </tr>
  </thead>
  <tbody>
<tr>
    <td>3</td>
    <td>RGB LED (com. cathode, rgb)</td>
    <td class="props">polarity common cathode; pin order rgb; package 5 mm [THT]; rgb RGB; part # LEDMU-999299</td>
</tr><tr>
    <td>1</td>
    <td>LEDs</td>
    <td class="props">variant 10mm; package led10mm</td>
</tr><tr>
    <td>1</td>
    <td>Photon</td>
    <td class="props">pins 24; variant variant 1; name Photon; version 1.0.0; manufacturer Particle</td>
</tr><tr>
    <td>1</td>
    <td>PowerBoost 500 Charger</td>
    <td class="props">variant variant 1; input connector JST; output connector USB</td>
</tr><tr>
    <td>1</td>
    <td>PIR sensor</td>
    <td class="props"></td>
</tr><tr>
    <td>9</td>
    <td>1kΩ Resistor</td>
    <td class="props">resistance 1kΩ; package 2010 [SMD]; tolerance ±5%</td>
</tr><tr>
    <td>1</td>
    <td>220Ω Resistor</td>
    <td class="props">resistance 220Ω; package 1206 [SMD]; tolerance ±5%</td>
</tr><tr>
    <td>1</td>
    <td>LIPO-1000mAh</td>
    <td class="props">variant 1000mAh; package lipo-1000</td>
</tr>
  </tbody>
</table>

##Communicate with the Boardroom-Beacon
This code is meant to work with [Mosquitto MQTT](https://mosquitto.org/) server and [Home Assistant](https://home-assistant.io/). For the photon to get the right calendar data, HASS uses a custom component and configuration [found here](https://github.com/Jam3/.homeassistant). If you don't want to install HASS you can use the Boardroom-Beacon without it by sending messages to Mosquitto through the command line.

###Install Mosquitto
[Download](https://mosquitto.org/download/)
Brew: `brew install mosquitto`

###Update the Mosquitto Configuration File
OSX: `/usr/local/sbin/mosquitto/mosquitto.conf`
Linux: `/etc/mosquitto/mosquitto.conf`

Add the following lines to the very end with your ip & port:
```
# Mosquittoo server IP & port
listener 8883 192.168.2.115
persistence true
persistence_location /var/lib/mosquitto/
persistence_file mosquitto.db
log_dest syslog
log_dest stdout
log_dest topic
log_type error
log_type warning
log_type notice
log_type information
connection_messages true
log_timestamp true
allow_anonymous true
# Add a password https://mosquitto.org/man/mosquitto_passwd-1.html
#password_file /usr/local/etc/mosquitto/pwfile
```
###Run Mosquitto
OSX:
`/usr/local/sbin/mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf`

Brew Background Service(OSX):
`brew services start mosquitto`

Linux:
`sudo mosquitto -v -c /etc/mosquitto/mosquitto.conf`

Background Service (Linux):
`sudo service mosquitto start`

###Publish and Subscribe
You may need to install `mosquitto_sub` & `mosquitto_pub` with Pip3
If you want to verify that Mosquitto is running properly and messages are being sent, you can use `mosquitto_sub`.

`mosquitto_sub -h <your-ip> -p 8883 -v -t 'homeassistant/boardroomBeacon/biggie/slot/0' -u <user> -P <password>`

Omit the user and password if you are not using a `pwfile` file.

Each LED subscribes to its own `slot` topic, `/slot/0`, `/slot/1` or `/slot/2`.
Publish to the status (booked, free, illegal) to the "biggie" boardroom:

`mosquitto_pub -h <your-ip> -p 8883 -t 'homeassistant/boardroomBeacon/biggie/slot/0' -m 'booked'`

The first LED should turn red.

`mosquitto_pub -h <your-ip> -p 8883 -t 'homeassistant/boardroomBeacon/biggie/slot/1' -m 'free'`
The second LED should turn green.

If you move around infront of the motion sensor the first LED should turn yellow.


####Subscribe
`mosquitto_sub -h 192.168.2.115 -p 8883 -v -t 'homeassistant/boardroomBeacon/biggie/slot/0'`
####Publish
`mosquitto_pub -h 192.168.2.115 -p 8883 -t 'homeassistant/boardroomBeacon/biggie/slot/0' -m 'free'`
####Publish wt Credentials
`mosquitto_pub -h 192.168.2.115 -p 8883 -t 'homeassistant/boardroomBeacon/biggie/slot/0' -u "<user>" -P "<password>" -m 'booked'`
