

#include "Deform_common.h"


using namespace ASTex;


class DeformStretch {

public:
    DeformStretch();

    static ImageRGBAu8 lateralStretch(ImageRGBAu8 img, double coef);
};
