#include "Deform_rotation.h"

// VERSION RGBA
ImageRGBAu8 DeformRotation::RotateRBGAAreaMapping(const ImageRGBAu8 img, double angle){

    ImageRGBAu8 newImg;
    newImg.initItk  (img.width()*2, img.height()*2, false);
    newImg.setCenter((int)((newImg.width()*(newImg.height()/newImg.width()))/2.), (int)(newImg.height()/2.));
    std::cout << newImg.getCenter()[0] << " " << newImg.getCenter()[1] << std::endl;


    typedef std::pair<int, int> Coords;
    int subPixels = 32;

    double rad = -angle*M_PI/180;
    Matrix rotMat;
    rotMat[0][0] = std::cos(rad);
    rotMat[0][1] = - std::sin(rad);
    rotMat[1][0] = std::sin(rad);
    rotMat[1][1] = std::cos(rad);

    // For each pixel in the output image
    for(uint32_t j = 0; j < newImg.height(); j++){

        for(uint32_t i = 0; i < newImg.width(); i++){
            //std::cout << "IDXs : " << i << "," << j << std::endl;
            // Calculate its transformed real position (i.e source pixel in img) using invert rotation

            double newR = 0., newG = 0., newB = 0.;
            //double realX = rotMat[0][0]*i + rotMat[0][1]*j;
            //double realY = rotMat[1][0]*j + rotMat[1][1]*i;

            double realX = newImg.getCenter()[0] + (i - newImg.getCenter()[0])*std::cos(-angle) - (j - newImg.getCenter()[1])*std::sin(-angle);
            double realY = newImg.getCenter()[1] + (i - newImg.getCenter()[0])*std::sin(-angle) + (j - newImg.getCenter()[1])*std::cos(-angle);

            newImg.pixelAbsolute(i,j) = RGBAu8(0,0,0,0);

            if( realX >= 0 && realY >= 0 && (int)realX < img.width()-1 && (int)realY < img.height()-1 ){

                //std::cout << "REAL : " << realX << "," << realY << std::endl;

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
                //std::cout << "--- Rx : " << Rx << std::endl;
                uint32_t x = (int)(Rx/(1./subPixels));
                double Ry = realY - c0.second;
                //std::cout << "--- Ry : " << Ry << std::endl;
                uint32_t y = (int)(Ry/(1./subPixels));
                //std::cout << "------ Point (" << x << "," << y << ") :" << std::endl;

                //std::cout << "P0 : " << (int)img.pixelAbsolute(c0.first, c0.second).GetRed() << " , " << (uint32_t)img.pixelAbsolute(c0.first, c0.second).GetGreen() << " ," << (int)img.pixelAbsolute(c0.first, c0.second).GetBlue() << std::endl;
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


                /*
                if( (int)realX+1 >= img.width() && (int)realY+1 >= img.height() ){



                }
                */

            }



        }
    }

    return newImg;


}



/*
template<typename IMG_T>
static IMG_T DeformRotation::RotateRBGA(const IMG_T img){

}*/


int main(int argc, char* argv[]){


    ImageRGBAu8 image, image_out;

    //bool ok = image.load(TEMPO_PATH+"single-leaf-photo-03.jpg");
    bool ok = image.load(TEMPO_PATH+"quilting_input8.png");
    if (!ok)
        return 1;

    image_out = DeformRotation::RotateRBGAAreaMapping(image, 70.);

    image_out.save(TEMPO_PATH+"Deform-quilt.png");

    return 0;
}
