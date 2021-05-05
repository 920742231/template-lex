all : out
	$(MAKE) -C test

out :
	$(MAKE) -C object

clean :
	$(MAKE) clean -C object