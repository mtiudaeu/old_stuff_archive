//#define DEBUG

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>


int main()
{
  std::string name;
  std::vector<int> code;
  std::cin >> name;

  {
    std::string codeString;
    std::cin.ignore();
    std::getline(std::cin,codeString);

    int nb;
    std::stringstream ss(codeString);
    while(ss >> nb) {
      code.push_back(nb);
    }
  }

  std::string result;
  const std::size_t minResultSize = 8;
  const std::size_t codeSize = code.size();
  const std::size_t nameSize = name.size();
  for(std::size_t i = 0; result.size() < minResultSize; ++i) {
    const std::size_t nameIndex = i % nameSize;
    const std::size_t codeIndex = i % codeSize;
#ifdef DEBUG
    char c = name[nameIndex];
    int nb = code[codeIndex];
    std::cout << c << nb << static_cast<char>(c + nb - 1) << std::endl;
#endif
    auto result_char = [&]() {
      static int maxChar = 'z';
      int k = name[nameIndex]+ (code[codeIndex]-1);
      if(k>maxChar) {
        k = 'a' + k - maxChar -1;
      }
      return k;
    };
    result.push_back(result_char());
  }

  result[0] = toupper(result[0]);
  result.push_back('!');
  std::cout << result << std::endl;
}
