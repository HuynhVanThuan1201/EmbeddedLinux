Hướng dẫn running file

1. giao thức IPv4 stream socket (DCP)
./IPv4_stream_socket_server <server_port>
./IPv4_stream_socket_client <ip_server_address> <server_port>

Có thể kiểm tra ip và port server bằng lệnh command line sau:
--------
ifconfig
--------

2. giao thức IPv4 datagram socket (UDP)
./IPv4_datagram_socket_server <server_port>
./IPv4_datagram_socket_client <ip_server_address> <server_port>

3. giao thức Unix stream socket (DCP)
./Unix_stream_socket_server
./Unix_stream_socket_client

4. giao thức Unix datagram socket (DCP)
./Unix_datagram_socket_server
./Unix_datagram_socket_client
