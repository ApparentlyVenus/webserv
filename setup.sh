#!/bin/bash

# Create directory structure
mkdir -p /tmp/webserv
mkdir -p /tmp/webserv/errors
mkdir -p /tmp/webserv/about
mkdir -p /tmp/webserv/contact
mkdir -p /tmp/webserv/css
mkdir -p /tmp/webserv/images

# Create CSS file
cat > /tmp/webserv/css/style.css << 'EOF'
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    font-family: Arial, sans-serif;
    line-height: 1.6;
    color: #333;
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    min-height: 100vh;
}

.container {
    max-width: 800px;
    margin: 50px auto;
    background: white;
    padding: 40px;
    border-radius: 10px;
    box-shadow: 0 10px 30px rgba(0,0,0,0.3);
}

h1 {
    color: #667eea;
    margin-bottom: 20px;
    font-size: 2.5em;
}

h2 {
    color: #764ba2;
    margin: 20px 0 10px 0;
}

nav {
    background: #667eea;
    padding: 15px;
    border-radius: 5px;
    margin-bottom: 30px;
}

nav ul {
    list-style: none;
    display: flex;
    gap: 20px;
}

nav a {
    color: white;
    text-decoration: none;
    font-weight: bold;
    padding: 5px 15px;
    border-radius: 3px;
    transition: background 0.3s;
}

nav a:hover {
    background: rgba(255,255,255,0.2);
}

.card {
    background: #f4f4f4;
    padding: 20px;
    margin: 20px 0;
    border-radius: 5px;
    border-left: 4px solid #667eea;
}

.btn {
    display: inline-block;
    background: #667eea;
    color: white;
    padding: 10px 20px;
    text-decoration: none;
    border-radius: 5px;
    margin-top: 10px;
    border: none;
    cursor: pointer;
    font-size: 16px;
}

.btn:hover {
    background: #764ba2;
}

form {
    margin-top: 20px;
}

.form-group {
    margin-bottom: 20px;
}

label {
    display: block;
    margin-bottom: 5px;
    font-weight: bold;
    color: #667eea;
}

input[type="text"],
input[type="email"],
textarea {
    width: 100%;
    padding: 10px;
    border: 2px solid #ddd;
    border-radius: 5px;
    font-size: 16px;
}

input[type="text"]:focus,
input[type="email"]:focus,
textarea:focus {
    outline: none;
    border-color: #667eea;
}

textarea {
    resize: vertical;
    min-height: 120px;
}

.error-page {
    text-align: center;
}

.error-code {
    font-size: 120px;
    color: #667eea;
    font-weight: bold;
    margin: 20px 0;
}

footer {
    margin-top: 40px;
    padding-top: 20px;
    border-top: 2px solid #eee;
    text-align: center;
    color: #666;
}
EOF

# Create index.html
cat > /tmp/webserv/index.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Webserv - Home</title>
    <link rel="stylesheet" href="/css/style.css">
</head>
<body>
    <div class="container">
        <nav>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="/about">About</a></li>
                <li><a href="/contact">Contact</a></li>
            </ul>
        </nav>

        <h1>🚀 Welcome to Webserv!</h1>
        
        <div class="card">
            <h2>Your HTTP Server is Running!</h2>
            <p>Congratulations! If you can see this page, your webserv implementation is working correctly.</p>
        </div>

        <div class="card">
            <h2>✨ Features</h2>
            <ul style="margin-left: 20px; margin-top: 10px;">
                <li>HTTP/1.1 Protocol Support</li>
                <li>Multiple Port Listening</li>
                <li>Virtual Host Configuration</li>
                <li>GET, POST, DELETE Methods</li>
                <li>Static File Serving</li>
                <li>Custom Error Pages</li>
                <li>Non-blocking I/O with Poll</li>
            </ul>
        </div>

        <div class="card">
            <h2>🧪 Test Your Server</h2>
            <p>Try these commands in your terminal:</p>
            <pre style="background: #333; color: #0f0; padding: 15px; border-radius: 5px; margin-top: 10px; overflow-x: auto;">
curl http://localhost:8080/
curl http://localhost:8080/about
curl -X POST http://localhost:8080/contact -d "name=Test&email=test@example.com"
            </pre>
        </div>

        <div class="card">
            <h2>📚 Quick Links</h2>
            <a href="/about" class="btn">Learn More</a>
            <a href="/contact" class="btn">Get in Touch</a>
        </div>

        <footer>
            <p>Built with ❤️ using C++98 | Webserv Project 2026</p>
        </footer>
    </div>
</body>
</html>
EOF

# Create about.html
cat > /tmp/webserv/about/about.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Webserv - About</title>
    <link rel="stylesheet" href="/css/style.css">
