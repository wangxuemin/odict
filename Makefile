HARDWARE_PLATFORM := $(shell uname -m)
ifeq ($(HARDWARE_PLATFORM),x86_64)
	release=Makefile.64
else
	release=Makefile.32
endif

all : target

target :
	make -f $(release)

debug :
	make debug -f $(release)

clean :
	make clean -f $(release)

output :
	make output -f $(release)

cov :
	make cov -f $(release)

ccpc :
	make ccpc -f $(release)

test :
	make test -f $(release)

libodict.a:
	make libodict.a -f $(release)

test1:
	make test1 -f $(release)

test2:
	make test2 -f $(release)

test3:
	make test3 -f $(release)

test_odb_128:
	make test_odb_128 -f $(release)

test_odb_64:
	make test_odb_64 -f $(release)

mass_data_save_test:
	make mass_data_save_test -f $(release)
