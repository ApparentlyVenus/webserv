# Webserv

*This project has been created as part of the 42 curriculum by odana, wasmar and yitani.*

## Description

Webserv is a custom HTTP/1.1 web server implementation built from scratch in C++98. The project demonstrates deep understanding of network programming, HTTP protocol specifications, and non-blocking I/O operations. The server handles multiple simultaneous client connections using a single event loop (poll/select/epoll/kqueue), serves static content, executes CGI scripts, manages file uploads, and implements session management with cookies.

### Key Features

- **HTTP/1.1 Protocol Support**: Full implementation of GET, POST, and DELETE methods
- **Non-blocking I/O**: Single poll() event loop handling all socket operations
- **CGI Execution**: Support for Python, PHP, and Perl scripts with proper environment variable setup
- **File Operations**: Upload, download, and delete capabilities with configurable size limits
- **Session Management**: Cookie-based authentication and user sessions
- **Directory Listing**: Configurable autoindex for browsing uploaded files
- **Custom Error Pages**: Configurable error pages for various HTTP status codes
- **NGINX-style Configuration**: Flexible server and location block configuration

### Architecture

The project follows a modular five-component architecture:

1. **Configuration Module**: Parses NGINX-style config files and stores settings
2. **Server Core Module**: Socket management, event loop, and connection handling
3. **Protocol Module**: HTTP request parsing and response formatting
4. **Application Module**: Request routing and handlers (static files, CGI, uploads)
5. **Utility Module**: Helper functions for strings, files, MIME types, and logging

## Instructions

### Prerequisites

- C++ compiler supporting C++98 standard
- Make
- Python 3, PHP-CGI, and Perl (optional, for CGI support)

### Compilation
```bash
# Clone the repository
git clone [repository-url]
cd webserv

# Compile the project
make

# The executable 'webserv' will be created
```

### Running the Server
```bash
# Start with a configuration file
./webserv [config_file]

# Example with test configuration
./webserv test/test.conf

# Example with default configuration
./webserv webserv.conf
```

### Configuration

The server uses an NGINX-inspired configuration format. Example:
```nginx
server {
    listen 8080;
    host 127.0.0.1;
    server_name localhost;
    root www/html;
    client_max_body_size 10485760;
    error_page 404 www/errors/404.html;
    error_page 500 www/errors/500.html;

    location / {
        allowed_methods GET POST;
        root www/html;
        index index.html;
        autoindex off;
    }

    location /uploads {
        allowed_methods POST DELETE GET;
        root www/uploads;
        upload_enable true;
        upload_store www/uploads;
        autoindex on;
    }

    location /cgi-bin {
        allowed_methods GET POST;
        cgi_extension .py .php .pl;
        cgi_py /usr/bin/python3;
        cgi_php /usr/bin/php-cgi;
        cgi_pl /usr/bin/perl;
        root www/cgi-bin;
    }
}
```

### Testing

#### Browser Testing
1. Start the server: `./webserv test/test.conf`
2. Open browser: `http://localhost:8062`
3. Navigate through the test pages to verify features

#### Command Line Testing
```bash
# Test GET request
curl http://localhost:8062/

# Test file upload
curl -X POST http://localhost:8062/uploads/test.txt -d "Hello World"

# Test file retrieval
curl http://localhost:8062/uploads/test.txt

# Test file deletion
curl -X DELETE http://localhost:8062/uploads/test.txt

# Test CGI execution
curl http://localhost:8062/cgi-bin/test.py

# Test session/login
curl -X POST http://localhost:8062/do_login -d "username=alice" -c cookies.txt
curl http://localhost:8062/dashboard -b cookies.txt
```

#### Stress Testing
```bash
# Using siege
siege -c 100 -t 30s http://localhost:8062/

# Using Apache Bench
ab -n 1000 -c 50 http://localhost:8062/
```

#### Memory Testing
```bash
valgrind --leak-check=full --track-origins=yes ./webserv test/test.conf
```

