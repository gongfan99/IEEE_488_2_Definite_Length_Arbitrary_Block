# Build and parse IEEE 488.2 Definite Length Arbitrary Block
Build and parse IEEE 488.2 Definite Length Arbitrary Block with C++11. It has been verified on Windows 7.

Each Definite Length Arbitrary Block is of the form: #<Num_digits><Num_bytes><Data>

# build example
```c
// Windows 7, VS 2015 installed
git clone https://github.com/gongfan99/IEEE_488_2_Definite_Length_Arbitrary_Block.git
cd test
build
test
```

# usage
```c
using dataType = double;
std::vector<dataType> in = {1.0, 2.0, 3.0};
std::string binaryBlock = build_IEEE_488_2_Definite_Length_Arbitrary_Block(in);
std::vector<dataType> out = parse_IEEE_488_2_Definite_Length_Arbitrary_Block<dataType>(binaryBlock);
assert(in == out);
```
More usage cases can be found in test/test.cc

# Reference
http://literature.cdn.keysight.com/litweb/pdf/ads2001/vee6at/appxA10.html