
#include "Deform_common.h"
#include <array>

using namespace ASTex;

class DeformCommon;


class DeformRotation {

public:
    DeformRotation();

    static ImageRGBAu8 RotateRBGAAreaMapping(const ImageRGBAu8 img, double angle);
    static ImageRGBAu8 resizeImage(const ImageRGBAu8 img);
};
