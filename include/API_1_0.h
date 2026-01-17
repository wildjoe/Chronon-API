#ifndef IAPI_H
#define IAPI_H
#include <string>
#include <string_view>
#include "APICore.h"


namespace ChronoAPI_1_0
{
    //using namespace ChronoAPI;
    constexpr uint8_t majorVersion = 1;
    constexpr uint8_t minorVersion = 0;
    constexpr uint8_t buildVersion = 0;
    constexpr std::string_view  versionString = "1.0.0";

    constexpr uint32_t version = ChronoAPI::makeVersion(majorVersion, minorVersion, buildVersion);
    // ---------------------------------------------------------------------------
    enum class APIError : ChronoAPI::error_t {
        NoError = 0,
        InvalidArgument,
        // Client errors
        //Разделить на части. Области для каждой версии API, области для реализация API. enum не подходит, нужно int32 
        // Server errors

        MaxError
    };
    // ----
    ChronoAPI::error_t makeError(APIError error) {
        return majorVersion << 24 | static_cast <ChronoAPI::error_t> (error) & 0xffffff;
    }
    // ----
    std::map<ChronoAPI::error_t, const std::string> errorMap
    {
        {static_cast<ChronoAPI::error_t>(APIError::NoError), "no error"},
        { static_cast<ChronoAPI::error_t>(APIError::InvalidArgument), "InvalidArgument" }
    };
    // ----
    std::string errorString(ChronoAPI::error_t errorCode) {
        auto [majAPI, minAPI, buildAPI] = ChronoAPI::getVersion(version);
        if (ChronoAPI::getMajorErrorCode(errorCode) == majAPI) {
            ChronoAPI::error_t minorCode = ChronoAPI::getMinorErrorCode(errorCode);
            if (ChronoAPI_1_0::errorMap.count(minorCode))
                return ChronoAPI_1_0::errorMap[minorCode];
            else
                return "unknown error";
        }
        else
            return ChronoAPI::errorString(errorCode);
    }
    // ---------------------------------------------------------------------------
    enum class APICommandEnum : int8_t {
        ClientPush,
        ClientSubscribe,
        ClientConnect,
        ClientDisconnected,
        ClientSendTimesync
    };

    // ---------------------------------------------------------------------------
    template <class T,auto E>
    class PushCommand : public ChronoAPI::APICommand<PushCommand<T, E>, E> {
    public:
        using derive = T;
    };
    // ---------------------------------------------------------------------------
    template <class Impl, class... Comm>
    class CommandPack {
    public:
        template <class... Data>
        static ChronoAPI::error_t push(Data... data) {
            try {
                ChronoAPI::callSerializeHelper< APICommandEnum::ClientPush, Comm..., Data...>(data...);
                throw APIError::InvalidArgument;
            }
            catch (ChronoAPI::error_t error_code) {
                std::cout<<"push serialize error: " <<  errorString(error_code) << std::endl;
            }
            return makeError(APIError::NoError);
        }
        //// Client producer
        //virtual APIError push() = 0;
        //virtual APIError subscribe() = 0;
        //virtual APIError connect() = 0;
        //virtual APIError disconnected() = 0;
        //virtual APIError sendTimesync() = 0;
        //// Server producer
        //virtual APIError updateData() = 0;
        //virtual APIError askSubscribe() = 0;
        //virtual APIError askDisconnect() = 0;
        //virtual APIError refuseConnection() = 0;
        //virtual APIError aceptConnection() = 0;
        //virtual APIError askTimesync() = 0;
    };
    // ---------------------------------------------------------------------------
    template<class CommandPack>
    class IAPI: public ChronoAPI::IAPICommon<IAPI<CommandPack>, version> {
    public:



        template <class... Data>
        APIError push(Data... data) {
            CommandPack::push(data...);
            return APIError::NoError;
        }

        template <class... Data>
        APIError subscribe(Data... data) {

            return APIError::NoError;
        }

    };
    //Нужен еще один объект для абстрагирования уровня командр. IAPI необходимо сделать самым верхним уровнем, которым будут 
        //пользоваться. В нем 4 метода push, subscribe, connect, disconnect. В базовый класс необходимы мехонизмы оповещения.

}
#endif

