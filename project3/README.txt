This project will simulate a Mars Rover for the purposes of exploring client/server communication using Sockets in C++.

Message Structure
----------------------------------
One byte ASCII ID - ASCII length - One byte ASCII length terminator (!) - payload

Messages
----------------------------------
ID	Purpose								Data
L	Sent by client: Tells server to turn left.			None
	Sent by server: Confirms and responds to turn command.		None
R	Sent by client: Tells server to turn right.			None
	Sent by server: Confirms and responds to turn command.		None 
P	Sent by client: Requests a picture.				None
	Sent by server: Sends the captured picture data.		.jpg file
D	Sent by client: Requests current direction.			None
	Sent by server: Sends current direction.			One byte ASCII character (N,S,E,W)
T	Sent by client: Requests air temperature.			None
	Sent by server: Sends current (random) temperature.		-50 to 50, so 2 or 3 bytes?
Q	Sent by client: Tells server to quit.				None
	(Never sent by server)
