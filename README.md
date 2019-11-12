# Load-Balancer
A loadbalancer's role is to equally distribute the load on all the servers.
When multiple clients attempt to connect to the server, loadbalancer checks the load on each server
and send the request to the server with the least load.

Points to note:
1. Both server and loadbalancer are multithreaded.
2. No. of servers there are fixed to 2, but can easily be scaled to desired number with little effort.
3. Port is fixed for the loadbalancer is static(6000). However, the code to dynamically read port 
   at runtime is present in the code(commented).
   
   
#Steps to run(each in separate terminal):#

1. Compile and run all the servers as : 
    gcc server<server#>.c -lpthread -o ser<server#>
    ./ser<server#>
    
2. Compile and run the loadbalancer as :
    gcc loadbalancer.c -lpthread -o lb
    ./lb
    
3. Compile and run all the clients as : 
    gcc client<client#>.c -lpthread -o cli<client#>
    ./cli<client#>
    
Note: Since loadbalancer and server are multithreaded, it is not required to re-run them again and again.
