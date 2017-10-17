all:

	+$(MAKE) -C client
	+$(MAKE) -C server
	cp client/sriClient sriClient
	cp server/sriServer sriServer
	
clean:

	rm sriClient
	rm sriServer
	rm client/sriClient
	rm server/sriServer
	

