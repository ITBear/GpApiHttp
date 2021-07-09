#pragma once

#include "../../GpApiHttp_global.hpp"

namespace GPlatform::API::RPC {

class GPAPI_HTTP_API GpApiSrvRequestHandlerHttp: public GpHttpRequestHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpApiSrvRequestHandlerHttp)
    CLASS_DECLARE_DEFAULTS(GpApiSrvRequestHandlerHttp)

protected:
                                    GpApiSrvRequestHandlerHttp  (GpApiMethodsManager::SP        aApiManager,
                                                                 GpTypeMapperFactory::SP        aTypeMapperFactory,
                                                                 GpApiTypeDetectorFactory::SP   aTypeDetectorFactory) noexcept;

public:
    virtual                         ~GpApiSrvRequestHandlerHttp (void) noexcept override;

    virtual GpHttpResponse::SP      OnRequest                   (const GpHttpRequest& aRequest) override final;

protected:
    virtual GpApiRsResultDesc::SP   GenResultOK                 (void) const = 0;
    virtual GpApiRsResultDesc::SP   GenResultEx                 (const std::exception& e) const = 0;
    virtual GpApiRsResultDesc::SP   GenResultExUnknown          (void) const = 0;
    virtual GpApiRsResultDesc::SP   CallAndCatch                (std::function<void()>) const = 0;

private:
    GpApiMethodsManager::SP         iApiManager;
    GpTypeMapperFactory::SP         iTypeMapperFactory;
    GpApiTypeDetectorFactory::SP    iTypeDetectorFactory;
};

}//namespace GPlatform::API::RPC
