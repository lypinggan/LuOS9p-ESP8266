9luOS
----

It is an OS for ESP8266 mk based on Lua-RTOS-ESP8266. But it was significally changed, checked and expanded.

Features
========
* Lua 5.3.4,
* Multithreading using Free-RTOS,
* UART (ttl rs232, com port),
* I2C,
* SPI,
* 16 PWMs (hardware),
* 16 PIOs,
* 5 ADC (3 up to 3v + 1 up to 1v + 1 used internally),
* 1 DAC (up to 3v),
* OLED screen,
* Keyboard with 8 buttons (cursor + 3 special + 1 external),
* GUI (menu based. using OLED and keyboard),
* Shell by remote (rs232) terminal,
* internal Editor of files (remote terminal),
* big set of easy to use shell commands (filesystem, ports, devices, network, server, other)
* net Ap and Sta modes,
* one command WiFi Sta search,
* PID regulator module,

* Webserver (with compressing):
  1. static HTML,
  1. CGI,
  1. WebSock (up to 5 connections),
  1. direct access to devices/ports using WebSock,

* 9p2000/Styx (Plan9/Inferno os) network virtual file system + server (currently in the work). What the functionality is work now:
  1. list of devices (ex: **ls /n/dev**),
  1. list of device's functions (ex: **cat /n/dev/pwm**),
  1. call a device's function (ex: **echo ch_val -i 5 -n 0.4 > /n/dev/pwm**),
  1. read the result of the call of the device's function (ex: **cat /n/dev/pwm** immediatly after the call),
  1. read a file/folder list from the fisical filesystem of the device (ex: **ls /n/fs/html/js**),
  1. read/write/create files on the fisical device's filesystem (ex: **cat /n/fs/html/js/common.js**; **echo a Text > /n/fs/afile.txt**),
  1. create new folders  on the fisical device's filesystem (ex: **mkdir /n/fs/new_folder**),
  1. remove files or folders from the fisical device's filesystem (ex: **rm /n/fs/html/a_file_or_folder**).

* PC-Studio for easyer upload/download/edit files into the luOS embedded filesystem. also PC-Studio contains remote terminal for luOS and (in the future) other features.

How to build
============
```
./bld.sh
./flash.sh
```

How to build PC-Studio
======================
```
cd pc-studio
./bld.sh
```

How to run PC-Studio
====================
```
cd pc-studio/bld
./prg
```

How to run Styx server
======================
In the device's console or from the lua script:
```
thread.start( styx.loop )
```
Or just select **Styx server** on the device's gui.

In the inferno shell/script/limbo etc:
```
mount -A tcp!<IP address of your device>!6701 /n
```
Now your device's styx virtual fs, including devices and fisical filesystem, is mounted to **/n** folder. Use it as a regular filesystem.
