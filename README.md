## DGS

### Distributed Multiplayer Game System

- The Distributed Game System was a final project for ECE 3574. The project demonstrated the use of a common C++ framework used to create networked game system
- The project contains a server that handles game object storage as well as game status information
- Each client has the ability to operate as a game master, with additional clients joining the game which supports up to 4 simultaneous players


![Client GUI](screenshots/multi_client.jpg?raw=true "Client GUI")
![Server GUI](screenshots/server_gui.jpg?raw=true "Client GUI")

### Core Technical Concepts

- The server is a multithreaded TCP server, with a new thread created for each incomming connection. Threads are destroyed upon client disconnection
- The client uses standard TCP communication to connect to the server. Up to 4 clients can run simultaneously and play the game, which is a modified version of "snake"
- The client utilizes various QT graphics and GUI libraries to draw the game board and provide game control
- All communication between the clients and server is contained in JSON messages that have predefined key-value pairs for required fields

### Dependencies
- Requires QT 5.7 or later


### Contact
- mdykshorn1@gmail.com