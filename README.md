# cubo

* frag: Chinese code
* matrix_conversion: convert plain text to binary file (and other options)
* matrix_sample: Plain text sample

## Ubuntu 14.04 x64
### Updating g++ to 4.9

```shell
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-4.9
```

### Remaking g++ symbolic link

```shell
cd /usr/bin
sudo rm g++
sudo ln -s g++-4.9 g++
```