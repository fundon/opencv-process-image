#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 修复色差
Mat MyGammaCorrection(Mat& img, float fGamma) {
  // build look up table
  unsigned char lut[256];
  for (int i = 0; i < 256; i++) {
    lut[i] = saturate_cast<uchar>(pow((float)(i/255.0), 1.f/fGamma) * 255.0f);
  }

  Mat dst = img.clone();
  const int channels = dst.channels();
  switch(channels) {
    case 1:
      {
        MatIterator_<uchar> it, end;
        for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++) {
          //*it = pow((float)(((*it))/255.0), fGamma) * 255.0;
          *it = lut[(*it)];
        }
        break;
      }
    case 3:
      {
        MatIterator_<Vec3b> it, end;
        for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++){
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

  return dst;
}

// 高斯滤波
Mat MyGaussianFilter (Mat& img) {
  // 设置高斯滤波器
  float halfSize = 500;
  float fSize = 2 * halfSize + 1;

  Mat gaussianKernel = getGaussianKernel(fSize, fSize / 6, CV_32F);
  mulTransposed(gaussianKernel, gaussianKernel, false);

  // 进行高斯滤波
  Mat imageFilter;
  filter2D(img, imageFilter, img.depth(), gaussianKernel, Point(-1, -1), 0, BORDER_CONSTANT);

  // 做差值
  return (img - imageFilter) + 128;
}

int main(int argc, char **argv) {
  char *input = argv[1];
  char *output = argv[2];
  float gamma_v = atoi(argv[3]);

  // 读取图片，并转换
  Mat src = imread(input, CV_LOAD_IMAGE_COLOR);

  CV_Assert(src.data);
  // accept only char type matrices
  CV_Assert(src.depth() != sizeof(uchar));

  Mat img = src.clone();
  // Mat img;
  // src.convertTo(img, CV_32F, 1, 0);

  Mat dst = MyGaussianFilter(img);

  dst = MyGammaCorrection(dst, gamma_v);

  // JPEG 品质
  vector<int> params;
  params.push_back(CV_IMWRITE_JPEG_QUALITY);
  params.push_back(94);

  // 保存图片
  imwrite(output, dst, params);

  return 0;
}
