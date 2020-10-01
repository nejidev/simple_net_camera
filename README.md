# simple net camera

	linux v4l2 net camera, capture jpg update http server

# 远程网络摄像头，放在家里，方便远程查看。

## 为什么不买一个？
	买到不全开源，不放心，不安全，也容易泄漏隐私。
	所以有了此项目，全开源，还可以方便移值到各种平台上。
	支持所有 linux 平台，Openwrt Raspberry

# pre depend library
	sudo apt-get install libcurl4-openssl-dev v4l-utils

# support Openwrt Raspberry ubuntu debain all linux machine
	build by c++11 using libcurl
	support v4l2 UVC camera only MJPEG

# usage & build
	make
	run
	./simple_net_camera or
	./simple_net_camera /dev/video0 or
	./simple_net_camera /dev/video0 https://192.168.1.100/php/cap_upload.php
