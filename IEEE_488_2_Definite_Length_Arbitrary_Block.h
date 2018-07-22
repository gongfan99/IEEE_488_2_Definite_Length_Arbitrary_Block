#pragma once

#include <vector>
#include <string>

template<class T>
std::string build_IEEE_488_2_Definite_Length_Arbitrary_Block(const std::vector<T>& in){
  int byte_count = in.size() * sizeof(T);
  int num_digits = std::to_string(byte_count).size();
  std::string out = "#" + std::to_string(num_digits) + std::to_string(byte_count);
  out.resize(2 + num_digits + byte_count);
  std::memcpy((char*)out.data() + 2 + num_digits, in.data(), byte_count);
  return out;
}

template<class T>
std::vector<T> parse_IEEE_488_2_Definite_Length_Arbitrary_Block(const std::string& in){  
  if (in.substr(0, 1) != "#") throw std::runtime_error("Error: first character should be #.");
  
  int num_digits = std::stoi(in.substr(1, 1));
  int byte_count = std::stoi(in.substr(2, num_digits));
  if ((2 + num_digits + byte_count) > in.size()) throw std::runtime_error("Error: input length not match byte count field.");
  
  std::vector<T> out(byte_count / sizeof(T));
  std::memcpy((char*)out.data(), in.data() + 2 + num_digits, byte_count);
  return out;
}