Welcome to the ExᴘʀᴇssPQDᴇʟɪᴠᴇʀʏ Project
==============================
- The ExᴘʀᴇssPQDᴇʟɪᴠᴇʀʏ is a project that provides example servers and clients that perform ExᴘʀᴇssPQDᴇʟɪᴠᴇʀʏ-applied PQC-TLS1.3 handshake using `epqd_client_lib` and `epqd_server_lib`.  
- [epqd_client_lib](https://github.com/ExpressPQDelivery/epqd_client_lib) and [epqd_server_lib](https://github.com/ExpressPQDelivery/epqd_server_lib) are libraries that implement ExᴘʀᴇssPQDᴇʟɪᴠᴇʀʏ handshake based on OpenSSL.

# How to compile
> make all

# How to run PQC-TLS1.3
./server [port] [algorithm]  
./client_tls [domain_address] [port] [algorithm]   

algorithm:  
- DILITHIUM2: dil2  
- DILITHIUM3: dil3  
- DILITHIUM5: dil5  
- FALCON512: fal512  
- FALCON1024: fal1024  

//for example  
server: ./server 12400 dil2  
client: ./client_tls helloworld.io 12400 dil2  

# How to run ExᴘʀᴇssPQDᴇʟɪᴠᴇʀʏ-applied PQC-TLS1.3
./server [port] [algorithm]  
./client_epqd [domain_address] [port] [algorithm]  

algorithm:  
- DILITHIUM2: dil2  
- DILITHIUM3: dil3  
- DILITHIUM5: dil5  
- FALCON512: fal512  
- FALCON1024: fal1024  

//for example  
server: ./server 12400 dil2  
client: ./client_epqd helloworld.io 12400 dil2  

# Prerequisite
> intstall [epqd_client_lib](https://github.com/ExpressPQDelivery/epqd_client_lib) for client  
> intstall [epqd_server_lib](https://github.com/ExpressPQDelivery/epqd_server_lib) for server  

# TroubleShooting
1. add environment variables
`export LD_LIBRARY_PATH=/usr/local/lib64`
