# photon-boardroom-beacon
The boardroom beacon is a physical device mounted to the exterior wall of a boardroom. It has 3 lights indicating meeting room availability in half hour allotments. The first light indicates the boardroom status for the current half hour, the second indicates the next half hour and the third, the half hour after that. So any casual observer can see what the boardroom’s status is, for the next hour and a half. Statuses include “available” (green), “booked” (red) and “illegal” (yellow). An illegal status indicates that a person is in the room but it is not booked. Malfunction will be indicated by all three lights flashing yellow.
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