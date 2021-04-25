#include "GpApiSrvRequestHandlerHttp.hpp"
#include <iostream>

namespace GPlatform::API::RPC {

GpApiSrvRequestHandlerHttp::GpApiSrvRequestHandlerHttp
(
    GpApiMethodsManager::SP         aApiManager,
    GpTypeMapperFactory::SP         aTypeMapperFactory,
    GpApiTypeDetectorFactory::SP    aTypeDetectorFactory
) noexcept:
iApiManager(std::move(aApiManager)),
iTypeMapperFactory(std::move(aTypeMapperFactory)),
iTypeDetectorFactory(std::move(aTypeDetectorFactory))
{
}

GpApiSrvRequestHandlerHttp::~GpApiSrvRequestHandlerHttp (void) noexcept
{
}

GpHttpResponse::SP  GpApiSrvRequestHandlerHttp::OnRequest (const GpHttpRequest& aRequest)
{
    //TODO: reimplement with log
    {
        std::string_view bodySW = GpRawPtrCharR(aRequest.body).AsStringView();

        if (bodySW.length() > 1024)
        {
            bodySW = bodySW.substr(0, 1024);
        }

        std::cout << "[GpApiSrvRequestHandlerHttp::OnRequest]: --------------------------- RQ ---------------------------\n"_sv;
        std::cout << bodySW << "\n";
        std::cout.flush();
    }

    GpApiRqIfDesc::SP rq;
    GpApiRsIfDesc::SP rs;

    auto typeMapper = iTypeMapperFactory->NewInstance();

    try
    {
        //Detect type
        auto                    typeDetector    = iTypeDetectorFactory->NewInstance(iApiManager.VC(), aRequest.body);
        const GpTypeStructInfo& rqTypeInfo      = typeDetector->DetectTypeInfo();

        //Deserialize RQ data
        rq = typeMapper->ToStruct(aRequest.body, rqTypeInfo).CastAs<GpApiRqIfDesc::SP>();

        //Find method and call
        GpApiMethod::SP method = iApiManager->Find(rq->Method());

        auto result = CallAndCatch([&](){rs = method->Process(rq.Vn());});

        if (rs.IsNULL())
        {
            rs = method->RsTypeInfo().NewInstance().CastAs<GpApiRsIfDesc::SP>();
        }

        rs->SetResult(result);
    } catch (const std::exception& e)
    {
        if (rs.IsNULL())
        {
            rs = iApiManager->NewDefaultRs();
        }

        rs->SetResult(GenResultEx(e));
    } catch (...)
    {
        if (rs.IsNULL())
        {
            rs = iApiManager->NewDefaultRs();
        }

        rs->SetResult(GenResultExUnknown());
    }

    //Serialize RS data
    GpBytesArray rsBody = typeMapper->FromStruct(rs.VC());

    //TODO: reimplement with log
    {
        std::string_view bodySW = GpRawPtrCharR(rsBody).AsStringView();

        if (bodySW.length() > 1024)
        {
            bodySW = bodySW.substr(0, 1024);
        }

        std::cout << "[GpApiSrvRequestHandlerHttp::OnRequest]: --------------------------- RS ---------------------------\n"_sv;
        std::cout << bodySW << "\n";
        std::cout.flush();
    }

    GpHttpHeaders headers;
    headers
        .SetContentType(GpContentType::APPLICATION_JSON, GpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::KEEP_ALIVE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    GpHttpResponse::SP httpRs = MakeSP<GpHttpResponse>
    (
        GpHttpResponseCode::OK_200,
        std::move(headers),
        std::move(rsBody)
    );

    return httpRs;
}

}//namespace GPlatform::API::RPC
