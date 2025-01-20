# Install Guide for Windows

To compile the code you can choose to use Docker and cross-compile from linux (this is what we do so it can be easily CI tested on GH) or a native compilation. 

## Docker based
1. Install Docker Desktop (https://docs.docker.com/desktop/setup/install/windows-install/)
2. Build the docker file
~~~
docker build -t beeline-fw-crosscompile -f emulator\windows-crosscompile.dockerfile .
~~~
3. You need to run `make PLATFORM=WINDOWS` inside the docker image while mounting the local filesystem inside docker. To do this run.

~~~
docker run --rm -v <path_to_repo>:/beeline -w /beeline/emulator beeline-fw-crosscompile make PLATFORM=WINDOWS
~~~

## Native / WSL
You can use a compiler of your choice. Given that we use the ARM GNU toolchain for firmware, we'd suggest using GCC. If you want to run the binaries created in WSL on Windows, you'll have to use a cross compiler (sudo apt-get install mingw-w64) (See the dockerfile for reference)

## Python
You'll need Python (tested on v3.12 but others should work just as well). We use pygame as a GUI for the emulator. You could install python in WSL and forward X-Server calls from WSL to Windows but we suggest just installing python / pyenv natively. 

once you have python and pip, 

~~~
pip install -r requirements.txt
~~~
should be enough to install pygame and numpy. 


