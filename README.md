# Steps to build Everything


<!--  -->


```bash
sudo apt install cmake gcc libtool libssl-dev make ninja-build git
```

```bash
sudo apt-get install doxygen
```

```bash
sudo apt-get install libclang1-15
```

```bash
git clone --branch OQS-OpenSSL_1_1_1-stable https://github.com/open-quantum-safe/openssl.git OPENSSL_DIR
```

```bash
cd OPENSSL_DIR
```

```bash
git clone --branch main https://github.com/open-quantum-safe/liboqs.git
```

```bash
cd liboqs
mkdir build && cd build
```

### Replace /home/titan/OPENSSL_DIR/oqs with your own path

```bash
cmake -GNinja -DCMAKE_INSTALL_PREFIX=/home/titan/OPENSSL_DIR/oqs -DOQS_USE_OPENSSL=OFF ..
```

```bash
ninja
ninja install
```

### Now navigate back to the OPENSSL_DIR

```bash
./Configure no-shared linux-x86_64 -lm
```

### You should get output something like below
```
Using os-specific seed configuration
Creating configdata.pm
Creating Makefile

**********************************************************************
***                                                                ***
***   OpenSSL has been successfully configured                     ***
***                                                                ***
***   If you encounter a problem while building, please open an    ***
***   issue on GitHub <https://github.com/openssl/openssl/issues>  ***
***   and include the output from the following command:           ***
***                                                                ***
***       perl configdata.pm --dump                                ***
***                                                                ***
***   (If you are new to OpenSSL, you might want to consult the    ***
***   'Troubleshooting' section in the INSTALL file first)         ***
***                                                                ***
**********************************************************************
```

### Running using max 2 threads at a time

```bash
make -j 2
```

```bash
sudo make install
```

### Now compile the c files by linking the binaries with appr flags

```bash
gcc -o server server.c -I/usr/local/include/openssl -L/usr/local/lib -L/usr/local/lib -lssl -lcrypto -loqs
```

```bash
gcc -o client client.c -I/usr/local/include/openssl -L/usr/local/lib -L/usr/local/lib -lssl -lcrypto -loqs
```
