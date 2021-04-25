#pragma once

#include "../GpCore2/GpCore.hpp"
#include "../GpApi/GpApi.hpp"
#include "../GpNetwork/GpNetwork.hpp"

#if defined(GPAPI_HTTP_LIBRARY)
    #define GPAPI_HTTP_API GP_DECL_EXPORT
#else
    #define GPAPI_HTTP_API GP_DECL_IMPORT
#endif
