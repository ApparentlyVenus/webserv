#!/usr/bin/php-cgi
<?php
header("Content-Type: text/html");

$name = "";
$message = "";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $input = file_get_contents('php://stdin');
    parse_str($input, $data);
    $name = isset($data['name']) ? htmlspecialchars($data['name']) : '';
    $message = isset($data['message']) ? htmlspecialchars($data['message']) : '';
}
?>
<html>
<head>
    <title>PHP CGI - Form Processing</title>
    <style>
        body { font-family: Arial; padding: 20px; }
        .result { background: #e8f5e9; padding: 15px; border-radius: 5px; margin: 20px 0; }
    </style>
</head>
<body>
    <h1>PHP CGI Script</h1>
    <h2>Form Processing Demo</h2>
    
    <?php if ($_SERVER['REQUEST_METHOD'] === 'POST'): ?>
        <div class="result">
            <h3>Form Submitted Successfully!</h3>
            <p><strong>Name:</strong> <?php echo $name; ?></p>
            <p><strong>Message:</strong> <?php echo $message; ?></p>
        </div>
    <?php endif; ?>
    
    <form method="POST" action="/cgi-bin/form.php">
        <p>
            <label>Name:</label><br>
            <input type="text" name="name" required style="width: 300px; padding: 5px;">
        </p>
        <p>
            <label>Message:</label><br>
            <textarea name="message" rows="4" required style="width: 300px; padding: 5px;"></textarea>
        </p>
        <button type="submit" style="padding: 8px 20px; cursor: pointer;">Submit</button>
    </form>
    
    <p><a href="/cgi.html">Back to CGI Demo</a></p>
</body>
</html>