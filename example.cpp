#include "crr.h"

int main(int argc, char **argv){

    ggplot(dataframe(argv[1]))  + geom_histogram() + basic_ascii_style();
    return 0;
}
