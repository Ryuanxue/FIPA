
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/temp/nginx_install_64/sbin/nginx -t

	kill -USR2 `cat /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/temp/nginx_install_64/logs/nginx.pid`
	sleep 1
	test -f /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/temp/nginx_install_64/logs/nginx.pid.oldbin

	kill -QUIT `cat /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/temp/nginx_install_64/logs/nginx.pid.oldbin`
