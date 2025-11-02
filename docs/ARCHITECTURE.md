# Webserv - System Architecture

## Project Overview

Webserv is a custom HTTP/1.1 web server built from scratch in C++98. This document outlines the modular architecture designed for the initial implementation, with consideration for future extensibility (SSL/TLS support for ft_transcendence).

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    WEBSERV APPLICATION                      │
└─────────────────────────────────────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
        ▼                   ▼                   ▼
┌───────────────┐   ┌──────────────┐   ┌──────────────┐
│ Configuration │   │ Server Core  │   │  Application │
│    Module     │   │    Module    │   │    Module    │
└───────────────┘   └──────────────┘   └──────────────┘
        │                   │                   │
        │                   │                   │
        └───────────────────┼───────────────────┘
                            │
                            ▼
                    ┌──────────────┐
                    │   Utility    │
                    │    Module    │
                    └──────────────┘
```

---

## Module Directory

| Module | Purpose | Dependencies |
|--------|---------|--------------|
| Configuration Module | Parse and store server configuration | None |
| Server Core Module | Network operations and event management | Configuration |
| Protocol Module | HTTP parsing and formatting | Server Core |
| Application Module | Request handling and business logic | Protocol, Server Core |
| Utility Module | Common helper functions | None |

---

## Module 1: Configuration Module

### Purpose
Parse the configuration file at startup and provide configuration data to other modules throughout the server's lifetime.

### Responsibilities
- Read configuration file from path argument or default location
- Parse server blocks and location directives
- Validate configuration syntax and values
- Store configuration in accessible data structures
- Provide configuration lookup methods

### Key Components
- **ConfigParser**: Reads and tokenizes configuration file
- **ServerConfig**: Stores server block configuration (ports, server names, error pages)
- **LocationConfig**: Stores location block configuration (routes, methods, CGI settings)
- **ConfigValidator**: Validates configuration values and dependencies

### Ownership
- Independent module, no runtime dependencies
- Loaded once at startup
- Read-only access provided to other modules

### Context Within Project
**Entry point for server behavior.** All server features are configured here. Other modules query this module to determine how to handle requests. This is the single source of truth for server configuration.

### Data Provided
- List of server blocks (port, server_name, root, error_pages)
- List of locations per server (path, methods, CGI settings, redirects)
- Global settings (max body size, timeout values)

---

## Module 2: Server Core Module

### Purpose
Manage all network operations, including socket creation, connection acceptance, and the main event loop that drives the entire server.

### Responsibilities
- Create and bind listening sockets based on configuration
- Accept incoming client connections
- Maintain non-blocking I/O for all sockets
- Run the main event loop (poll/select/epoll/kqueue)
- Manage connection lifecycle and state
- Handle connection timeouts
- Buffer management for partial reads/writes

### Key Components

#### 2.1 Socket Manager
- **Purpose**: Create and manage listening sockets
- **Operations**: socket(), bind(), listen(), accept()
- **Ownership**: Manages listening socket file descriptors

#### 2.2 Event Loop
- **Purpose**: Monitor all file descriptors for I/O readiness
- **Operations**: poll()/select()/epoll()/kqueue()
- **Ownership**: Central event dispatcher, calls other modules based on events

#### 2.3 Connection Manager
- **Purpose**: Track all active client connections
- **Operations**: Accept new connections, buffer I/O, close connections
- **Ownership**: Manages client socket file descriptors and connection state

### Ownership
- Owns all socket file descriptors (listening and client)
- Controls the main event loop
- Delegates parsing to Protocol Module
- Delegates handling to Application Module

### Context Within Project
**The heart of the server.** This module never stops running. It waits for network events, reads data from clients, passes data to other modules for processing, and writes responses back to clients. All other modules are called from this module's event loop.

### Data Provided
- Connection objects (FD, state, buffers, timestamps)
- Read/write events to other modules

---

## Module 3: Protocol Module

### Purpose
Handle all HTTP protocol operations: parsing incoming requests and formatting outgoing responses according to HTTP/1.1 specification.

### Responsibilities
- Parse HTTP request line (method, URI, version)
- Parse HTTP headers
- Parse request body (handle Content-Length and chunked encoding)
- Validate HTTP syntax
- Generate HTTP response (status line, headers, body)
- Handle HTTP state across multiple recv() calls (partial requests)

### Key Components

#### 3.1 Request Parser
- **Purpose**: Convert raw bytes into structured Request objects
- **Operations**: Parse request line, headers, body
- **Ownership**: Creates Request objects

#### 3.2 Response Generator
- **Purpose**: Convert Response objects into HTTP-formatted bytes
- **Operations**: Format status line, headers, body
- **Ownership**: Creates response byte strings

#### 3.3 HTTP State Machine
- **Purpose**: Track parsing progress for partial requests
- **Operations**: Manage parsing state (line, headers, body)
- **Ownership**: Maintains per-connection parsing state

### Ownership
- Does NOT own sockets (provided by Server Core)
- Does NOT make routing decisions (handled by Application)
- Purely focused on HTTP syntax

### Context Within Project
**The translator between network bytes and structured data.** This module understands HTTP but nothing about files, CGI, or application logic. It converts incoming byte streams into Request objects that the Application module can work with, and converts Response objects back into HTTP-formatted bytes for transmission.

### Data Provided
- Request objects (method, URI, headers, body)
- Response byte strings (formatted HTTP responses)

---

## Module 4: Application Module

### Purpose
Implement all business logic for handling HTTP requests: routing, file serving, CGI execution, uploads, redirects, and error handling.

### Responsibilities
- Route requests to appropriate handlers
- Serve static files
- Execute CGI scripts
- Handle file uploads
- Generate redirects
- Handle errors with custom error pages
- Apply location-specific rules

### Key Components

#### 4.1 Request Router
- **Purpose**: Match requests to server blocks and locations
- **Operations**: URI matching, handler selection
- **Ownership**: Routing decisions

#### 4.2 Static File Handler
- **Purpose**: Serve files from the filesystem
- **Operations**: File reading, MIME type detection, directory listings
- **Ownership**: File serving logic

#### 4.3 CGI Handler
- **Purpose**: Execute CGI scripts and capture output
- **Operations**: fork(), exec(), pipe management, environment variables
- **Ownership**: CGI process lifecycle

#### 4.4 Upload Handler
- **Purpose**: Process file uploads from clients
- **Operations**: Multipart parsing, file writing, size validation
- **Ownership**: Upload processing logic

#### 4.5 Error Handler
- **Purpose**: Generate error responses
- **Operations**: Error page generation, custom error pages
- **Ownership**: Error response creation

#### 4.6 Redirect Handler
- **Purpose**: Generate HTTP redirects
- **Operations**: 301/302 response creation
- **Ownership**: Redirect logic

### Ownership
- Owns all request handling logic
- Does NOT own sockets or HTTP parsing
- Receives Request objects, produces Response objects

### Context Within Project
**The brain of the server.** This module decides what to do with each request. It knows about files, CGI, uploads, and redirects. It makes all the decisions about how to respond to a request based on the configuration and the request details. All handlers in this module work with high-level Request/Response objects, never touching raw sockets or HTTP bytes.

### Data Provided
- Response objects (status code, headers, body)

---

## Module 5: Protocol Module (Detailed Breakdown)

**Note**: This module was introduced in the main diagram but deserves detailed explanation as it bridges Server Core and Application.

### Purpose
Separate HTTP protocol concerns from both network I/O and application logic.

### Sub-modules

#### Request Parser
- **Input**: Raw byte buffer from Server Core
- **Output**: Structured Request object to Application
- **Handles**: Partial requests, chunked encoding, malformed requests

#### Response Generator  
- **Input**: Response object from Application
- **Output**: HTTP-formatted byte string to Server Core
- **Handles**: Header formatting, chunked responses, keep-alive

### Ownership
Stateless protocol operations. Maintains parsing state per connection but does not own connections or make handling decisions.

### Context Within Project
**The protocol layer.** Insulates the application from HTTP details. Application developers work with clean Request/Response objects and never need to know about HTTP syntax, header formatting, or chunked encoding.

---

## Module 6: Utility Module

### Purpose
Provide common functionality used across all modules to avoid code duplication.

### Responsibilities
- String manipulation (trim, split, case-insensitive compare)
- File operations (exists, permissions, size, read)
- MIME type detection based on file extension
- Logging (timestamped log entries)
- URL encoding/decoding

### Key Components

#### String Utilities
- **Purpose**: Common string operations
- **Operations**: trim(), split(), toLower(), urlDecode()

#### File Utilities
- **Purpose**: Filesystem operations
- **Operations**: fileExists(), isReadable(), getFileSize(), readFile()

#### Logger
- **Purpose**: Centralized logging
- **Operations**: logInfo(), logError(), logWarning()

#### MIME Helper
- **Purpose**: Determine Content-Type from file extension
- **Operations**: getMimeType(".html") → "text/html"

### Ownership
Pure utility functions with no state or dependencies.

### Context Within Project
**The toolbox.** Every module uses these utilities. This prevents reimplementing common operations and ensures consistency across the codebase (e.g., all modules log the same way, all modules determine MIME types the same way).

---

## Data Flow: Complete Request Cycle

```
1. Client connects
   │
   ├─> Server Core (Socket Manager)
   │   └─> accept() new connection
   │       └─> Connection Manager stores FD and state
   │
