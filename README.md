# Multithreaded Load Balancer

A loadbalancer's role is to equally distribute the load on all the servers.  
When multiple clients attempt to connect to the server, loadbalancer checks the load on each server and sends the request to the server with the least load.    
Here the task of the server is to capitalize the passed string.

Look at the following image explaining the flow of data.
![Loadbalancer Diagram](https://user-images.githubusercontent.com/26066500/68735120-a9ff3f00-0602-11ea-9f73-98805281adfe.png)  

#### Points to note:  
1. Both server and loadbalancer are multithreaded.
2. No. of servers there are fixed to 2, but can easily be scaled to the desired number with little effort.
3. Port is fixed for the loadbalancer is static (6000). However, the code to dynamically read port at runtime is present in the code (commented).
4. Maximum string size is assumed to be 100 characters.   
5. Load here is considered to be the number of connections on the server.
    
    
#### Steps to run (each in separate terminal):  
From server file and client file, make various copies while compiling.

1. Compile and run all the servers :  
    `gcc server.c -lpthread -o ser`  
    `./ser`
    
   Run all the servers on different ports preferably from 6001 onwards.
    
2. Compile and run the loadbalancer server :  
    `gcc loadbalancer.c -lpthread -o lb`  
    `./lb`
    
3. Compile and run all the clients :  
    `gcc client.c -lpthread -o cli`  
    `./cli`
    
> **Note:** Since loadbalancer and server are multithreaded, it is not required to rerun them again and again.  

Here is a screenshot of the code running with 4 clients and 2 servers.  

![screenshot](https://user-images.githubusercontent.com/26066500/68779941-7ef50980-065b-11ea-8af7-7d92bb7be20a.png)
