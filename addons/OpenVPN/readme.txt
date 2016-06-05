cd easy-rsa
=======================================================
if you have ca.crt and ca.key just copy them to easy-rsa/keys otherwise:

. ./vars ;- store data like password
./clean-all
./build-ca ; - generate ca.crt ca.key
=======================================================
For server:
./build-key-server <server name> ; generate <server name>.crt <server name>.key

./build-dh ; generate dh{n}.pem

For client:
./build-key <client name> ; generate <client name>.crt <client name>.key

=======================================================
Secret files: ca.key <server name>.key <client name>.key

Server files: ca.crt dh{n}.pem <server name>.crt <server name>.key

Client files: ca.crt <client name>.crt <client name>.key