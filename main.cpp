#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include <cctype>
#include <cstdio>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <regex>
#include <memory>
#include <ranges>
#include <string_view>
#include <filesystem>

static int IndentationCheck=0;

class RegexPatterns{
    public:
        void RegisterRe(std::string re){
            this->_RePatterns.push_back(re);
        }
        std::vector<std::string> _RePatterns;
};

class PrototypeAST {
  public:
    std::string Name;
    std::vector<std::string> Args;
};
namespace std{
std::ostream & operator<< (std::ostream &out,std::smatch& base_match){
    for (auto v : base_match){
        out << "base_match.str(x) " << v.str() << std::endl;
    }
    return out;
}
}
std::unique_ptr<PrototypeAST> ParsePrototype(std::string& code) {
    std::cout << "ParsePrototype " << code << std::endl;
    std::string pattern = R"(\s+(\w+)\s*\(\s*([^)]+?)\s*\):)";
    std::smatch base_match;
    if(std::regex_search(code,base_match,std::regex(pattern))){
        std::cout << base_match;
    }

    // https://hczhcz.github.io/2014/08/19/a-detail-of-the-difference-between-g++-and-clang++.html
    //return nullptr;

    //https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2210r0.html
    PrototypeAST Function;
    Function.Name = base_match.str(0);
    std::cout << "Function Name " << Function.Name << std::endl;
    for (const auto&& param : std::string_view(base_match.str(2))|std::views::split(',')){

        // 这代码在VC下测试可以编译，clang编译不了
        //std::cout << "Function Arg " << std::string_view({ param.begin(),param.end() }) << std::endl;
        
        // 将就一下这样写
        std::string t;
        for(auto c : param){
            t.push_back(c);
        }
        Function.Args.push_back(t);
        std::cout << "Function Arg " << t << std::endl;
    }
    return std::make_unique<PrototypeAST>(Function);
}


int main(int argc,char* argv[]){
    if (argc != 2)
    {
        std::cout << "arguments must be 2\n";
        return 0;
    }

    if (!std::filesystem::exists(argv[1])){
        std::cout << "file " << argv[1] << "is not exist\n";
        return 0;
    }
    
    std::ifstream source(argv[1]);
    std::string code;

    RegexPatterns re;
    re.RegisterRe(R"(\b^(if|else|for|while|def)\b)");

    while(std::getline(source,code)){
        std::cout << code << std::endl;
        std::smatch base_match;
        for(auto pattern:re._RePatterns){
            if (std::regex_search(code,base_match,std::regex(pattern))){
                std::cout << base_match;
                std::string token = base_match.str(0);
                if (token == "def"){
                    ParsePrototype(code);
                    //ParseExpression();
                }
            }
        }
    }


    return 0;
}