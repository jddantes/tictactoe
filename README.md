tictactoe
=========

TicTacToe for a Server/Client

Runs in Linux. Uses header files like sys/socket.h which would be located in /usr/include on a Linux machine.

USAGE

Server-side:
  Compile and run like any other C program.
  
Client-size:
  On a terminal:
    telnet <ip.address.of.server> <port>
  The port number is defined in the source file to be 7890
  
Positions are numbered like this:
  1 2 3
  4 5 6
  7 8 9

LOG
  v1
    Turn starts with the server (turn indicated by "Your turn" on top of the board).
    Checks if position is occupied, but not if a pattern is made.
    Game ends when board is filled.
    
