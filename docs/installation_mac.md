# Install Guide for Mac OS

No docker image for compilation is available for MacOS. We just use clang via xcode. 

## Python
You'll need
* Python (tested on 3.13 but should work on older like 3.10 just as well)
* pygame (see below)
* numpy

### pygame
You might find `pip install pygame` is enough these days but to make sure pictures load, follow the tutorial taken from the [Pygame Wiki: MacCompile](https://www.pygame.org/wiki/MacCompile)

### numpy
~~~
pip install numpy
~~~

### gdown
Not essential for running but this will be useful during the interview.
~~~
pip install gdown
gdown --version
~~~

## Command Line Tools (CLT) for XCode 
They are required to build the FW and for brew. Chances are you have them already if you use git asf. Make sure XCode is up-to-date!
~~~ zsh
xcode-select --install
~~~

To update an existing version, it might be necessary to remove the old like so: 
~~~ zsh
sudo rm -rf /Library/Developer/CommandLineTools
xcode-select --install
~~~

