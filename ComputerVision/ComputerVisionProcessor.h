#pragma once

#include "ImageProcessing.h"

class ComputerVisionProcessor {
private:
    cv::Mat image;
    cv::Mat integralImage;
    cv::Mat currentImage;

    Bitmap^ convertImageToBitmap() {
        gcroot<Bitmap^> output = gcnew Bitmap(image.cols, image.rows);
        int red, green, blue, gray;
        Color c;
        uint8_t* pixelPtr = (uint8_t*) currentImage.data;

        for (int i = 1; i < image.cols; i++) {
            for (int j = 1; j < image.rows; j++) {
                if (currentImage.channels() == 3) {
                    red = currentImage.at<cv::Vec3b>(j, i)[2];
                    green = currentImage.at<cv::Vec3b>(j, i)[1];
                    blue = currentImage.at<cv::Vec3b>(j, i)[0];
                    c = Color::FromArgb(red, green, blue);
                } else {
                    gray = pixelPtr[image.cols * currentImage.channels() * j + i * currentImage.channels()];
                    c = Color::FromArgb(gray, gray, gray);
                }
                output->SetPixel(i, j, c);
            }
        }

        return output;
    }

    cv::Mat gaussianBlur(const cv::Mat &source) {
        cv::Mat dest;
        cv::GaussianBlur(source, dest, cv::Size(7, 7), 1.4, 1.4);
        return dest;
    }

    cv::Mat applyThresholds(const cv::Mat &source, int minThreshold, float maxThreshold) {
        cv::Mat result = cv::Mat(source.rows, source.cols, CV_8UC1);

        for (int i = 1; i < source.rows - 1; i++) {
            for (int j = 1; j < source.cols - 1; j++) {
                uchar pixel = source.at<uchar>(i, j);

                if (pixel < minThreshold) {
                    pixel = 0;
                } else if (pixel >= minThreshold && pixel < maxThreshold) {
                    pixel = 127;
                } else if (pixel >= maxThreshold) {
                    pixel = 255;
                }

                result.at<uchar>(i, j) = pixel;
            }
        }

        return result;
    }

    cv::Mat checkNeighbors(cv::Mat &source) {
        for (int i = 1; i < source.rows - 1; i++) {
            for (int j = 1; j < source.cols - 1; j++) {
                if (source.at<uchar>(i, j) == 127) {
                    cv::Point c2 = cv::Point(j, i - 1);
                    cv::Point c3 = cv::Point(j + 1, i - 1);
                    cv::Point c4 = cv::Point(j + 1, i);
                    cv::Point c5 = cv::Point(j + 1, i + 1);
                    cv::Point c6 = cv::Point(j, i + 1);
                    cv::Point c7 = cv::Point(j - 1, i + 1);
                    cv::Point c8 = cv::Point(j - 1, i);
                    cv::Point c9 = cv::Point(j - 1, i - 1);

                    if (source.at<uchar>(c2) == 255 || source.at<uchar>(c3) == 255 ||
                            source.at<uchar>(c4) == 255 || source.at<uchar>(c5) == 255 ||
                            source.at<uchar>(c6) == 255 || source.at<uchar>(c7) == 255 ||
                            source.at<uchar>(c8) == 255 || source.at<uchar>(c9) == 255) {
                        source.at<uchar>(i, j) = 255;
                    } else {
                        source.at<uchar>(i, j) = 0;
                    }
                }
            }
        }
        return source;
    }

    cv::Mat applyNonMaximumSuppression(const cv::Mat &source, const cv::Mat &angles, const cv::Mat &gradients) {
        cv::Mat result = cv::Mat(source.rows, source.cols, CV_8UC1);

        for (int i = 1; i < source.rows - 1; i++) {
            for (int j = 1; j < source.cols - 1; j++) {
                cv::Point c = cv::Point(j, i);
                cv::Point c2 = cv::Point(j, i - 1);
                cv::Point c3 = cv::Point(j + 1, i - 1);
                cv::Point c4 = cv::Point(j + 1, i);
                cv::Point c5 = cv::Point(j + 1, i + 1);
                cv::Point c6 = cv::Point(j, i + 1);
                cv::Point c7 = cv::Point(j - 1, i + 1);
                cv::Point c8 = cv::Point(j - 1, i);
                cv::Point c9 = cv::Point(j - 1, i - 1);

                if (((angles.at<uchar>(c) == 0) && (gradients.at<uchar>(c) >= gradients.at<uchar>(c8)) && (gradients.at<uchar>(i, j) >= gradients.at<uchar>(c4))) ||
                        ((angles.at<uchar>(c) == 45) && (gradients.at<uchar>(c) >= gradients.at<uchar>(c3)) && (gradients.at<uchar>(i, j) >= gradients.at<uchar>(c7))) ||
                        ((angles.at<uchar>(c) == 90) && (gradients.at<uchar>(c) >= gradients.at<uchar>(c2)) && (gradients.at<uchar>(i, j) >= gradients.at<uchar>(c6))) ||
                        ((angles.at<uchar>(c) == 135) && (gradients.at<uchar>(c) >= gradients.at<uchar>(c9)) && (gradients.at<uchar>(i, j) >= gradients.at<uchar>(c5)))) {
                    result.at<uchar>(c) = gradients.at<uchar>(c);
                }
                else {
                    result.at<uchar>(c) = 0;
                }
            }
        }

        return result;
    }

