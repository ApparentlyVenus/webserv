#!/usr/bin/python3

import os
import sys
import urllib.parse

# Read POST data
content_length = os.environ.get('CONTENT_LENGTH', '0')
if content_length:
    post_data = sys.stdin.read(int(content_length))
else:
    post_data = ""

# Parse form data
form_data = {}
if post_data:
    form_data = urllib.parse.parse_qs(post_data)

print("Content-Type: text/html\n")
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("    <title>Form Handler Result</title>")
print("    <style>")
print("        body { font-family: Arial, sans-serif; max-width: 800px; margin: 50px auto; padding: 20px; background-color: #f5f5f5; }")
print("        .container { background-color: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }")
print("        h1 { color: #333; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }")
print("        .success { background-color: #d4edda; color: #155724; padding: 15px; border-radius: 4px; border: 1px solid #c3e6cb; }")
print("        table { width: 100%; border-collapse: collapse; margin: 20px 0; }")
print("        th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }")
print("        th { background-color: #4CAF50; color: white; }")
print("        a { color: #4CAF50; text-decoration: none; font-weight: bold; }")
print("        a:hover { text-decoration: underline; }")
print("    </style>")
print("</head>")
print("<body>")
print("    <div class='container'>")
print("        <h1>Form Submission Result</h1>")
print("        <div class='success'>")
print("            <strong>SUCCESS!</strong> Your form was submitted via POST method.")
print("        </div>")
print("        <h2>Submitted Data</h2>")
print("        <table>")
print("            <tr><th>Field</th><th>Value</th></tr>")

if form_data:
    for key, value in form_data.items():
        print("            <tr><td>" + key + "</td><td>" + value[0] + "</td></tr>")
else:
    print("            <tr><td colspan='2'>No data received</td></tr>")

print("        </table>")
print("        <h2>Request Information</h2>")
print("        <table>")
print("            <tr><th>Variable</th><th>Value</th></tr>")
print("            <tr><td>REQUEST_METHOD</td><td>" + os.environ.get('REQUEST_METHOD', 'N/A') + "</td></tr>")
print("            <tr><td>CONTENT_TYPE</td><td>" + os.environ.get('CONTENT_TYPE', 'N/A') + "</td></tr>")
print("            <tr><td>CONTENT_LENGTH</td><td>" + str(content_length) + "</td></tr>")
print("        </table>")
print("        <p><a href='/test-post.html'>Try Again</a> | <a href='/'>Back to Home</a></p>")
print("    </div>")
print("</body>")
print("</html>")
