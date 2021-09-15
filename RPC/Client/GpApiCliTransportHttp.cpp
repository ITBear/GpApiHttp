#include "GpApiCliTransportHttp.hpp"
#include <iostream>

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

        //TODO: move to log
        {
            std::string_view bodySW = GpRawPtrCharR(body).AsStringView();

            if (bodySW.length() > 1024)
            {
                bodySW = bodySW.substr(0, 1024);
            }
            std::cout << "[GpApiCliTransportHttp::ProcessRQ]: =========================== RQ =======================\n"
                      << bodySW << std::endl;
        }

        //Prepate HTTP RQ
        GpHttpHeaders rqHeaders;//TODO: move UP
        rqHeaders.Add(GpHttpHeaderType::CONTENT_TYPE, "application/json"_sv);

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

    //TODO: move to log
    {
        std::string_view bodySW = GpRawPtrCharR(httpRs->body).AsStringView();

        if (bodySW.length() > 1024)
        {
            bodySW = bodySW.substr(0, 1024);
        }
        std::cout << "[GpApiCliTransportHttp::ProcessRQ]: =========================== RS =======================\n"
                  << bodySW << std::endl;
    }

    //Deserialize
    GpTypeStructBase::SP rsStruct;

    try
    {
        rsStruct = typeMapper.ToStruct(httpRs->body, aRsTypeInfo);
    } catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        throw;
    }

    return rsStruct.CastAs<GpApiRsIfDesc::SP>();
}

}//namespace GPlatform::API::RPC
