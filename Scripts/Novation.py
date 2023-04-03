
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
#
# Launchkey Mini II Sweep Animation
# by David Madison © 2018
# www.partsnotincluded.com
#
# This is a sweep animation to demonstrate control of the Novation Launchkey Mini
# II's RG LED pads. The script will generate a random color from the LED palette
# that sweeps from left to right across the available LED pads.
#
 
 import mido
 import random
 from time import sleep
  
  row1 = (96, 97, 98, 99, 100, 101, 102, 103, 104)  # LED indices, first row
  row2 = (112, 113, 114, 115, 116, 117, 118, 119, 120)  # LED incides, second row
  leds = row1 + row2  # LED indices, combined
   
    
    def write_led(led_id, color_vel):
    	midi_out.send(mido.Message('note_on', channel=0, note=led_id, velocity=color_vel))
	 
	  
	  def random_color():
	  	red_or_green = bool(random.randint(0, 1))  # Making sure the number is always >0 for one component
			return random.randint(int(red_or_green), 3) + \
					   random.randint(int(not red_or_green), 3) * 16
					    
					     
					     if __name__ == "__main__":
					     	try:
								midi_out = mido.open_output("MIDIOUT2 (Launchkey Mini) 2")  # Launchkey InControl port
										midi_out.send(mido.Message.from_bytes([0x90, 0x0C, 0x7F]))  # Switch to "InControl" mode
											
													while True:
																color = random_color()
																			
																						for index, led in enumerate(row1):
																										# Set current LED color
																														write_led(row1[index], color)
																																		write_led(row2[index], color)
																																						
																																										# Turn off last set LEDs
																																														write_led(row1[index - 1], 0)
																																																		write_led(row2[index - 1], 0)
																																																						sleep(0.1)
																																																						 
																																																						 	except KeyboardInterrupt:
																																																									pass
																																																									 
																																																									  
																																																									  for led in leds:
																																																									  	write_led(led, 0)  # Turn off all LEDs
																																																										midi_out.send(mido.Message.from_bytes([0x90, 0x0C, 0x00]))  # Disable "InControl" mode
																																																										midi_out.close()
