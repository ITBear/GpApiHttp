#pragma once

#include "../../GpApiHttp_global.hpp"

namespace GPlatform::API::RPC {

class GPAPI_HTTP_API GpApiCliTransportHttpFactory final: public GpApiCliTransportFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpApiCliTransportHttpFactory)
    CLASS_DECLARE_DEFAULTS(GpApiCliTransportHttpFactory)

public:
                                    GpApiCliTransportHttpFactory    (GpTypeMapperFactory::SP    aTypeMapperFactory,
                                                                     GpHttpClientFactory::SP    aHttpClientFactory) noexcept;
                                    GpApiCliTransportHttpFactory    (std::string_view           aURL,
                                                                     GpTypeMapperFactory::SP    aTypeMapperFactory,
                                                                     GpHttpClientFactory::SP    aHttpClientFactory);
        virtual                     ~GpApiCliTransportHttpFactory   (void) noexcept override final;

    virtual GpApiCliTransport::SP   NewInstance                     (void) const override final;

private:
    const std::string               iURL;
    GpTypeMapperFactory::SP         iTypeMapperFactory;
    GpHttpClientFactory::SP         iHttpClientFactory;
};

}//namespace GPlatform::API::RPC