    void calculateAnglesAndGradients(const cv::Mat &source, cv::Mat &angles, cv::Mat &gradients) {
        for (int i = 1; i < source.rows - 1; i++) {
            for (int j = 1; j < source.cols - 1; j++) {
                uchar p2 = source.at<uchar>(i - 1, j);
                uchar p3 = source.at<uchar>(i - 1, j + 1);
                uchar p4 = source.at<uchar>(i, j + 1);
                uchar p5 = source.at<uchar>(i + 1, j + 1);
                uchar p6 = source.at<uchar>(i + 1, j);
                uchar p7 = source.at<uchar>(i + 1, j - 1);
                uchar p8 = source.at<uchar>(i, j - 1);
                uchar p9 = source.at<uchar>(i - 1, j - 1);

                int Gx = (p3 + 2 * p4 + p5) - (p9 + 2 * p8 + p7);
                int Gy = (p9 + 2 * p2 + p3) - (p7 + 2 * p6 + p5);

                float G = sqrt((float)Gx*Gx + Gy*Gy);

                float dir = (atan2((float)Gy, Gx) / CV_PI) * 180.0f;
                if (((dir < 22.5) && (dir >= -22.5)) || (dir >= 157.5) || (dir < -157.5)) {
                    dir = 0;
                }
                if (((dir >= 22.5) && (dir < 67.5)) || ((dir < -112.5) && (dir >= -157.5))) {
                    dir = 45;
                }
                if (((dir >= 67.5) && (dir < 112.5)) || ((dir < -67.5) && (dir >= -112.5))) {
                    dir = 90;
                }
                if (((dir >= 112.5) && (dir < 157.5)) || ((dir < -22.5) && (dir >= -67.5))) {
                    dir = 135;
                }

                gradients.at<uchar>(i, j) = G;
                angles.at<uchar>(i, j) = dir;
            }
        }
    }
public:
    ComputerVisionProcessor(std::string filepath) {
        image = cv::imread(filepath, CV_LOAD_IMAGE_COLOR);
        image.copyTo(currentImage);
    }

    Bitmap^ makeGrayscale() {
        if (currentImage.channels() > 2) {
            cv::Mat* result = new cv::Mat(image.rows, image.cols, currentImage.type());
            cv::cvtColor(currentImage, *result, CV_BGR2GRAY);
            currentImage = *result;
        }
        return convertImageToBitmap();
    }

    Bitmap^ makeCannyEdgeDetection() {
        currentImage = gaussianBlur(currentImage);
        cv::Mat result;
        cv::Mat angles = cv::Mat(image.rows, image.cols, CV_8UC1);
        cv::Mat gradients;
        result = gaussianBlur(currentImage);
        result.copyTo(gradients);
        calculateAnglesAndGradients(result, angles, gradients);
        result = applyNonMaximumSuppression(result, angles, gradients);
        result = applyThresholds(result, 10, 50);
        result = checkNeighbors(result);
        result.copyTo(currentImage);
        return convertImageToBitmap();
    }

    Bitmap^ makeAveragingFilter() {
        cv::Mat result, invertedImage, distTransformedImage, integralImage;
        cv::threshold(currentImage, invertedImage, 0, 255, CV_THRESH_BINARY_INV);
        cv::distanceTransform(invertedImage, distTransformedImage, CV_DIST_L2, 3);
        cv::integral(image, integralImage);
        distTransformedImage.convertTo(distTransformedImage, CV_8U);
        image.copyTo(result);

        int rows = distTransformedImage.rows;
        int cols = distTransformedImage.cols;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int factor = (int) distTransformedImage.at<uchar>(i, j);
                if (factor % 2 != 1) {
                    factor++;
                }
                int halfFactor = factor / 2;

                while ((i  - halfFactor < 0) || (i + halfFactor + 1 > rows) ||
                    (j - halfFactor < 0) || (j + halfFactor + 1 > cols)) {
                    factor -= 2;
                    halfFactor = factor / 2;
                }

                if (factor > 0) {
                    result.at<cv::Vec3b>(i, j) = (integralImage.at<cv::Vec3i>(i - halfFactor, j - halfFactor) -
                        integralImage.at<cv::Vec3i>(i - halfFactor , j + halfFactor + 1) - 
                        integralImage.at<cv::Vec3i>(i + halfFactor + 1, j - halfFactor) +
                        integralImage.at<cv::Vec3i>(i + halfFactor + 1, j + halfFactor + 1)) / (factor * factor);
                }
            }
        }

        result.copyTo(currentImage);
        return convertImageToBitmap();
    }

    void buildIntegralImage() {
        integralImage = cv::Mat(image.rows, image.cols, currentImage.type());
        for (int i = 0; i < image.cols; i++) {
            for (int j = 0; j < image.rows; j++) {
                int value = 0;
                for (int iOrig = 0; iOrig <= i; iOrig++) {
                    for (int jOrig = 0; jOrig <= j; jOrig++) {
                        value += currentImage.at<uchar>(jOrig, iOrig);
                    }
                }
                integralImage.at<uchar>(j, i) = value;
            }
        }
    }
};