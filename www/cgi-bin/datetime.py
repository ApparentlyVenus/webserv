#!/usr/bin/python3

import os
import datetime

print("Content-Type: text/html\r")
print("\r")
print("<html>")
print("<head><title>Python CGI - Date/Time</title></head>")
print("<body style='font-family: Arial; padding: 20px;'>")
print("<h1>Python CGI Script</h1>")
print("<h2>Current Date and Time</h2>")
print("<p><strong>Server Time:</strong> {}</p>".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
print("<h3>Environment Variables:</h3>")
print("<ul>")
print("<li>REQUEST_METHOD: {}</li>".format(os.environ.get('REQUEST_METHOD', 'N/A')))
print("<li>QUERY_STRING: {}</li>".format(os.environ.get('QUERY_STRING', 'N/A')))
print("<li>SERVER_NAME: {}</li>".format(os.environ.get('SERVER_NAME', 'N/A')))
print("<li>SERVER_PORT: {}</li>".format(os.environ.get('SERVER_PORT', 'N/A')))
print("</ul>")
print("<p><a href='/cgi.html'>Back to CGI Demo</a></p>")
print("</body>")
print("</html>")