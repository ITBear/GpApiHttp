#include "GpApiCliTransportHttpFactory.hpp"
#include "GpApiCliTransportHttp.hpp"

namespace GPlatform::API::RPC {

GpApiCliTransportHttpFactory::GpApiCliTransportHttpFactory
(
    GpTypeMapperFactory::SP aTypeMapperFactory,
    GpHttpClientFactory::SP aHttpClientFactory
) noexcept:
iTypeMapperFactory(std::move(aTypeMapperFactory)),
iHttpClientFactory(std::move(aHttpClientFactory))
{
}

GpApiCliTransportHttpFactory::GpApiCliTransportHttpFactory
(
    std::string_view        aURL,
    GpTypeMapperFactory::SP aTypeMapperFactory,
    GpHttpClientFactory::SP aHttpClientFactory
):
iURL(aURL),
iTypeMapperFactory(std::move(aTypeMapperFactory)),
iHttpClientFactory(std::move(aHttpClientFactory))
{
}

GpApiCliTransportHttpFactory::~GpApiCliTransportHttpFactory (void) noexcept
{
}

GpApiCliTransport::SP   GpApiCliTransportHttpFactory::NewInstance (void) const
{
    return MakeSP<GpApiCliTransportHttp>
    (
        iURL,
        iTypeMapperFactory->NewInstance(),
        iHttpClientFactory->NewInstance()
    );
}

}//namespace GPlatform::API::RPC
