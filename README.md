# C_HTTP
HTTP request in the C language (Linux + Windows )

#build

gcc test.c http.c -o http_req

#Usage

./http_req hostname page [num]


## example

./http_req www.example.com "/index.php?id=42&action=foo"

note the abscence of http:// and the usage of quote if your terminal proceed the special characters in the page



