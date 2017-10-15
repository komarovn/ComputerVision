#pragma once

#include <vcclr.h>
#include <string>
#include "opencv\cv.h"
#include "opencv2\core\mat.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace std;

class ImageProcessing abstract
{
protected:
    gcroot <Bitmap^> sourceImage;
    gcroot <Bitmap^> outputImage;
    virtual Color CalculateNewPixelColor(int x, int y) = 0;
public:
    int Clamp(int value, int max, int min)
    {
        if (value > max) return max;
        if (value < min) return min;
        return value;
    }

public:
    ImageProcessing(Bitmap ^image)
    {
        sourceImage = image;
    }

public:
    Bitmap^ ProcessImage()
    {
        Bitmap^ resultImage = gcnew Bitmap(sourceImage->Width, sourceImage->Height);
        for (int i = 0; i < sourceImage->Width; i++)
        {
            for (int j = 0; j < sourceImage->Height; j++)
            {
                resultImage->SetPixel(i, j, CalculateNewPixelColor(i, j));
            }
        }
        return resultImage;
    }

    Bitmap ^OutputImage() {
        return outputImage;
    }

};