project01: main.cpp
	g++ -o project01 main.cpp

.PHONY: test
test: project01
	./project01 test.txt

.PHONY: test-corpus
test-corpus: project01
	cat /tmp/corpus/*.txt | ./project01 -

project01.js: main.cpp
	emcc -o project01.js main.cpp

.PHONY: clean
clean:
	rm -f ./project01
