#include <string>
#include <vector>
#include <xmmintrin.h>

class Generator {       // The class
  private:
    int seed;
    double min;
    double max;
  public:             // Access specifier
    Generator(int seed, double min, double max);
    double gen();
    std::vector<double> gen_vector(int length);
};

void normalize(std::vector<double> &x);

void normalize_r(std::vector<double> &x);

void normalize_xmm(std::vector<double> &x);