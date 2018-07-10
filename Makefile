cc=g++
prom = code
deps = main.cpp
$(prom):$(deps)
	g++ -o $(prom) $(deps) -O2 -std=c++17 -march=corei7-avx

clean:
	rm -rf $(prom)
