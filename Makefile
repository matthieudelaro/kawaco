GTEST_DIR=../../googletest-master/googletest

test: test.c++ utils.cpp kawako.cpp
	# g++ -isystem  ${GTEST_DIR}/include -pthread test.c++ kawakoDisplay.cpp libgtest.a -o unittest -DTEST_ON_LAPTOP
	g++ -isystem  ${GTEST_DIR}/include -pthread test.c++ utils.cpp kawako.cpp libgtest.a -o unittest -DTEST_ON_LAPTOP
	./unittest

csv:
	g++ -nostdinc -x c -E main/main.ino -DTEST_ON_LAPTOP -DGENERATE_CSV | grep COM28 | bash > vlcSyncFiles/auto.csv


