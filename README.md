# ChatSocketMultithread
## Client

Compile client
```
gcc client1.c -o client -pthread -l json-c
```

Execute client
```
./client 120.0.0.1 0 8989 username
```

## Server
Compile server
```
gcc server1.c -o server -pthread -l json-c
```

Execute server
```
./server 8989
```
