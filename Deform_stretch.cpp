
#include "Deform_stretch.h"

ImageRGBAu8 DeformStretch::lateralStretch(ImageRGBAu8 img, double coef){

    ImageRGBAu8 newImg;
    newImg.initItk  ((int)(img.width()*2), img.height(), true);

    double coefTemp;
    double ratio = coef /(img.width()/2);
    int stretchedPixels = 0;
    int stretchingPerPixel = 0;

    for(uint32_t j = 0; j < img.height(); j++){

        coefTemp = 0.;
        stretchedPixels = 0;
        stretchingPerPixel = 0;

        for(uint32_t i = 0; i < img.width()/2; i++){

            coefTemp += ratio;
            ImageRGBAu8::PixelType p = img.pixelAbsolute(i,j);
            p[3] = 255;

            if (coefTemp - (int)coefTemp >= 0.5){
                stretchingPerPixel = (int)coefTemp + 1;
            }
            else{
                stretchingPerPixel = (int)coefTemp;
            }

            for(uint32_t k = 0; k <= stretchingPerPixel; k++){

                newImg.pixelAbsolute(i + stretchedPixels + k, j) = p;

            }
            stretchedPixels += stretchingPerPixel;
        }

        for(uint32_t i = img.width()/2; i < img.width(); i++){

            coefTemp -= ratio;
            ImageRGBAu8::PixelType p = img.pixelAbsolute(i,j);
            p[3] = 255;

            if (coefTemp - (int)coefTemp >= 0.5){
                stretchingPerPixel = (int)coefTemp + 1;
            }
            else{
                stretchingPerPixel = (int)coefTemp;
            }

            for(uint32_t k = 0; k <= stretchingPerPixel; k++){

                newImg.pixelAbsolute(i + stretchedPixels + k, j) = p;

            }
            stretchedPixels += stretchingPerPixel;
        }
    }

    return newImg;
}

int main(int argc, char* argv[]){

    ImageRGBAu8 image, image_out;

    bool ok = image.load(TEMPO_PATH+"single-leaf-photo-03.jpg");
    //bool ok = image.load(TEMPO_PATH+"quilting_input8.png");
    if (!ok)
        return 1;

    image_out = DeformStretch::lateralStretch(image, 2.5);

    image_out.save(TEMPO_PATH+"Deform-leaf-stretch.png");

    return 0;
}
