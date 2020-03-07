#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

using std::string;
using std::vector;
using std::unordered_map;
using std::variant;

class format_type{

};

class ggplot{
    public:
    void operator +( format_type fmt){
        
    }
};

class dataframe{
    unordered_map<string, vector<variant<int,double,string>>> data;
    public:
    dataframe() {}
    ggplot operator + (ggplot &plt){
        
    }
};

int main(int argc, char **argv){
    dataframe() + ggplot() + format_type();
    return 0;
}
