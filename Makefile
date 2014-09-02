test: bcrb.o
bcrb.o: bcrb.h
clean:
	rm -f test bcrb.o
dist: clean
	cd .. ; tar czvf bcrb.tar.gz bcrb/
