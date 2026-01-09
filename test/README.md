
# Webserv Test Suite

This directory contains a comprehensive test suite for testing your webserver implementation.

## Directory Structure

```
test/
├── test.conf              # Test configuration file
├── README.md             # This file
└── www/
    ├── html/             # HTML test pages
    │   ├── index.html           # Main test page with links to all tests
    │   ├── test-get.html        # GET method test
    │   ├── test-post.html       # POST method test with form
    │   ├── test-upload.html     # File upload test (POST)
    │   └── test-delete.html     # DELETE method test
    ├── cgi-bin/          # CGI scripts
    │   ├── test.py              # Python CGI test
    │   ├── test.php             # PHP CGI test
    │   ├── test.pl              # Perl CGI test
    │   └── form-handler.py      # POST form handler
    ├── uploads/          # Upload directory (will be created by server)
    └── errors/           # Error pages
        ├── 404.html             # Not Found error page
        └── 500.html             # Internal Server Error page
```

## How to Run Tests

1. Start your webserver with the test configuration:
   ```bash
   ./webserv test/test.conf
   ```

2. Open your browser and navigate to:
   ```
   http://localhost:8080
   ```

3. The main page will show links to all available tests.

## Test Coverage

### GET Method Tests
- `/` - Main index page
- `/test-get.html` - Simple GET test
- `/uploads` - Directory listing with autoindex

### POST Method Tests
- `/test-post.html` - Form submission test (submits to CGI)
- `/test-upload.html` - File upload test

### DELETE Method Tests
- `/test-delete.html` - Delete uploaded files

### CGI Tests
- `/cgi-bin/test.py` - Python CGI (GET)
- `/cgi-bin/test.php` - PHP CGI (GET)
- `/cgi-bin/test.pl` - Perl CGI (GET)
- `/cgi-bin/form-handler.py` - Python form handler (POST)

## Configuration Details

- **Port**: 8080
- **Host**: 127.0.0.1
- **Max Body Size**: 10 MB (10485760 bytes)
- **Upload Directory**: test/www/uploads
- **Error Pages**: Custom 404 and 500 pages

## Expected Behavior

1. **GET requests** should return the requested HTML pages
2. **POST requests** to /uploads should store files in the uploads directory
3. **DELETE requests** to /uploads/filename should remove the specified file
4. **CGI scripts** should execute and return dynamic HTML content
5. **Error pages** should display for 404 and 500 errors
6. **Autoindex** should show directory listings for /uploads

## Testing Tips

1. Test GET first - it's the simplest
2. Upload some files, then test DELETE
3. Check that CGI environment variables are properly set
4. Verify that error pages are displayed correctly
5. Test edge cases:
   - Large file uploads (near the size limit)
   - Non-existent files (should return 404)
   - Deleting files that don't exist
   - Multiple uploads

## Troubleshooting

If CGI scripts don't work:
- Check that scripts are executable: `chmod +x test/www/cgi-bin/*`
- Verify interpreter paths in the shebang lines
- Check CGI environment variables are being set correctly

If uploads don't work:
- Ensure the uploads directory exists and is writable
- Check that upload_enable is set to true in the config
- Verify client_max_body_size is appropriate
