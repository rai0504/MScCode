#ifndef ANISOTROPIC_NONLINEAR_CED
#define ANISOTROPIC_NONLINEAR_CED

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

enum eDerivativeFilter
{
    SOBEL=0,
    SCHARR
};

enum eUpdateScheme
{
    STANDARD=0,
    NONNEGATIVITY,
    EULER,
    EULER_CONVOLUTION
};

cv::Mat* CoherenceEnhancedDiffusion(
        /* input_image */           cv::Mat* u,
        /* derivative_filter */     eDerivativeFilter derivativeFilter,
        /* noise scale */           double sigma,
        /* integration scale */     double rho,
        /* c1 */                    double c1,
        /* c2 */                    double c2,
        /* time step */             double tau,
        /* number of iterations */  unsigned int n,
        /* diffusion period */      unsigned int Tdiffusion,
        /* update_scheme */         eUpdateScheme updateScheme
        );

void Gradient(cv::Mat* u, cv::Mat* ux, cv::Mat* uy, eDerivativeFilter derivativeFilter);
#endif /* ANISOTROPIC_NONLINEAR_CED */