### Project Structure
```
webserv/
├── inc/                    # Header files
│   ├── ServerConfig.hpp
│   ├── LocationConfig.hpp
│   ├── Request.hpp
│   ├── Response.hpp
│   ├── Handlers.hpp
│   ├── SessionManager.hpp
│   └── ...
├── src/                    # Source files
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Request.cpp
│   ├── Response.cpp
│   ├── Handlers.cpp
│   └── ...
├── www/                    # Web content
│   ├── html/              # Static HTML pages
│   ├── cgi-bin/           # CGI scripts
│   ├── uploads/           # Upload directory
│   └── errors/            # Custom error pages
├── test/                   # Test configuration and files
├── docs/                   # Documentation
│   ├── PROJECT_INSTRUCTIONS.md
│   └── ARCHITECTURE.md
├── Makefile
├── webserv.conf           # Default configuration
└── README.md
```

## Resources

### HTTP Protocol
- [RFC 2616 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616) - The HTTP/1.1 specification
- [RFC 3875 - CGI Specification](https://www.rfc-editor.org/rfc/rfc3875) - Common Gateway Interface specification
- [Mozilla HTTP Documentation](https://developer.mozilla.org/en-US/docs/Web/HTTP) - Comprehensive HTTP reference

### Network Programming
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) - Socket programming tutorial
- [poll() man page](https://man7.org/linux/man-pages/man2/poll.2.html) - I/O multiplexing documentation
- [NGINX Documentation](https://nginx.org/en/docs/) - Reference for configuration syntax

### C++ Resources
- [C++98 Standard](https://www.iso.org/standard/25845.html) - Language specification
- [cppreference.com](https://en.cppreference.com/) - C++ standard library reference

### Articles and Tutorials
- "Building a Simple HTTP Server" - Understanding server architecture
- "Non-blocking I/O in C++" - Event-driven programming patterns
- "Understanding CGI" - How web servers execute scripts

### AI Usage

AI tools (ChatGPT, Claude) were used to assist with:

1. **Learning and Research**:
   - Understanding HTTP/1.1 protocol details and edge cases
   - Clarifying RFC specifications and behavior expectations
   - Learning about multipart/form-data parsing
   - Understanding non-blocking I/O patterns with poll()

2. **Code Review and Debugging**:
   - Identifying memory leaks and uninitialized variables
   - Debugging path resolution issues in request routing
   - Fixing edge cases in request parsing (incomplete requests, chunked encoding)
   - Valgrind error analysis and resolution

3. **Implementation Guidance**:
   - Session management architecture design
   - CGI environment variable setup following RFC 3875
   - Proper error handling and HTTP status code selection
   - Configuration file parsing strategy

4. **Documentation**:
   - Architecture documentation structure
   - README formatting and content organization
   - Code comments and inline documentation

5. **Redundant Code**:
   - Getters and Setters
   - Some utils
   
**What AI Did NOT Do**:
- AI did not write the core server implementation
- Design decisions and architecture were team-driven
- All code was reviewed, understood, and modified by team members
- Testing strategies and validation were performed manually

The team maintained full understanding of all implemented code and could explain any part during evaluation. AI served as a learning aid and debugging assistant, not as a code generator.

## Technical Choices

### Why poll() over select()?
- No file descriptor limit (select() limited to 1024)
- More efficient for large numbers of connections
- Cleaner API for managing multiple sockets

### Why C++98?
- Project requirement per 42 curriculum
- Demonstrates understanding without modern C++ conveniences
- Prepares for working with legacy codebases

### Configuration Design
- NGINX-inspired syntax for familiarity
- Modular parsing allows easy extension
- Clear separation between server and location blocks

### Session Management
- In-memory storage for simplicity (no database required)
- Session expiration prevents memory leaks
- Cookie-based authentication follows web standards

## Known Limitations

- HTTP/1.1 only (no HTTP/2 support)
- No SSL/TLS (plain HTTP only)
- In-memory session storage (not persistent across restarts)
- Single configuration file (no includes)
- No virtual host support (single server per instance)

## Future Enhancements

This implementation was designed with extensibility in mind for future projects (ft_transcendence):
- SSL/TLS support can be added by abstracting socket layer
- WebSocket support for real-time communication
- Database integration for persistent sessions
- Load balancing and multi-server support

## Team Contributions

- **yitani**: Application layer (request handling, routing, CGI execution)
- **odana**: Configuration parsing, validation, cookies, and session management system
- **wasmar**: Server core (socket management, event loop, connection handling)

All team members contributed to testing, debugging, and documentation.
school