2. Client sends request data
   │
   ├─> Server Core (Event Loop)
   │   └─> poll() detects readable FD
   │       └─> Connection Manager recv() data into buffer
   │
3. Request parsing
   │
   ├─> Protocol Module (Request Parser)
   │   └─> Parse method, URI, headers, body
   │       └─> Create Request object
   │
4. Request routing and handling
   │
   ├─> Application Module (Request Router)
   │   └─> Match server block and location
   │       └─> Select handler (Static/CGI/Upload/Redirect)
   │           └─> Handler processes request
   │               └─> Create Response object
   │
5. Response formatting
   │
   ├─> Protocol Module (Response Generator)
   │   └─> Format status line, headers, body
   │       └─> Create response byte string
   │
6. Response transmission
   │
   ├─> Server Core (Connection Manager)
   │   └─> Buffer response data
   │       └─> Event Loop detects writable FD
   │           └─> send() response data
   │               └─> Close or keep-alive
```

---

## Module Interaction Rules

### Configuration Module
- **Called by**: Main (at startup), All modules (for config lookup)
- **Calls**: None
- **Provides**: Configuration data (read-only)

### Server Core Module
- **Called by**: Main (starts event loop)
- **Calls**: Protocol Module (for parsing/formatting), Application Module (for handling)
- **Provides**: I/O operations, event notifications

### Protocol Module
- **Called by**: Server Core
- **Calls**: Utility Module
- **Provides**: Request objects, Response byte strings

### Application Module
- **Called by**: Server Core (via Protocol Module)
- **Calls**: Utility Module, Configuration Module
- **Provides**: Response objects

### Utility Module
- **Called by**: All modules
- **Calls**: System libraries only
- **Provides**: Helper functions

---
