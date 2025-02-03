//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: QSFController.cpp
//
// MATLAB Coder version            : 24.1
// C/C++ source code generated on  : 2025-01-30 08:35:22:766
//

// Include Files
#include "QSFController.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <emmintrin.h>

// Function Declarations
static double rt_powd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

//
// Arguments    : const double z[12]
//                const double k[10]
//                const double param[4]
//                const double ref_1[5]
//                const double ref_2[5]
//                const double ref_3[5]
//                double F[3]
// Return Type  : void
//
void QSFController(const double z[12], const double k[10],
                   const double param[4], const double ref_1[5],
                   const double ref_2[5], const double ref_3[5], double F[3])
{
  __m128d r;
  __m128d r1;
  double b_z[9];
  double b_c_tmp;
  double b_omega_idx_1_tmp;
  double b_z_idx_3_tmp;
  double b_z_idx_3_tmp_tmp;
  double c_c_tmp;
  double c_omega_idx_1_tmp;
  double c_tmp;
  double c_z_idx_3_tmp;
  double d_omega_idx_1_tmp;
  double d_z_idx_3_tmp;
  double e_omega_idx_1_tmp;
  double f_omega_idx_1_tmp;
  double g_omega_idx_1_tmp;
  double h_omega_idx_1_tmp;
  double i_omega_idx_1_tmp;
  double j_omega_idx_1_tmp;
  double nu__3;
  double omega_idx_0;
  double omega_idx_1;
  double omega_idx_1_tmp;
  double omega_idx_2;
  double z_idx_1_tmp;
  double z_idx_2_tmp;
  double z_idx_3_tmp;
  double z_idx_3_tmp_tmp;
  double z_tmp;
  //     %% Input Interface
  //  p = [p__1;p__2;p__3];
  //  q = [q__1;q__2;q__3];
  //  v = [v__1;v__2;v__3];
  //     %% The output
  //     %% Aux input
  //  nu__3 = -k3*[y3;y3_dt];
  //  nu__3_dt = -k3*[y3_dt;nu__3];
  //  along z-axis
  //  set nu_3 = y3_ddt
  z_idx_1_tmp = z[8] - ref_3[1];
  nu__3 = (-k[8] * (z[2] - ref_3[0]) + -k[9] * z_idx_1_tmp) + ref_3[2];
  //  Further derivatives for output
  //  along x-axis
  //  nu__1 = - k1*[y1; y1_dt; y1_ddt; y1_dddt];
  //  along y-axis
  //  nu__2 = - k2*[y2; y2_dt; y2_ddt; y2_dddt];
  //  ch = nu__3;
  //  ch_dt = nu__3_dt;
  //     %% Transformed input, u
  // u__2 = -L * m__q * (((2 * omega__2 * omega__3 * g - 2 * nu__3 * omega__2 *
  // omega__3 + nu__2) * q__3 ^ 3) + ((((2 * omega__3 * k32 - 3 * omega__1 *
  // omega__2) * nu__3 + 3 * omega__1 * omega__2 * g + 2 * v__3 * omega__3 *
  // k31) * q__1 - ((k32 ^ 2 + omega__2 ^ 2 - omega__3 ^ 2 - k31) * nu__3 + v__3
  // * k31 * k32 - omega__2 ^ 2 * g + omega__3 ^ 2 * g) * q__2) * q__3 ^ 2) +
  // ((((2 * omega__1 * k32 - 3 * omega__2 * omega__3) * nu__3 + 2 * v__3 *
  // omega__1 * k31 + 3 * omega__2 * omega__3 * g) * q__1 ^ 2) + 0.3e1 * q__2 *
  // (((omega__1 * omega__3) + 0.2e1 / 0.3e1 * omega__2 * k32) * nu__3 -
  // (omega__1 * omega__3 * g) + 0.2e1 / 0.3e1 * v__3 * omega__2 * k31) * q__1 +
  // ((-2 * omega__1 * k32 + omega__2 * omega__3) * nu__3) - (2 * v__3 *
  // omega__1 * k31) - (omega__2 * omega__3 * g)) * q__3 + (2 * (-2 * omega__1 *
  // omega__2 * q__1 ^ 3 + q__2 * (omega__1 ^ 2 - omega__2 ^ 2) * q__1 ^ 2 + 2 *
  // omega__1 * omega__2 * q__1 - omega__1 ^ 2 * q__2) * (-g + nu__3))) / q__3 /
  // (-g + nu__3); u__3 = L * m__q * ((((2 * omega__2 * k32 + omega__1 *
  // omega__3) * nu__3 - omega__1 * omega__3 * g + 2 * v__3 * omega__2 * k31 +
  // nu__1) * q__3 ^ 3) + ((((-k32 ^ 2 + omega__1 ^ 2 - 2 * omega__2 ^ 2 +
  // omega__3 ^ 2 + k31) * nu__3 - omega__1 ^ 2 * g + 2 * omega__2 ^ 2 * g -
  // omega__3 ^ 2 * g - v__3 * k31 * k32) * q__1 + q__2 * ((-2 * omega__3 * k32
  // + omega__1 * omega__2) * nu__3 - omega__1 * omega__2 * g - 2 * v__3 *
  // omega__3 * k31)) * q__3 ^ 2) + ((((2 * omega__2 * k32 + 3 * omega__1 *
  // omega__3) * nu__3 - 3 * omega__1 * omega__3 * g + 2 * v__3 * omega__2 *
  // k31) * q__1 ^ 2) - 0.2e1 * (((omega__1 * k32) - 0.3e1 / 0.2e1 * omega__2 *
  // omega__3) * nu__3 + (v__3 * omega__1 * k31) + 0.3e1 / 0.2e1 * omega__2 *
  // omega__3 * g) * q__2 * q__1 - (2 * omega__1 * omega__3 * (-g + nu__3))) *
  // q__3 + (2 * ((omega__1 ^ 2 - omega__2 ^ 2) * q__1 ^ 2 + 2 * omega__1 *
  // omega__2 * q__1 * q__2 - omega__1 ^ 2) * (-g + nu__3) * q__1)) / q__3 / (-g
  // + nu__3);
  z_idx_2_tmp = -param[3] + nu__3;
  z_idx_3_tmp = param[3] - nu__3;
  b_z_idx_3_tmp = z[5] * z[5];
  z_idx_3_tmp_tmp = ref_3[2] * k[9];
  b_z_idx_3_tmp_tmp = -z[8] + ref_3[1];
  c_z_idx_3_tmp =
      ((-k[9] * nu__3 + b_z_idx_3_tmp_tmp * k[8]) + z_idx_3_tmp_tmp) + ref_3[3];
  d_z_idx_3_tmp = -z[3] * z[10] + z[4] * z[9];
  //     %% Input transfomration
  //  Qinv = [q__1 * (m__q + m__p) - (q__2 * q__1 / q__3), (q__1 ^ 2 - 1) /
  //  q__3; q__2 * (m__q + m__p) (-q__2 ^ 2 + 1) / q__3 q__2 * q__1 / q__3; q__3
  //  * (m__q + m__p) -q__2 q__1;]; actual force, F
  c_tmp = z[9] * z[9];
  b_c_tmp = z[10] * z[10];
  c_c_tmp = z[11] * z[11];
  omega_idx_2 = param[0] + param[1];
  b_z[0] = z[3] * omega_idx_2;
  omega_idx_1 = z[3] * z[4] / z[5];
  b_z[3] = -omega_idx_1;
  z_tmp = z[3] * z[3];
  b_z[6] = (z_tmp - 1.0) / z[5];
  b_z[1] = z[4] * omega_idx_2;
  b_z[4] = (-(z[4] * z[4]) + 1.0) / z[5];
  b_z[7] = omega_idx_1;
  b_z[2] = z[5] * omega_idx_2;
  b_z[5] = -z[4];
  b_z[8] = z[3];
  omega_idx_0 = z_idx_2_tmp / z[5] + param[1] * param[2] / omega_idx_2 *
                                         ((c_tmp + b_c_tmp) + c_c_tmp);
  omega_idx_1_tmp = k[9] * k[9];
  z_idx_3_tmp_tmp =
      ((k[9] * nu__3 + k[8] * z_idx_1_tmp) - z_idx_3_tmp_tmp) - ref_3[3];
  omega_idx_2 = 2.0 * k[9] * ref_3[2];
  b_omega_idx_1_tmp = z[11] * z_idx_2_tmp * z[9];
  c_omega_idx_1_tmp =
      ((2.0 * k[9] * nu__3 + (-2.0 * ref_3[1] + 2.0 * z[8]) * k[8]) -
       omega_idx_2) -
      2.0 * ref_3[3];
  d_omega_idx_1_tmp = rt_powd_snf(z[5], 3.0);
  e_omega_idx_1_tmp = c_c_tmp * param[3];
  f_omega_idx_1_tmp = ref_3[2] * omega_idx_1_tmp;
  g_omega_idx_1_tmp = 2.0 * z[11] * z_idx_3_tmp_tmp;
  h_omega_idx_1_tmp = 2.0 * z_idx_2_tmp;
  i_omega_idx_1_tmp = c_tmp - b_c_tmp;
  j_omega_idx_1_tmp = 2.0 * z[9] * z[10] * z[3];
  omega_idx_1 =
      -param[2] *
      ((((-2.0 * z[10] * z[11] * z_idx_2_tmp +
          ((((-k[4] * (z[1] - ref_2[0]) + -k[5] * (z[7] - ref_2[1])) +
             -k[6] * (z[4] * z_idx_2_tmp / z[5] - ref_2[2])) +
            -k[7] *
                (((z[9] * z_idx_3_tmp * b_z_idx_3_tmp +
                   (c_z_idx_3_tmp * z[4] - z[3] * z[11] * z_idx_3_tmp) * z[5]) +
                  z[4] * d_z_idx_3_tmp * z_idx_3_tmp) /
                     b_z_idx_3_tmp -
                 ref_2[3])) +
           ref_2[4])) *
             d_omega_idx_1_tmp +
         ((-3.0 * z[10] * z_idx_2_tmp * z[9] + g_omega_idx_1_tmp) * z[3] -
          (((((z_idx_2_tmp * b_c_tmp +
               ((omega_idx_1_tmp - c_c_tmp) - k[8]) * nu__3) +
              e_omega_idx_1_tmp) +
             (z_idx_1_tmp * k[9] + ref_3[2]) * k[8]) -
            f_omega_idx_1_tmp) +
           ref_3[4]) *
              z[4]) *
             b_z_idx_3_tmp) +
        ((((c_omega_idx_1_tmp * z[9] - 3.0 * z[10] * z[11] * z_idx_2_tmp) *
               z_tmp +
           3.0 *
               (b_omega_idx_1_tmp +
                0.66666666666666663 * z[10] * z_idx_3_tmp_tmp) *
               z[4] * z[3]) +
          (((-2.0 * k[9] * nu__3 + (2.0 * ref_3[1] - 2.0 * z[8]) * k[8]) +
            omega_idx_2) +
           2.0 * ref_3[3]) *
              z[9]) +
         z[10] * z[11] * z_idx_2_tmp) *
            z[5]) +
       h_omega_idx_1_tmp * (((-2.0 * z[9] * z[10] * rt_powd_snf(z[3], 3.0) +
                              z[4] * i_omega_idx_1_tmp * z_tmp) +
                             j_omega_idx_1_tmp) -
                            c_tmp * z[4])) *
      param[1] / z[5] / z_idx_2_tmp;
  omega_idx_2 =
      param[2] *
      (((((b_omega_idx_1_tmp + c_omega_idx_1_tmp * z[10]) +
          ((((-k[0] * (z[0] - ref_1[0]) + -k[1] * (z[6] - ref_1[1])) +
             -k[2] * (z[3] * z_idx_2_tmp / z[5] - ref_1[2])) +
            -k[3] *
                (((-z[10] * z_idx_3_tmp * b_z_idx_3_tmp +
                   (c_z_idx_3_tmp * z[3] + z[4] * z[11] * z_idx_3_tmp) * z[5]) +
                  z[3] * d_z_idx_3_tmp * z_idx_3_tmp) /
                     b_z_idx_3_tmp -
                 ref_1[3])) +
           ref_1[4])) *
             d_omega_idx_1_tmp +
         (((((((z_idx_2_tmp * c_tmp +
                (2.0 * param[3] - 2.0 * nu__3) * b_c_tmp) +
               ((-omega_idx_1_tmp + c_c_tmp) + k[8]) * nu__3) -
              e_omega_idx_1_tmp) +
             (b_z_idx_3_tmp_tmp * k[9] - ref_3[2]) * k[8]) +
            f_omega_idx_1_tmp) -
           ref_3[4]) *
              z[3] +
          (z[10] * z_idx_2_tmp * z[9] - g_omega_idx_1_tmp) * z[4]) *
             b_z_idx_3_tmp) +
        (((3.0 * z[11] * z_idx_2_tmp * z[9] + 2.0 * z[10] * z_idx_3_tmp_tmp) *
              z_tmp -
          2.0 * (z_idx_3_tmp_tmp * z[9] - 1.5 * z[10] * z[11] * z_idx_2_tmp) *
              z[4] * z[3]) -
         2.0 * z[11] * z_idx_2_tmp * z[9]) *
            z[5]) +
       h_omega_idx_1_tmp * z[3] *
           ((i_omega_idx_1_tmp * z_tmp + j_omega_idx_1_tmp * z[4]) - c_tmp)) *
      param[1] / z[5] / z_idx_2_tmp;
  r = _mm_loadu_pd(&b_z[0]);
  r = _mm_mul_pd(r, _mm_set1_pd(omega_idx_0));
  r1 = _mm_loadu_pd(&b_z[3]);
  r1 = _mm_mul_pd(r1, _mm_set1_pd(omega_idx_1));
  r = _mm_add_pd(r, r1);
  r1 = _mm_loadu_pd(&b_z[6]);
  r1 = _mm_mul_pd(r1, _mm_set1_pd(omega_idx_2));
  r = _mm_add_pd(r, r1);
  _mm_storeu_pd(&F[0], r);
  F[2] = (b_z[2] * omega_idx_0 + b_z[5] * omega_idx_1) + b_z[8] * omega_idx_2;
}

//
// File trailer for QSFController.cpp
//
// [EOF]
//
