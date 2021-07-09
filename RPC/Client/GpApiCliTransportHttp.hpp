#pragma once

#include "../../GpApiHttp_global.hpp"
#include <optional>

namespace GPlatform::API::RPC {

class GPAPI_HTTP_API GpApiCliTransportHttp final: public GpApiCliTransport
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpApiCliTransportHttp)
    CLASS_DECLARE_DEFAULTS(GpApiCliTransportHttp)

public:
                                GpApiCliTransportHttp   (GpTypeMapper::SP   aTypeMapper,
                                                         GpHttpClient::SP   aHttpClient) noexcept;
                                GpApiCliTransportHttp   (std::string_view   aURL,
                                                         GpTypeMapper::SP   aTypeMapper,
                                                         GpHttpClient::SP   aHttpClient);
    virtual                     ~GpApiCliTransportHttp  (void) noexcept override final;

    std::string_view            URL                     (void) const noexcept {return iURL;}
    void                        SetURL                  (std::string_view aURL) {iURL = aURL;}

    virtual GpApiRsIfDesc::SP   ProcessRQ               (const GpApiRqIfDesc&           aRq,
                                                         const GpTypeStructInfo&        aRsTypeInfo,
                                                         std::optional<ProcessRqRsFnT>  aBeforeProcessFn,
                                                         std::optional<ProcessRqRsFnT>  aAfterProcessFn) override final;

private:
    std::string                 iURL;
    GpTypeMapper::SP            iTypeMapper;
    GpHttpClient::SP            iHttpClient;
};

}//namespace GPlatform::API::RPC
