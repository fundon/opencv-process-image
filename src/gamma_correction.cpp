//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>

using namespace cv;

void MyGammaCorrection(Mat& src, Mat& dst, float fGamma)  
{  
    CV_Assert(src.data);  
  
    // accept only char type matrices  
    CV_Assert(src.depth() != sizeof(uchar));  
  
    // build look up table  
    unsigned char lut[256];  
    for( int i = 0; i < 256; i++ )  
    {  
        lut[i] = saturate_cast<uchar>(pow((float)(i/255.0), 1.f/fGamma) * 255.0f);  
    }  
  
    dst = src.clone();  
    const int channels = dst.channels();  
    switch(channels)  
    {  
        case 1:  
            {  
  
                MatIterator_<uchar> it, end;  
                for( it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++ )  
                    //*it = pow((float)(((*it))/255.0), fGamma) * 255.0;  
                    *it = lut[(*it)];  
  
                break;  
            }  
        case 3:   
            {  
  
                MatIterator_<Vec3b> it, end;  
                for( it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++ )  
                {  
                    //(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;  
                    //(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;  
                    //(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;  
                    (*it)[0] = lut[((*it)[0])];  
                    (*it)[1] = lut[((*it)[1])];  
                    (*it)[2] = lut[((*it)[2])];  
                }  
  
                break;  
  
            }  
    }  
}  

int main(int argc, char **argv)
{
    Mat img = cv::imread(argv[1]);
    Mat dst;

    float gamma_v = atoi(argv[2]);

    MyGammaCorrection(img, dst, gamma_v);

    cv::imwrite(argv[3], dst);
}

