/*
 * Copyright (c) 2021 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "hpm_math.h"
#if HPM_MATH_MOD == HPM_MATH_MOD_Q_HW
#include <nds32_intrinsic.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_basic_math.h"
#elif HPM_MATH_MOD == HPM_MATH_MOD_DSP_FP
#include <nds32_intrinsic.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_basic_math.h"
#endif

#if HPM_MATH_MOD == HPM_MATH_MOD_Q_HW
HPM_MATH_TYPE hpm_dsp_hw_f32_q(float from_f32)
{
     HPM_MATH_TYPE m_val; 
     float from_f321 = from_f32; 
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
     riscv_dsp_convert_f32_q15(&from_f321, &m_val, 1);   
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    riscv_dsp_convert_f32_q31(&from_f321, &m_val, 1);
#endif
    return m_val;
}

float hpm_dsp_hw_q_f32(HPM_MATH_TYPE from_mdf)
{
    float m_val; 
    HPM_MATH_TYPE from_q1 = from_mdf; 
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
    riscv_dsp_convert_q15_f32(&from_q1, &m_val, 1);   
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    riscv_dsp_convert_q31_f32(&from_q1, &m_val, 1);
#endif
    return m_val;
}

HPM_MATH_TYPE hpm_dsp_hw_mul_q(HPM_MATH_TYPE x , HPM_MATH_TYPE y)
{
    HPM_MATH_TYPE m_val; 
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
    riscv_dsp_mul_q15(&x, &y, &m_val, 1); 
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    riscv_dsp_mul_q31(&x, &y, &m_val, 1);
#endif   
    return m_val;
}

HPM_MATH_TYPE hpm_dsp_hw_div_q(HPM_MATH_TYPE x , HPM_MATH_TYPE y)
{
    HPM_MATH_TYPE m_val;
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
    m_val = ((((int)x)<<15)/y); 
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    riscv_dsp_mul_q31(&x, &y, &m_val, 1);
#endif 
    return m_val;
}

HPM_MATH_TYPE hpm_dsp_hw_atan_q(HPM_MATH_TYPE x)
{
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
    return riscv_dsp_atan_q15(x); 
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    return riscv_dsp_atan_q31(x);
#endif 
}

HPM_MATH_TYPE hpm_dsp_hw_atan2_q(HPM_MATH_TYPE x , HPM_MATH_TYPE y)
{
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
    return riscv_dsp_atan2_q15(x, y); 
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    return riscv_dsp_atan2_q31(x, y);
#endif 
}

HPM_MATH_TYPE hpm_dsp_hw_abs_q(HPM_MATH_TYPE x)
{
    HPM_MATH_TYPE m_val;    
#if (HPM_QMATH_N == HPM_QMATH_MOD_Q15)
    riscv_dsp_abs_q15(&x, &m_val, 1); 
#elif (HPM_QMATH_N == HPM_QMATH_MOD_Q31)
    riscv_dsp_abs_q31(&x, &m_val, 1);
#endif 
    return m_val;
}
#elif  HPM_MATH_MOD == HPM_MATH_MOD_DSP_FP

HPM_MATH_TYPE hpm_dsp_hw_mul_fp(HPM_MATH_TYPE x , HPM_MATH_TYPE y)
{
    HPM_MATH_TYPE m_val; 
    riscv_dsp_mul_f32(&x, &y, &m_val, 1);
    return m_val;
}

HPM_MATH_TYPE hpm_dsp_hw_div_fp(HPM_MATH_TYPE x , HPM_MATH_TYPE y)
{
    HPM_MATH_TYPE m_val;
    riscv_dsp_div_f32(&x, &y, &m_val, 1);
    return m_val;
}

HPM_MATH_TYPE hpm_dsp_hw_atan_fp(HPM_MATH_TYPE x)
{
    return riscv_dsp_atan_f32(x);
}

HPM_MATH_TYPE hpm_dsp_hw_atan2_fp(HPM_MATH_TYPE x , HPM_MATH_TYPE y)
{
    return riscv_dsp_atan2_f32(x,y);
}

HPM_MATH_TYPE hpm_dsp_hw_abs_fp(HPM_MATH_TYPE x)
{
    HPM_MATH_TYPE m_val;    
    riscv_dsp_abs_f32(&x, &m_val, 1); 
    return m_val;
}

#endif