</head>
<body>
    <div class="container">
        <nav>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="/about">About</a></li>
                <li><a href="/contact">Contact</a></li>
            </ul>
        </nav>

        <h1>📖 About Webserv</h1>
        
        <div class="card">
            <h2>What is Webserv?</h2>
            <p>Webserv is a lightweight HTTP/1.1 server implementation written in C++98. It's designed to handle multiple simultaneous connections efficiently using non-blocking I/O and the poll() system call.</p>
        </div>

        <div class="card">
            <h2>🏗️ Architecture</h2>
            <ul style="margin-left: 20px; margin-top: 10px;">
                <li><strong>Server Class:</strong> Main server loop using poll()</li>
                <li><strong>Config Parser:</strong> Nginx-style configuration</li>
                <li><strong>Request Parser:</strong> HTTP/1.1 compliant parsing</li>
                <li><strong>Response Builder:</strong> Dynamic response generation</li>
                <li><strong>Router:</strong> Location-based request routing</li>
            </ul>
        </div>

        <div class="card">
            <h2>⚙️ Technical Details</h2>
            <p><strong>Language:</strong> C++98</p>
            <p><strong>I/O Model:</strong> Non-blocking with poll()</p>
            <p><strong>Protocol:</strong> HTTP/1.1</p>
            <p><strong>Methods:</strong> GET, POST, DELETE</p>
            <p><strong>Features:</strong> Keep-alive, chunked transfer, virtual hosts</p>
        </div>

        <div class="card">
            <h2>🎯 Use Cases</h2>
            <p>Perfect for:</p>
            <ul style="margin-left: 20px; margin-top: 10px;">
                <li>Learning HTTP protocol internals</li>
                <li>Understanding server architecture</li>
                <li>Static file serving</li>
                <li>API development and testing</li>
                <li>Educational projects</li>
            </ul>
        </div>

        <footer>
            <p>Built with ❤️ using C++98 | Webserv Project 2026</p>
        </footer>
    </div>
</body>
</html>
EOF

# Create contact.html
cat > /tmp/webserv/contact/contact.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Webserv - Contact</title>
    <link rel="stylesheet" href="/css/style.css">
</head>
<body>
    <div class="container">
        <nav>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="/about">About</a></li>
                <li><a href="/contact">Contact</a></li>
            </ul>
        </nav>

        <h1>📧 Contact Us</h1>
        
        <div class="card">
            <h2>Get in Touch</h2>
            <p>Have questions about Webserv? Want to contribute or report a bug? We'd love to hear from you!</p>
        </div>

        <form method="POST" action="/contact">
            <div class="form-group">
                <label for="name">Name</label>
                <input type="text" id="name" name="name" required placeholder="Your name">
            </div>

            <div class="form-group">
                <label for="email">Email</label>
                <input type="email" id="email" name="email" required placeholder="your@email.com">
            </div>

            <div class="form-group">
                <label for="subject">Subject</label>
                <input type="text" id="subject" name="subject" required placeholder="What's this about?">
            </div>

            <div class="form-group">
                <label for="message">Message</label>
                <textarea id="message" name="message" required placeholder="Your message here..."></textarea>
            </div>

            <button type="submit" class="btn">Send Message</button>
        </form>

        <div class="card" style="margin-top: 30px;">
            <h2>📍 Other Ways to Reach Us</h2>
            <p><strong>GitHub:</strong> github.com/yourproject/webserv</p>
            <p><strong>Email:</strong> webserv@example.com</p>
            <p><strong>Documentation:</strong> Read the docs →</p>
        </div>

        <footer>
            <p>Built with ❤️ using C++98 | Webserv Project 2026</p>
        </footer>
    </div>
</body>
</html>
EOF

# Create 404 error page
cat > /tmp/webserv/errors/404.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>404 - Page Not Found</title>
    <link rel="stylesheet" href="/css/style.css">
</head>
<body>
    <div class="container error-page">
        <div class="error-code">404</div>
        <h1>Page Not Found</h1>
        <p>Oops! The page you're looking for doesn't exist.</p>
        <a href="/" class="btn">Go Home</a>
    </div>
</body>
</html>
EOF

# Create 500 error page
cat > /tmp/webserv/errors/500.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>500 - Internal Server Error</title>
    <link rel="stylesheet" href="/css/style.css">
</head>
<body>
    <div class="container error-page">
        <div class="error-code">500</div>
        <h1>Internal Server Error</h1>
        <p>Something went wrong on our end. We're working on fixing it!</p>
        <a href="/" class="btn">Go Home</a>
    </div>
</body>
</html>
EOF

echo "✅ Test environment created successfully!"
echo "📁 Files created in: /tmp/webserv"
echo ""
echo "To run your server:"
echo "  ./webserv config.conf"
echo ""
echo "Then visit: http://localhost:8080"
EOF

# Make the script executable
chmod +x setup_test.sh