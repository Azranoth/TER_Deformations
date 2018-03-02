
#include "Deform_common.h"
#include <array>

using namespace ASTex;

typedef std::array<std::array<double,2>, 2> Matrix;


//template<typename IMG_T>
class DeformRotation {

public:
    DeformRotation();

    static ImageRGBAu8 RotateRBGAAreaMapping(const ImageRGBAu8 img, double angle);
    //static IMG_T RotateRBGA(const IMG_T img);
};
