# Load-Balancer
A loadbalancer's role is to equally distribute the load on all the servers.
When multiple clients attempt to connect to the server, loadbalancer checks the load on each server
and send the request to the server with the least load.
Here the task of the server is to capitalize the passed string.

Look at the following image explaining the flow of data.
![Loadbalancer Diagram](https://user-images.githubusercontent.com/26066500/68712795-7997b080-05c2-11ea-86a5-e7b63c9c9a56.png)

Points to note:
1. Both server and loadbalancer are multithreaded.
2. No. of servers there are fixed to 2, but can easily be scaled to desired number with little effort.
3. Port is fixed for the loadbalancer is static(6000). However, the code to dynamically read port 
   at runtime is present in the code(commented).
4. Maximum string size is assumed to be 100 characters.
   
   
Steps to run(each in separate terminal):
From server file and client file, make various copies while compiling.

1. Compile and run all the servers as : 
    `gcc server.c -lpthread -o ser<server#>`
    `./ser<server#>`
    
   Run all the servers on different ports preferably from 6001 onwards.
    
2. Compile and run the loadbalancer as :
    `gcc loadbalancer.c -lpthread -o lb`
    `./lb`
    
3. Compile and run all the clients as : 
    `gcc client.c -lpthread -o cli<client#>`
    `./cli<client#>`
    
Note: Since loadbalancer and server are multithreaded, it is not required to re-run them again and again.
