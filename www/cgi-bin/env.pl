#!/usr/bin/perl

print "Content-Type: text/html\r\n\r\n";

print "<html>\n";
print "<head><title>Perl CGI - Environment</title></head>\n";
print "<body style='font-family: Arial; padding: 20px;'>\n";
print "<h1>Perl CGI Script</h1>\n";
print "<h2>All Environment Variables</h2>\n";
print "<table border='1' cellpadding='8' style='border-collapse: collapse;'>\n";
print "<tr><th>Variable</th><th>Value</th></tr>\n";

foreach my $key (sort keys %ENV) {
    my $value = $ENV{$key};
    print "<tr><td><strong>$key</strong></td><td>$value</td></tr>\n";
}

print "</table>\n";
print "<p><a href='/cgi.html'>Back to CGI Demo</a></p>\n";
print "</body>\n";
print "</html>\n";