# NeuTalk

A instant chat application, developed with gtkmm and c++. 

## Compile

To compile application:

```
cd client
make
```

To compile server:

```
cd server
make
```

## Run

**Note:** edit IP address and port number in file  `config` .

```
cd client/bin
./neutalk
```

Run server

```
cd server/bin
./server
```

## Update Log
0.9.2
- now, information bar won't expand the main window.
- User will be still notified when corresponding chat window is hide.

0.9.3
- This version make it possible to monitor disconnections.

0.9.4
- Chat room support.

0.9.5
- Fix a bug on authentication.
- Chat History support. 

## Bug Report
- [ ] There is a potential crash when user list is clicked