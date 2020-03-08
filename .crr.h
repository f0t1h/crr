
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;
using std::unordered_map;
using std::variant;

using std::getline;
using std::istringstream;
using std::ifstream;


class dataframe_o{
    public:
    unordered_map<string, vector<int> > data;
    vector<string> header;
    string path;


    dataframe_o() {}
    dataframe_o(const string &path) : path(path) {
        ifstream f(path);
        string line;
        string col;
        int value;
        getline(f, line);
        istringstream is(line);

        while( is >> col){
            header.push_back(col);
        }
        for(const auto &col : header){
            data[col];
        }

        while(getline(f,line)){
            istringstream is(line);
            int index = 0;
            while( is >> value){

                data[header[index++]].push_back(value);
            }
        }
    }
};

dataframe_o dataframe(const string &path){
    dataframe_o o(path);
    return o;
}

int max_of(std::vector<int> vals){
    int max = vals[0];
    for(int i : vals){
        if( i > max){
            max = i;
        }
    }
    return max;
}

int min_of(std::vector<int> vals){
    int min = vals[0];
    for(int i : vals){
        if( i < min){
            min = i;
        }
    }
    return min;
}

class ascii_art_o{
    public:
    void operator() (dataframe_o *frame){
        size_t s = frame->data[frame->header[0]].size();

        int x_max = 1 + max_of(frame->data[frame->header[0]]);
        int y_max = 1 + max_of(frame->data[frame->header[1]]);
        int mx[x_max][y_max]  = { 0};
        for(int i =0; i<s;i++){
            int xx = frame->data[frame->header[0]][i];
            int yy = frame->data[frame->header[1]][i];

            mx[xx][yy] = 1;
        }
        std::cout << frame->header[1] << "\n";
        std::cout << "^\n";
        for(int i = y_max-1; i>0;i--){
            std::cout << "│";
            for(int j =1; j<x_max;j++){
                std::cout << (mx[j][i] ? "o" : " ");
            }
            std::cout << "\n";
        }
        std::cout << "└";
        for(int j =0; j<x_max;j++){
            std::cout << "─";
        }
        std::cout << "> " << frame->header[0] << "\n";
    }
};

ascii_art_o ascii_art_f(){
    ascii_art_o o;
    return o;
};

class ggplot_o{
    dataframe_o *dat;
    public:

    ggplot_o() {}
    ggplot_o(dataframe_o &f) : dat(&f) {}
    template < class format_type>
    void operator +( format_type fmt){
        fmt(dat);
    }
};

ggplot_o ggplot(dataframe_o f){
    ggplot_o o(f);
    return o;
}

#define library(title) int main(int argc, char **args){
#define exit(num) return num;}
#define ascii_art() ascii_art_f();

