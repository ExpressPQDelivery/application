Welcome to the ExpressPQDelivery Project
==============================
The ExpressPQDelivery is a project that provides example servers and clients that perform ExpressPQDelivery-applied PQC-TLS1.3 handshake using epqd_client_lib and epqd_server_lib.
epqd_client_lib (github.com/ExpressPQDelivery/epqd_client_lib) and epqd_server_lib (github.com/ExpressPQDelivery/epqd_server_lib) are libraries that implements ExpressPQDelivery handshake based on OpenSSL.

# How to compile
> make all

# How to run 
> ./server [port] [algorithm]
> ./client [domain_address] [port] [algorithm]
algorithm: DILITHIUM2: dil2, DILITHIUM3: dil3, DILITHIUM5: dil5,
FALCON512: fal512, FALCON1024: fal1024
# Prerequisite
> intstall github.com/ExpressPQDelivery/epqd_client_lib for client
> intstall github.com/ExpressPQDelivery/epqd_server_lib for server

# TroubleShooting
1. add environment variables
export LD_LIBRARY_PATH=/usr/local/lib64
