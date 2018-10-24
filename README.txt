The programs in UDP and TCP work very similarly with few small changes.

The server programs will first set socket configurations using information from the command line, then wait for bytes to be received, and finally recursively return bytes for the correct output. The TCP server will also create a connection when one is found and close it when all of the output is sent, but the UDP program will not.

The client programs also work nearly identically: The client establishes configurations from the command line. The TCP connection will also try to establish a connection. The programs then send its user input, and wait for the server to send back only one character to be sure that all the output from the server is received. Once all the output from the server is received and printed, then the client closes the connection.

Other resources:
https://utcc.utoronto.ca/~cks/space/blog/unix/UDPConnectVsSendto
https://stackoverflow.com/questions/308695/how-do-i-concatenate-const-literal-strings-in-c
https://www.geeksforgeeks.org/strcpy-in-c-cpp/
https://stackoverflow.com/questions/13656702/sending-and-receiving-strings-over-tcp-socket-separately#
