
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <sstream>
#include <functional>
#include <memory>
#include <cmath>

using std::string;
using std::vector;
using std::unordered_map;
using std::variant;

using std::getline;
using std::istringstream;
using std::ifstream;

using std::shared_ptr;


class dataframe_o{
    public:
    unordered_map<string, vector<double> > data;
    vector<string> header;
    string path;


    dataframe_o() {}
    dataframe_o(const string &path) : path(path) {
        ifstream f(path);
        string line;
        string col;
        double value;
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
    ~dataframe_o(){
        std::cerr << "Dataframe removed!\n";
    }
};
typedef shared_ptr<dataframe_o> dfp;

dfp dataframe(const string &path){
    return std::make_shared<dataframe_o>(path);
}

template<class TYPE>
TYPE max_of(std::vector<TYPE> vals){
    TYPE max = vals[0];
    for(TYPE i : vals){
        if( i > max){
            max = i;
        }
    }
    return max;
}

template<class TYPE>
TYPE min_of(std::vector<TYPE> vals){
    TYPE min = vals[0];
    for(TYPE i : vals){
        if( i < min){
            min = i;
        }
    }
    return min;
}


class ggplot_o{
    dfp dat;
    vector< vector< string>> canvas;
    vector<string> palette;
    std::function<void(string)> printer;
    public:

    ggplot_o() {}
    ggplot_o(dfp f) : dat(f), printer([](string mock){}) {}
    ~ggplot_o(){
        draw_canvas();

    }
    void draw_canvas(){

        for(auto outer = canvas.rbegin(); outer != canvas.rend(); ++outer){
            //for(const vector<char> &vc : canvas){
            auto &vc = *outer;
            for( auto iter = vc.begin(); iter != vc.end(); ++iter){
                 
                printer(*iter);
            }
            printer("\n");
        }
        std::cerr << "Draw Done!\n";
    }

    friend class ascii_art_o;
    friend class geom_line_o;

    friend class geom_histogram_o;
    friend class style_o;
};
typedef shared_ptr<ggplot_o> ggp;

template < class format_type>
ggp operator +( ggp p, shared_ptr<format_type> fmt){
    (*fmt)(p);
    return p;
}


class ascii_art_o{
    public:
    ggp plotter;
    void operator() (ggp plot_ptr ){
        plotter = plot_ptr;
    }
   ~ascii_art_o(){
    }
};
typedef shared_ptr<ascii_art_o> aap;

aap ascii_art(){
    return std::make_shared<ascii_art_o>();
};

class geom_line_o{

    public:
    ggp plotter;
    geom_line_o(ggp plotter) : plotter(plotter){
 
    }
    ~geom_line_o(){ // TODO
        dfp frame = plotter->dat;
        auto &canvas = plotter->canvas;
        size_t s = frame->data[frame->header[0]].size();

        int x_max = 1 + max_of(frame->data[frame->header[0]]);
        int y_max = 1 + max_of(frame->data[frame->header[1]]);
        double mx[x_max][y_max]  = { 0};
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
typedef shared_ptr<geom_line_o> linep;

class geom_histogram_o{

    ggp plotter;
    int binc;
    int height;
    public:
    geom_histogram_o() : binc(30), height(20){
    }
// TODO move drawing to styler;;;;
    ~geom_histogram_o(){
        std::cerr << "Geom Histogram Done!" << binc << "\n";
        dfp df = plotter->dat;
        string FULLC = "█", TTHC = "▄", OTHC = "_";
        
        for(int i = 0; i< height; ++i){
            plotter->canvas.emplace_back(1+binc, " ");
        }
        for(const auto &s_and_v : df->data){
            const string &title = s_and_v.first;
            const vector<double> &veri = s_and_v.second;

            vector<int> bins;
            bins.resize(binc+1,0);
            int mx =  max_of(df->data[title]);
            int mn =  min_of(df->data[title]);
            for(double v : veri){
                int index = static_cast<int>((binc*(v - mn + 1)/(1+mx-mn)));
                bins[index]+=1;
            }
            


            for( int i = 0; i <= binc; ++i){

                int j = 0;
                while( bins[i] > 0){
                    if(bins[i] > 2){
                        plotter->canvas[j][i] = FULLC;
                        bins[i]-=3;
                        j+=1;
                    }
                    else if(bins[i] >1){
                        plotter->canvas[j][i] = TTHC;
                        bins[i]-=2;
                        j+=1;
                    }
                    else if(bins[i] > 0){
                        plotter->canvas[j][i] = OTHC;
                        bins[i]-=1;
                        j+=1;
                    }
                }

            }
        }       
    }
    void operator() ( ggp plt){
        plotter = plt;
    }

};
typedef shared_ptr<geom_histogram_o> histp;

histp geom_histogram(){
    return std::make_shared<geom_histogram_o>();
}

class style_o{
    public:
    ggp plotter;
    vector<string> palette;
    std::function<void(string)> printer;

    style_o(){}
    style_o(std::function<void(string)> p) : printer(p){}
    ~style_o(){
        std::cerr << "Styling Done\n";
        plotter->palette = palette;
        plotter->printer = printer;
    }
    void operator() ( ggp plt){
        plotter = plt;
    }
   
};
typedef shared_ptr<style_o> styp;

styp basic_ascii_style(){
    return std::make_shared<style_o>([](string a){std::cout << a;});
}

ggp ggplot(dfp f){
    return std::make_shared<ggplot_o>(f);
}



