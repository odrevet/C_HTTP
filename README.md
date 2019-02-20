# C_HTTP

HTTP request in the C language for Linux and Windows. 

# build

 gcc test.c http.c -o http_req

# Usage

 ./http_req hostname page

# example

 ./http_req www.example.com "/index.php?id=42&action=foo"

note: 

* the url is not prefixed by 'http://'
* the usage of quotes to prevent special character to mess with the terminal
