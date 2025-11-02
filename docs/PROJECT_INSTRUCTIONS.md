# Webserv Project - Instructions & Context

## AI Communication Guidelines

**Be concise. Don't trail off.**
**Provide only what's requested. No unsolicited code/config files.**
**Use documentation style for team readability.**

---

## Project Overview

Building HTTP/1.1 web server in C++98 for:
1. Webserv project (non-blocking HTTP + CGI)
2. ft_transcendence project (add SSL/TLS later)

Modular architecture enables future upgrades without rewrites.

---

## Webserv Requirements

**Network:**
- Single poll()/select()/epoll()/kqueue() for all I/O
- Non-blocking sockets only (disk files exempt)
- Never read/write without poll() readiness
- No errno for flow control

**HTTP:**
- HTTP/1.1 support
- Methods: GET, POST, DELETE
- Accurate status codes
- Chunked transfer encoding

**Configuration:**
- NGINX-inspired config file
- Multiple ports/server blocks
- Location-based routing
- Error pages, body size limits

**Features:**
- Static file serving
- File uploads
- CGI execution (fork only for CGI)
- Directory listing
- Custom error pages

**Constraints:**
- C++98 only
- Browser compatible
- Compare with NGINX behavior
- macOS: fcntl() with F_SETFL, O_NONBLOCK, FD_CLOEXEC only

---

## ft_transcendence Requirements

**HTTPS Mandatory:**
- All connections must be HTTPS (not HTTP)
- WebSockets must use wss:// (not ws://)

**Security:**
- SQL injection/XSS protection
- Password hashing
- API route protection
- .env files for secrets

**Optional Features:**
- Backend: PHP or Fastify/Node.js
- Frontend: TypeScript + optional Tailwind
- Database: SQLite
- User management + OAuth
- Live chat (WebSockets)
- 2FA/JWT
- Microservices architecture

**Docker:**
- Single command launch
- All services containerized

---

## Architecture (5 Modules)

**1. Configuration** - Parse config file → Store settings  
**2. Server Core** - Sockets + Event loop + Connection management  
**3. Protocol** - HTTP parsing + Response formatting  
**4. Application** - Request routing + Handlers (Static, CGI, Upload, Error, Redirect)  
**5. Utility** - String/File/MIME helpers + Logging

**Flow:** Config → Server Core → Protocol → Application → Utility

**Key Principle:** Modules communicate through objects, not direct calls. Changes isolated to specific modules.

---

## Request Flow

1. Client connects → Server Core accepts
2. Event loop detects read → recv() data
3. Protocol parses → Request object
4. Application routes → Handler processes → Response object
5. Protocol formats → HTTP bytes
6. Event loop detects write → send() data

## SSL/TLS Upgrade (For Transcendence)

**Changes needed: ~15-20% of codebase**

**What changes:**
- Server Core: Add socket abstraction + TLS socket class (OpenSSL)
- Protocol: Add WebSocket support
- Application: Add new handlers (API, OAuth, WebSocket, DB)

**What stays same:**
- Event loop logic
- HTTP parsing
- Existing handlers (static, CGI, upload, error)
- Configuration structure
- All utilities

**Principle:** Additive changes, not rewrites.

---

## Key Documentation

**HTTP/1.1 - RFC 2616:**
- Sections 4-6: Message format, requests, responses
- Sections 9-10: Methods, status codes
- Section 14: Headers

**CGI - RFC 3875:**
- Section 4: Environment variables (REQUEST_METHOD, QUERY_STRING, CONTENT_TYPE, CONTENT_LENGTH, PATH_INFO, SCRIPT_NAME, etc.)

**Socket Programming:**
- System calls: socket(), bind(), listen(), accept(), recv(), send(), close()
- I/O multiplexing: poll()/select()/epoll()/kqueue()
- Non-blocking: fcntl(fd, F_SETFL, O_NONBLOCK)

**NGINX Reference:**
- Study server/location block structure
- Compare behavior with your implementation
- Use for testing: nginx.org/en/docs/

---

## State Machines

**Connection States:** ACCEPTING → READING → PROCESSING → WRITING → CLOSING (or back to READING if Keep-Alive)

**CGI Flow:** Detect CGI → Setup env vars → Create pipes → fork() → Child: execve(script) / Parent: I/O via pipes → Parse output → Send response

---

## Testing

**Manual:** telnet, curl, browsers  
**Comparison:** Run NGINX and webserv side-by-side, compare outputs  
**Stress:** siege, ab (Apache Bench)  
**Memory:** valgrind  

---

## Key Data Structures

**Connection:** FD, address, read/write buffers, state, timestamp  
**Request:** method, URI, version, headers (map), body  
**Response:** status code, headers (map), body, keep-alive flag  
**ServerConfig:** port, server_name, root, error_pages, locations  
**LocationConfig:** path, methods, root, CGI settings, redirects  

## Common Pitfalls

❌ Blocking I/O (always poll() before read/write on sockets)  
❌ Using errno for flow control  
❌ Memory/FD leaks  
❌ Ignoring SIGPIPE  
❌ No input validation (buffer overflows)  
❌ Incomplete request handling (data arrives in chunks)  
❌ Missing timeouts  
❌ Tight coupling between modules  

---

## Quick Reference

**Project files in:** `/mnt/project/webserv.pdf` and `/mnt/project/ft_transcendence.pdf`  
**Architecture doc:** `ARCHITECTURE.md` (detailed module breakdown)
