.PHONY: run release debug clean

run: release
	./build/test

release:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j $(nproc)

debug:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build -j $(nproc)

clean:
	rm -rf build