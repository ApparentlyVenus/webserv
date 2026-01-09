#!/usr/bin/php-cgi
<?php
header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html>
<head>
    <title>PHP CGI Test</title>
    <style>
        body { font-family: Arial, sans-serif; max-width: 800px; margin: 50px auto; padding: 20px; background-color: #f5f5f5; }
        .container { background-color: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        h1 { color: #333; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }
        .success { color: #4CAF50; font-weight: bold; }
        table { width: 100%; border-collapse: collapse; margin: 20px 0; }
        th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }
        th { background-color: #4CAF50; color: white; }
        a { color: #4CAF50; text-decoration: none; }
        a:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <div class='container'>
        <h1>PHP CGI Test</h1>
        <p class='success'>SUCCESS! PHP CGI is working correctly.</p>
        <h2>Server Information</h2>
        <table>
            <tr><th>Variable</th><th>Value</th></tr>
            <tr><td>Current Time</td><td><?php echo date('Y-m-d H:i:s'); ?></td></tr>
            <tr><td>PHP Version</td><td><?php echo phpversion(); ?></td></tr>
            <tr><td>SERVER_SOFTWARE</td><td><?php echo $_SERVER['SERVER_SOFTWARE'] ?? 'N/A'; ?></td></tr>
            <tr><td>SERVER_NAME</td><td><?php echo $_SERVER['SERVER_NAME'] ?? 'N/A'; ?></td></tr>
            <tr><td>GATEWAY_INTERFACE</td><td><?php echo $_SERVER['GATEWAY_INTERFACE'] ?? 'N/A'; ?></td></tr>
            <tr><td>REQUEST_METHOD</td><td><?php echo $_SERVER['REQUEST_METHOD'] ?? 'N/A'; ?></td></tr>
            <tr><td>QUERY_STRING</td><td><?php echo $_SERVER['QUERY_STRING'] ?? 'N/A'; ?></td></tr>
            <tr><td>REMOTE_ADDR</td><td><?php echo $_SERVER['REMOTE_ADDR'] ?? 'N/A'; ?></td></tr>
            <tr><td>HTTP_USER_AGENT</td><td><?php echo $_SERVER['HTTP_USER_AGENT'] ?? 'N/A'; ?></td></tr>
        </table>
        <p><a href='/'>Back to Home</a></p>
    </div>
</body>
</html>
