#include <complex>
#include <af/dim4.hpp>
#include <af/defines.h>
#include <af/array.h>
#include <af/defines.h>
#include <copy.hpp>
#include <helper.h>
#include <backend.h>
#include <generator.hpp>
#include <random.hpp>

using af::dim4;
using namespace detail;

af_err af_get_data_ptr(void *data, const af_array arr)
{
    af_err ret = AF_SUCCESS;

    try {
        af_dtype type;
        af_get_type(&type, arr);
        switch(type) {
        case f32:   copyData(static_cast<float    *>(data), arr);  break;
        case c32:   copyData(static_cast<cfloat   *>(data), arr);  break;
        case f64:   copyData(static_cast<double   *>(data), arr);  break;
        case c64:   copyData(static_cast<cdouble  *>(data), arr);  break;
        case b8:    copyData(static_cast<char     *>(data), arr);  break;
        case s32:   copyData(static_cast<int      *>(data), arr);  break;
        case u32:   copyData(static_cast<unsigned *>(data), arr);  break;
        case u8:    copyData(static_cast<uchar    *>(data), arr);  break;
        case s8:    copyData(static_cast<char     *>(data), arr);  break;
        default:    ret  = AF_ERR_RUNTIME;                         break;
        }
    }
    CATCHALL
        return ret;
}

//Strong Exception Guarantee
af_err af_create_array(af_array *result, const void * const data,
                       const unsigned ndims, const dim_type * const dims,
                       const af_dtype type)
{
    af_err ret = AF_ERR_ARG;
    af_array out;
    try {
        dim4 d((size_t)dims[0]);
        for(unsigned i = 1; i < ndims; i++) {
            d[i] = dims[i];
        }
        switch(type) {
        case f32:   out = createHandle(d, static_cast<const float   *>(data)); break;
        case c32:   out = createHandle(d, static_cast<const cfloat  *>(data)); break;
        case f64:   out = createHandle(d, static_cast<const double  *>(data)); break;
        case c64:   out = createHandle(d, static_cast<const cdouble *>(data)); break;
        case b8:    out = createHandle(d, static_cast<const char    *>(data)); break;
        case s32:   out = createHandle(d, static_cast<const int     *>(data)); break;
        case u32:   out = createHandle(d, static_cast<const uint    *>(data)); break;
        case u8:    out = createHandle(d, static_cast<const uchar   *>(data)); break;
        case s8:    out = createHandle(d, static_cast<const char    *>(data)); break;
        default:    ret = AF_ERR_NOT_SUPPORTED;    break;
        }
        std::swap(*result, out);
        ret = AF_SUCCESS;
    }
    CATCHALL
        return ret;
}

//Strong Exception Guarantee
af_err af_constant(af_array *result, const double value,
                   const unsigned ndims, const dim_type * const dims,
                   const af_dtype type)
{
    af_err ret = AF_ERR_ARG;
    af_array out;
    try {
        dim4 d((size_t)dims[0]);
        for(unsigned i = 1; i < ndims; i++) {
            d[i] = dims[i];
        }
        switch(type) {
        case f32:   out = createHandle<float  >(d, value); break;
        case c32:   out = createHandle<cfloat >(d, value); break;
        case f64:   out = createHandle<double >(d, value); break;
        case c64:   out = createHandle<cdouble>(d, value); break;
        case b8:    out = createHandle<char   >(d, value); break;
        case s32:   out = createHandle<int    >(d, value); break;
        case u32:   out = createHandle<uint   >(d, value); break;
        case u8:    out = createHandle<uchar  >(d, value); break;
        case s8:    out = createHandle<char   >(d, value); break;
        default:    ret = AF_ERR_NOT_SUPPORTED;    break;
        }
        std::swap(*result, out);
        ret = AF_SUCCESS;
    }
    CATCHALL
        return ret;
}

template<typename T>
static inline af_array randu_(const af::dim4 &dims)
{
    return getHandle(*randu<T>(dims));
}

af_err af_randu_wrapper(af_array *out, const unsigned dim0, const unsigned dim1,
                        const unsigned dim2, const unsigned dim3, const af_dtype type)
{
     dim_type dims[] = {dim0, dim1, dim2, dim3};
    return af_randu(out, 4, dims, type);
}

af_err af_randu(af_array *out, const unsigned ndims, const dim_type * const dims, const af_dtype type)
{
    af_err ret = AF_SUCCESS;
    af_array result;
    try {
        dim4 d((size_t)dims[0]);
        for(unsigned i = 1; i < ndims; i++) {
            d[i] = dims[i];
            if(d[i] < 1) {
                return AF_ERR_ARG;
            }
        }
        switch(type) {
            case f32:   result = randu_<float  >(d);    break;
            case c32:   result = randu_<cfloat >(d);    break;
            case f64:   result = randu_<double >(d);    break;
            case c64:   result = randu_<cdouble>(d);    break;
            case s32:   result = randu_<int    >(d);    break;
            case u32:   result = randu_<uint   >(d);    break;
            case u8:    result = randu_<uchar  >(d);    break;
            // Removed because of bool type. Functions implementations exist.
            //case s8:    result = randu_<char   >(d);    break;
            //case b8:    result = randu_<char   >(d);    break;
            default:    ret    = AF_ERR_NOT_SUPPORTED; break;
        }
        if(ret == AF_SUCCESS)
            std::swap(*out, result);
    }
    CATCHALL
    return ret;
}

template<typename T>
static inline af_array randn_(const af::dim4 &dims)
{
    return getHandle(*randn<T>(dims));
}

af_err af_randn_wrapper(af_array *out, const unsigned dim0, const unsigned dim1,
                        const unsigned dim2, const unsigned dim3, const af_dtype type)
{
    dim_type dims[] = {dim0, dim1, dim2, dim3};
    return af_randn(out, 4, dims, type);
}

af_err af_randn(af_array *out, const unsigned ndims, const dim_type * const dims, const af_dtype type)
{
    af_err ret = AF_SUCCESS;
    af_array result;
    try {
        dim4 d((size_t)dims[0]);
        for(unsigned i = 1; i < ndims; i++) {
            d[i] = dims[i];
            if(d[i] < 1) {
                return AF_ERR_ARG;
            }
        }
        switch(type) {
            case f32:   result = randn_<float  >(d);    break;
            case c32:   result = randn_<cfloat >(d);    break;
            case f64:   result = randn_<double >(d);    break;
            case c64:   result = randn_<cdouble>(d);    break;
            default:    ret    = AF_ERR_NOT_SUPPORTED; break;
        }
        if(ret == AF_SUCCESS)
            std::swap(*out, result);
    }
    CATCHALL
    return ret;
}

af_err af_destroy_array(af_array arr)
{
    af_err ret = AF_ERR_ARG;
    try {
        af_dtype type;
        af_get_type(&type, arr);

        switch(type) {
        case f32:   destroyArray<float   >(arr); break;
        case c32:   destroyArray<cfloat  >(arr); break;
        case f64:   destroyArray<double  >(arr); break;
        case c64:   destroyArray<cdouble >(arr); break;
        case b8:    destroyArray<char    >(arr); break;
        case s32:   destroyArray<int     >(arr); break;
        case u32:   destroyArray<uint    >(arr); break;
        case u8:    destroyArray<uchar   >(arr); break;
        case s8:    destroyArray<char    >(arr); break;
        default:    ret = AF_ERR_NOT_SUPPORTED;    break;
        }
        ret = AF_SUCCESS;
    }
    CATCHALL
        return ret;
}