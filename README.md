# Multi-junction traffic light controller 
The goal of the class project is to give you experience in Real-Time Embedded System Design. The design experience will utilize one of the prominent processor technologies, namely, the application specific instruction set processors, or microcontrollers. The TivaC board will be the hardware platform for the project implementation and the FreeRTOS will be used to manage the concurrent tasks.

This course project is to design a Traffic Light controller with four road junctions of different priorities, with parallel railroad crossing. The railroad goes east/west and has the highest priority. The north/south road junctions have higher priority than the east/west roads. Single lane railroad crossing has a gate, which is vertical by default, and closes before train approaches. Once the gate is about to close, all four red lights go flashing, (and with a bonus, a siren sounds is turned on) until the train passes. Pedestrians can also provide requests to cross, at any of the four corners. System can operate at various modes as specified later. See layout below of the crossing:


Each road junction has its timer for the Go (green light), followed by Stop time (sum of the Go times for the other junctions), when other directions can Go. As stop time expires, Green light of the corresponding junction goes green, and all other opposite directions must be red. A green light should be followed by a yellow light before it turns red, but this feature will be removed for simplicity. Also, for simplicity, assume that North and South Go together, as well as East and West Go together. Algorithm for calculating the order in which the Red lights have to glow is decided by the priority of the corresponding junction. 

# Operating modes: 

# Normal mode: 
This is the normal mode. As spefied in the simplification features, either the North/South Go or the East/West Go, with parametric times given at compile time: tgn=tgs, tge=tgw in this order. For example, if these values are 5, 5, 2.5, 2.5, then the Go times for the North/South will be 5 time units, and stop for 2.5 time units (seconds, minutes). Thus, the priority is higher for the North/South directions. 

# Pedestrian crossing/Hazard mode: 
This is an as needed mode. Four PD buttons are provided at each of the four corners, to provide a single signal. When this signal is asserted, the current junction that is going waits for its remaining time, then all stop lights should be turned red for time equal to compile time parameter: tcross. After that time, priority mode should be resumed. Same action can be done when an emergency vehicle approaches. All flows should be stopped. 

# Train approaching: 
This is the highest priority mode. Two train approaching sensors (simulated by an on/off switch), are placed at either side of the rail road to provide a single signal 'Train'. If this signal is active, the current junction that is actually going should stop, then all stop lights should be flashing red. Then the train gate should close (simulated by turning on a RED light, or for the bonus option use a servo motor and attach a gate bar to move from vertical to horizontal position, also for a second bonus turn on a siren). The distance between the two train sensors should be longer than the longest train. So, when the second train sensor is triggered it should be assumed safe to open the gate and resume the normal mode.
