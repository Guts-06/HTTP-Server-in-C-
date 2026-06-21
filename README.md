# HTTP Server in C++

A multi-threaded HTTP/1.1 server built from scratch in C++ using raw POSIX sockets — no external libraries.

## Features
- Multi-threaded (one thread per client via pthreads)
- HTTP GET request parsing
- Static file serving (HTML, CSS)
- Multiple routes (/ /about /style.css)
- HTTP status codes (200 OK, 404 Not Found)
- Content-Type and Content-Length headers

## Tech Stack
- **Language:** C++17
- **Networking:** POSIX Sockets (`sys/socket.h`)
- **Concurrency:** pthreads (`pthread.h`)
- **Build:** Makefile

## How to Run
Requirements: Linux / WSL + g++ + make

```bash
git clone https://github.com/YOUR_USERNAME/http-server-cpp.git
cd http-server-cpp
make
./server
```

Open: `http://localhost:8080`

## Resume Bullet
> Built a multi-threaded HTTP/1.1 server in C++ from scratch using POSIX sockets, handling concurrent GET requests, static file serving, and proper HTTP status codes (200, 404) — no external libraries used.
