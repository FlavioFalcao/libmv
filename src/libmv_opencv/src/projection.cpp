/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2009, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <opencv2/sfm/sfm.hpp>

using namespace cv;

template<typename T>
void HomogeneousToEuclidean_(const cv::Mat & _X, cv::Mat & _x)
{
    int d = _X.rows - 1;

    const cv::Mat_<T> & X_rows = _X.rowRange(0,d);
    const Mat_<T> h = _X.row(d);

    const T * h_ptr = h[0], *h_ptr_end = h_ptr + h.cols;
    const T * X_ptr = X_rows[0];
    T * x_ptr = _x.ptr<T>(0);
    for (; h_ptr != h_ptr_end; ++h_ptr, ++X_ptr, ++x_ptr)
    {
        const T * X_col_ptr = X_ptr;
        T * x_col_ptr = x_ptr, *x_col_ptr_end = x_col_ptr + d * _x.step;
        for (; x_col_ptr != x_col_ptr_end; X_col_ptr+=X_rows.step, x_col_ptr+=_x.step )
        {
            *x_col_ptr = (*X_col_ptr) / (*h_ptr);
        }
    }
}

void HomogeneousToEuclidean(const InputArray _X, OutputArray _x)
{
    const Mat X = _X.getMat();
    cv::Mat x = _x.getMat();

    if( X.depth() == CV_32F )
    {
        HomogeneousToEuclidean_<float>(X,x);
    }
    else
    {
        HomogeneousToEuclidean_<double>(X,x);
    }
}