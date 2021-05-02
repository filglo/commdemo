## Application description
This application serves as demonstration for pipe based bi-directional inter-process communication on Linux.
## How to run
* Requires gcc (you can change compiler in makefile)
* Execute `make` command in the project folder
* Start two separate instances of application from shell with `./messenger`
## Usage
Two running application instances can communicate with each other by sending random messages of fixed size.  
Sending message in one application instance allows receiving that message in the second application instance.
There should always be exactly two instances running and in case one instance is closed, second one should be properly closed as well.
## Available commands
* `s` to send random message to the second application

    Message length and buffer size are defined in *buffer.h*  
    Also sends buffered messages
    If receiver is not connected, buffers the messages
* `g` to get all messages sent from the second application
* `h` to print help message
* `q` to quit
* **Confirm command by pressing enter**
