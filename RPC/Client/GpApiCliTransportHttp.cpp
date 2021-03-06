#include "GpApiCliTransportHttp.hpp"

namespace GPlatform::API::RPC {

GpApiCliTransportHttp::GpApiCliTransportHttp
(
    GpTypeMapper::SP    aTypeMapper,
    GpHttpClient::SP    aHttpClient
) noexcept:
iTypeMapper(std::move(aTypeMapper)),
iHttpClient(std::move(aHttpClient))
{
}

GpApiCliTransportHttp::GpApiCliTransportHttp
(
    std::string_view    aURL,
    GpTypeMapper::SP    aTypeMapper,
    GpHttpClient::SP    aHttpClient
):
iURL(aURL),
iTypeMapper(std::move(aTypeMapper)),
iHttpClient(std::move(aHttpClient))
{
}

GpApiCliTransportHttp::~GpApiCliTransportHttp (void) noexcept
{
}

GpApiRsIfDesc::SP   GpApiCliTransportHttp::ProcessRQ
(
    const GpApiRqIfDesc&            aRq,
    const GpTypeStructInfo&         aRsTypeInfo,
    std::optional<SerializeRqFnT>   aBeforeSerializeRqFn,
    std::optional<SerializeRqFnT>   aAfterSerializeRqFn,
    std::optional<ProcessRqRsFnT>   aBeforeProcessFn,
    std::optional<ProcessRqRsFnT>   aAfterProcessFn
)
{
    const GpTypeMapper& typeMapper = iTypeMapper.VC();

    //RQ/RS
    GpHttpRequest::SP   httpRq;
    GpHttpResponse::SP  httpRs;
    {
        //Serialize
        GpBytesArray body;
        body.reserve(512);

        GpByteWriterStorageByteArray    bodyStorage(body);
        GpByteWriter                    bodyWriter(bodyStorage);

        if (aBeforeSerializeRqFn.has_value())
        {
            aBeforeSerializeRqFn.value()(bodyWriter);
        }

        typeMapper.FromStruct(aRq, bodyWriter);

        if (aAfterSerializeRqFn.has_value())
        {
            aAfterSerializeRqFn.value()(bodyWriter);
        }

        //Prepate HTTP RQ
        GpHttpHeaders rqHeaders;

        httpRq = MakeSP<GpHttpRequest>
        (
            GpHttpVersion::HTTP_1_1,
            GpHttpRequestType::POST,
            iURL,
            rqHeaders,
            std::move(body)
        );

        if (aBeforeProcessFn.has_value())
        {
            aBeforeProcessFn.value()(std::make_any<GpHttpRequest::SP>(httpRq));
        }

        //Do HTTP RQ
        httpRs = iHttpClient->Do(httpRq, GpHttpClient::ErorrMode::THROW_ON_NOT_200);
    }

    if (aAfterProcessFn.has_value())
    {
        aAfterProcessFn.value()(std::make_any<GpHttpResponse::SP>(httpRs));
    }

    //Deserialize
    GpTypeStructBase::SP    rsStruct    = typeMapper.ToStruct(httpRs->body, aRsTypeInfo);
    GpApiRsIfDesc::SP       res         = rsStruct.CastAs<GpApiRsIfDesc::SP>();

    return res;
}

}//namespace GPlatform::API::RPC
