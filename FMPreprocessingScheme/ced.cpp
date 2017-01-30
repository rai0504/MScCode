#include "ced.h"
#include <iostream>
using namespace std;

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
        )
{
    static cv::Mat* f;

    if ((u->dims>2)             ||
        (sigma<=0)              ||
        (rho<=0)                ||
        (c1<=0) || (c1>=1)      ||
        (c2<=0)                 ||
        (tau<=0)
        )
        return f;  // return an empty matrix. Only except 2-dimensional images.

    f = u;
    //if (f->type()!=CV_64FC1 || f->type()!=CV_64F) f->convertTo(*f, CV_64FC1);

    for (unsigned int k=0; k<n; k++)
    {
        f->convertTo(*f,CV_64FC1);
        if (k%Tdiffusion==0)
        {
            // 1. Calculate Gaussian smoothed the image using K_sigma
            cv::Mat f_sigma;
            //int kernel_size = ceil(3*sigma);
            int kernel_size = ceil(2.0*(1.0 + (sigma-0.8)/(0.3)));
            if (kernel_size%2==0) kernel_size++;
            cv::GaussianBlur(*f, f_sigma, cv::Size(kernel_size, kernel_size), sigma, sigma, cv::BORDER_REPLICATE);

            // 2. Calculate tensor product grad(u_sigma)*grad(u_sigma)^T
            cv::Mat f_sigma_dx2, f_sigma_dx_dy, f_sigma_dy2;
            {
                cv::Mat f_sigma_dx, f_sigma_dy;
                Gradient(&f_sigma, &f_sigma_dx, &f_sigma_dy, derivativeFilter);
                f_sigma_dx2 = f_sigma_dx.mul(f_sigma_dx);
                f_sigma_dx_dy = f_sigma_dx.mul(f_sigma_dy);
                f_sigma_dy2 = f_sigma_dy.mul(f_sigma_dy);
            }

            // 3. Calculate the structure tensor by averaging the orienations using K_rho
            cv::Mat J11, J12, J22;
            {
                //int kernel_size = ceil(3*rho);
                int kernel_size = ceil(2.0*(1.0 + (rho-0.8)/(0.3)));
                if (kernel_size%2==0) kernel_size++;
                cv::GaussianBlur(f_sigma_dx2, J11, cv::Size(kernel_size, kernel_size), rho, rho, cv::BORDER_REPLICATE);
                cv::GaussianBlur(f_sigma_dx_dy, J12, cv::Size(kernel_size, kernel_size), rho, rho, cv::BORDER_REPLICATE);
                cv::GaussianBlur(f_sigma_dy2, J22, cv::Size(kernel_size, kernel_size), rho, rho, cv::BORDER_REPLICATE);
            }

            // 4. Calculate the diffusion tensor
            cv::Mat a, b, c;
            {
                cv::Mat pi = J22-J11;
                cv::Mat kappa = pi.mul(pi) + 4*J12.mul(J12);
                double lambda_1 = c1;
                cv::Mat lambda_2;

                cv::Mat temp1, temp2;
                cv::exp(-c2/kappa, temp1);
                lambda_2 = c1 + (1-c1)*temp1;

                temp1 = lambda_1-lambda_2;
                cv::pow(kappa, -0.5, temp2);
                temp1 = temp1.mul(temp2);
                temp2 = lambda_1+lambda_2;

                a = 0.5*(temp2 - temp1.mul(pi));
                b = temp1.mul(J12);
                c = 0.5*(temp2 + temp1.mul(pi));
            }

            // 6. Calculate df/dt
            cv::Mat df_dt;
            if (updateScheme==EULER)
            {
                // 5. Calculate the flux components
                cv::Mat j1, j2;
                {
                    cv::Mat f_dx, f_dy;
                    Gradient(f, &f_dx, &f_dy, derivativeFilter);
                    j1 = a.mul(f_dx) + b.mul(f_dy);
                    j2 = b.mul(f_dx) + c.mul(f_dy);
                }

                {
                    cv::Mat j1_dx, j2_dy;
                    if (derivativeFilter==SOBEL)
                    {
                        cv::Sobel(j1, j1_dx, CV_64FC1, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE);
                        cv::Sobel(j2, j2_dy, CV_64FC1, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE);
                    }
                    else if (derivativeFilter==SCHARR)
                    {
                        cv::Scharr(j1, j1_dx, CV_64FC1, 1, 0, 1, 0, cv::BORDER_REPLICATE);
                        cv::Scharr(j2, j2_dy, CV_64FC1, 0, 1, 1, 0, cv::BORDER_REPLICATE);
                    }

                    df_dt = j1_dx + j2_dy;
                }
            }
            else if (updateScheme==STANDARD)
            {
                df_dt = cv::Mat::zeros(a.rows, a.cols, CV_64FC1);

                for(int i=1; i<a.rows-1; i++)
                    for(int j=1; j<a.cols-1; j++)
                    {
                        double a11 = (-b.at<double>(i-1,j)-b.at<double>(i,j+1))/4;
                        double a12 = (c.at<double>(i,j+1)+c.at<double>(i,j))/2;
                        double a13 = (b.at<double>(i+1,j)+b.at<double>(i,j+1))/4;

                        double a21 = (a.at<double>(i-1,j) + a.at<double>(i,j))/2;
                        double a22 = (-((a.at<double>(i-1,j)+2*a.at<double>(i,j)+a.at<double>(i+1,j))/2) - ((c.at<double>(i,j-1)+2*c.at<double>(i,j)+c.at<double>(i,j+1))/2));
                        double a23 = (a.at<double>(i+1,j) + a.at<double>(i,j))/2;

                        double a31 = (b.at<double>(i-1,j)+b.at<double>(i,j-1))/4;
                        double a32 = (c.at<double>(i,j-1)+c.at<double>(i,j))/2;
                        double a33 = (-b.at<double>(i+1,j)-b.at<double>(i,j-1))/4;

                        // convolution with current image
#if 1
                        df_dt.at<double>(i,j) = a11*f->at<double>(i+1,j+1) + a12*f->at<double>(i+1,j) + a13*f->at<double>(i+1,j-1) +
                                                a21*f->at<double>(i,j+1)   + a22*f->at<double>(i,j)   + a23*f->at<double>(i,j-1)   +
                                                a31*f->at<double>(i-1,j+1) + a32*f->at<double>(i-1,j) + a33*f->at<double>(i-1,j-1);
#else
                        df_dt.at<double>(i,j) = a11*f->at<double>(i-1,j-1) + a12*f->at<double>(i-1,j) + a13*f->at<double>(i-1,j+1) +
                                                a21*f->at<double>(i,j-1)   + a22*f->at<double>(i,j)   + a23*f->at<double>(i,j+1)   +
                                                a31*f->at<double>(i+1,j-1) + a32*f->at<double>(i+1,j) + a33*f->at<double>(i+1,j+1);
#endif
                    }
            }
            else if (updateScheme==NONNEGATIVITY)
            {
                df_dt = cv::Mat::zeros(a.rows, a.cols, CV_64FC1);

                for(int i=1; i<a.rows-1; i++)
                    for(int j=1; j<a.cols-1; j++)
                {
                    double a11 = (abs(b.at<double>(i-1,j+1))-b.at<double>(i-1,j+1)+abs(b.at<double>(i,j))-b.at<double>(i,j))/4;
                    double a12 = (c.at<double>(i,j+1)+c.at<double>(i,j)-abs(b.at<double>(i,j+1)-abs(b.at<double>(i,j))))/2;
                    double a13 = (abs(b.at<double>(i+1,j+1))+b.at<double>(i+1,j+1)+abs(b.at<double>(i,j))+b.at<double>(i,j))/4;
                    double a21 = (a.at<double>(i-1,j)+a.at<double>(i,j)-abs(b.at<double>(i-1,j))-abs(b.at<double>(i,j)))/2;

                    double a22 = -(a.at<double>(i-1,j)+2*a.at<double>(i,j)+a.at<double>(i+1,j))/2 -
                            (abs(b.at<double>(i-1,j+1))-b.at<double>(i-1,j+1)+abs(b.at<double>(i+1,j+1))+b.at<double>(i+1,j+1))/4 +
                            (abs(b.at<double>(i-1,j-1))-b.at<double>(i-1,j-1)+abs(b.at<double>(i+1,j-1))+b.at<double>(i+1,j-1))/4 -
                            (c.at<double>(1,j-1)+2*c.at<double>(i,j)+c.at<double>(i,j+1))/2;

                    double a23 = (a.at<double>(i+1,j)+a.at<double>(i,j)-abs(b.at<double>(i+1,j))-abs(b.at<double>(i,j)))/2;
                    double a33 = (abs(b.at<double>(i+1,j-1))-b.at<double>(i+1,j-1)+abs(b.at<double>(i,j))-b.at<double>(i,j))/4;
                    double a32 = (c.at<double>(i,j-1)+c.at<double>(i,j)-abs(b.at<double>(i,j-1)-abs(b.at<double>(i,j))))/2;
                    double a31 = (abs(b.at<double>(i-1,j-1))+b.at<double>(i-1,j-1)+abs(b.at<double>(i,j))+b.at<double>(i,j))/4;

#if 1
                        df_dt.at<double>(i,j) = a11*f->at<double>(i+1,j+1) + a12*f->at<double>(i+1,j) + a13*f->at<double>(i+1,j-1) +
                                                a21*f->at<double>(i,j+1)   + a22*f->at<double>(i,j)   + a23*f->at<double>(i,j-1)   +
                                                a31*f->at<double>(i-1,j+1) + a32*f->at<double>(i-1,j) + a33*f->at<double>(i-1,j-1);
#else
                        df_dt.at<double>(i,j) = a11*f->at<double>(i-1,j-1) + a12*f->at<double>(i-1,j) + a13*f->at<double>(i-1,j+1) +
                                                a21*f->at<double>(i,j-1)   + a22*f->at<double>(i,j)   + a23*f->at<double>(i,j+1)   +
                                                a31*f->at<double>(i+1,j-1) + a32*f->at<double>(i+1,j) + a33*f->at<double>(i+1,j+1);
#endif
                }
            }

            // 7. Update the image
            *f = *f + tau*df_dt;
            f->convertTo(*f,CV_8UC1);
        }
    }

    return f;
}

void Gradient(cv::Mat* u, cv::Mat* ux, cv::Mat* uy, eDerivativeFilter derivativeFilter)
{
    if (derivativeFilter==SOBEL)
    {
        cv::Sobel(*u, *ux, CV_64FC1, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE);
        cv::Sobel(*u, *uy, CV_64FC1, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE);
    }
    else if (derivativeFilter==SCHARR)
    {
        cv::Scharr(*u, *ux, CV_64FC1, 1, 0, 1, 0, cv::BORDER_REPLICATE);
        cv::Scharr(*u, *uy, CV_64FC1, 0, 1, 1, 0, cv::BORDER_REPLICATE);
    }
}
