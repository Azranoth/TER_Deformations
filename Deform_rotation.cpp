#include "Deform_rotation.h"

// VERSION RGBA
ImageRGBAu8 DeformRotation::RotateRBGAAreaMapping(const ImageRGBAu8 img, double angle){

    ImageRGBAu8 newImg;
    newImg.initItk  (img.width()+img.height(), img.height()+img.width(), false);
    newImg.setCenter( (int)(newImg.width()/2), (int)(newImg.height()/2) );

    std::cout << img.getCenter()[0] << " " << img.getCenter()[1] << std::endl;
    std::cout << newImg.getCenter()[0] << " " << newImg.getCenter()[1] << std::endl;

    typedef std::pair<int, int> Coords;
    int subPixels = 16;

    double rad = angle*M_PI/180;

    // For each pixel in the output image
    for(uint32_t j = 0; j < newImg.height(); j++){

        for(uint32_t i = 0; i < newImg.width(); i++){

            double newR = 0., newG = 0., newB = 0.;

            // Calculate its transformed real position (i.e source pixel in img) using invert rotation
            double realX = img.getCenter()[0] + (i - newImg.getCenter()[0])*std::cos(rad) - (j - newImg.getCenter()[1])*std::sin(rad);
            double realY = img.getCenter()[1] + (i - newImg.getCenter()[0])*std::sin(rad) + (j - newImg.getCenter()[1])*std::cos(rad);

            newImg.pixelAbsolute(i,j) = RGBAu8(0,0,0,0);

            if( realX >= 0 && realY >= 0 && (int)realX < img.width()-1 && (int)realY < img.height()-1 ){

                // Get the 4 partial covering pixels using the transformed coordinates of (realX,realY)
                Coords c0 = Coords( (uint32_t)realX  , (uint32_t)realY   );
                Coords c1 = Coords( (uint32_t)realX+1, (uint32_t)realY   );
                Coords c2 = Coords( (uint32_t)realX  , (uint32_t)realY+1 );
                Coords c3 = Coords( (uint32_t)realX+1, (uint32_t)realY+1 );

                ImageRGBAu8::PixelType p0 = img.pixelAbsolute(c0.first, c0.second);
                ImageRGBAu8::PixelType p1 = img.pixelAbsolute(c1.first, c1.second);
                ImageRGBAu8::PixelType p2 = img.pixelAbsolute(c2.first, c2.second);
                ImageRGBAu8::PixelType p3 = img.pixelAbsolute(c3.first, c3.second);

                // ----- Leptonica's RAM formula
                double Rx = realX - c0.first;
                uint32_t x = (int)(Rx/(1./subPixels));

                double Ry = realY - c0.second;
                uint32_t y = (int)(Ry/(1./subPixels));

                newR = (1./(subPixels*subPixels)) * ( ((subPixels - x)     * (subPixels - y) * p0.GetRed())
                                                          + (x            * (subPixels - y) * p2.GetRed())
                                                          + (y            * (subPixels - x) * p1.GetRed())
                                                          + (x            * y               * p3.GetRed()));

                newG = (1./(subPixels*subPixels)) * ( ((subPixels - x)     * (subPixels - y) * p0.GetGreen())
                                                          + (x            * (subPixels - y) * p2.GetGreen())
                                                          + (y            * (subPixels - x) * p1.GetGreen())
                                                          + (x            * y               * p3.GetGreen()));

                newB = (1./(subPixels*subPixels)) * ( ((subPixels - x)     * (subPixels - y) * p0.GetBlue())
                                                          + (x            * (subPixels - y) * p2.GetBlue())
                                                          + (y            * (subPixels - x) * p1.GetBlue())
                                                          + (x            * y               * p3.GetBlue()));

                //std::cout << "( " << newR << " , " << newG << " , " << newB << " )" << std::endl;

                newImg.pixelAbsolute(i,j) = RGBAu8(newR, newG, newB, 255);

            }
        }
    }

    return newImg;
}


ImageRGBAu8 DeformRotation::resizeImage(const ImageRGBAu8 img){


    int minX = -1, minY = -1, maxX = -1, maxY = -1;
    for(uint32_t j = 0; j < img.height(); j++){

        // ---- Get the min & max coordinates in the image
        for(uint32_t i = 0; i < img.width(); i++){

            ImageRGBAu8::PixelType p = img.pixelAbsolute(i, j);

            if( i < (uint32_t)minX && p.GetAlpha() > 0){
                minX = i;
            }
            if( maxX == -1 || (i > (uint32_t)maxX && p.GetAlpha() > 0)){
                maxX = i;
            }
            if( j < (uint32_t)minY && p.GetAlpha() > 0){
                minY = j;
            }
            if( maxY == -1 || (j > (uint32_t)maxY && p.GetAlpha() > 0)){
                maxY = j;
            }
        }
    }
    std::cout << "min && max : " << minX << "," << minY << " - " << maxX << "," << maxY << std::endl;

    ImageRGBAu8 resImg;
    resImg.initItk  (maxX-minX+1, maxY-minY+1, false);

    // ----- Extract the region between the min & max coordinates
    for(uint32_t j = minY; j <= maxY; j++){

        for(uint32_t i = minX; i <= maxX; i++){

            ImageRGBAu8::PixelType p = img.pixelAbsolute(i, j);
            resImg.pixelAbsolute(i-minX, j-minY) = p;
        }
    }

    // ----- Then return it as a new image
    return resImg;
}




/*
template<typename IMG_T>
static IMG_T DeformRotation::RotateRBGA(const IMG_T img){

}*/


int main(int argc, char* argv[]){


    ImageRGBAu8 image, image_out;

    bool ok = image.load(TEMPO_PATH+"single-leaf-photo-03.jpg");
    //bool ok = image.load(TEMPO_PATH+"quilting_input8.png");
    //bool ok = image.load(TEMPO_PATH+"simpleRGB.png");
    if (!ok)
        return 1;

    image.setCenter(image.width()/2, image.height()/2);

    image_out = DeformRotation::RotateRBGAAreaMapping(image, 25.);
    image_out = DeformRotation::resizeImage(image_out);

    image_out.save(TEMPO_PATH+"Deform-leaf25.png");

    return 0;
}
