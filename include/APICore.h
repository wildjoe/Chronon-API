#ifndef APICORE_H
#define APICORE_H
#include <vector>
#include <map>

namespace ChronoAPI {

    // ---------------------------------------------------------------------------
    using binary_data_t = std::vector<int8_t>;
    using error_t = uint32_t;
    // ----

    template <auto ID, class Head, class... Tails, class... Data>
    static constexpr binary_data_t callSerializeHelper(Data... data) {
        if constexpr (Head::id == ID) {
            return Head::serialize(data...);
        }
        else
            return callSerializeHelper<ID, Data...>(data...);
    }
    // ----
    template <auto ID, class... Data>
    static constexpr binary_data_t callSerializeHelper(Data... data) {
        static_assert(false, "Incorrect Command ");
        return binary_data_t();
    }       
    // ---------------------------------------------------------------------------
    constexpr uint32_t makeVersion(const uint8_t major, const uint8_t minor, const uint16_t build) {
        return major << 24 | minor << 16 | build;
    }
    // ---------------------------------------------------------------------------
    constexpr std::tuple<uint8_t, uint8_t, uint16_t> getVersion(uint32_t version) {
        return std::tuple<uint8_t, uint8_t, uint16_t>{(version>>24)&0xff, (version >> 16) & 0xff, version  & 0xffff};
    }
    // ---------------------------------------------------------------------------
    // ----
    const error_t APIVersionMaskError = 0xff000000;
    const uint8_t APIVersionShiftError = 24;
    const error_t MaxGeneralError = 0x00ffffff;
    error_t getMajorErrorCode(error_t errorCode) {
        return errorCode & ~MaxGeneralError;
    }
    // ----
    error_t getMinorErrorCode(error_t errorCode) {
        return errorCode & MaxGeneralError;
    }
    // ----
    enum class APIError : uint32_t {
        NoError = 0,
        // Client errors
        //Разделить на части. Области для каждой версии API, области для реализация API. enum не подходит, нужно int32 
        // Server errors

        MaxError
    };
    // ----
    std::map<error_t, const std::string> errorMap
    { 
        {static_cast<error_t>(APIError::NoError), "no error"}
    };
    // ----
    std::string errorString(error_t errorCode) {
        if (getMajorErrorCode(errorCode) == 0) { // Core API Errors
            error_t minorCode = getMinorErrorCode(errorCode);
            if (ChronoAPI::errorMap.count(minorCode))
                return ChronoAPI::errorMap[minorCode];
            else
                return "unknown error";
        } else
            return "unknown API version";
    }

    // ---------------------------------------------------------------------------
    error_t makeErrorCode(error_t errorMinor, uint8_t apiVersion) {
        return errorMinor & MaxGeneralError | apiVersion << APIVersionShiftError;
    }

    // ---------------------------------------------------------------------------
    template <class T, auto Id>
    class APICommand {
    public:
        using Type = decltype(Id);
        static constexpr auto id = Id;

        template <class... Data>
        static binary_data_t serialize(Data... data)  {
            return T::derive::serialize_impl(data...);
        };

        static bool parse(binary_data_t data) {
            return T::derive::parse_impl(data);
        };
    };
    // ---------------------------------------------------------------------------
    template <auto I, class T>
    class CommandPair {
    public:
        using ID = decltype(I) ;
        static constexpr auto id = I;
        using type = T;
    };
    // ---------------------------------------------------------------------------
    template <class Base, uint32_t V>
    class IAPICommon {
    public:
        static constexpr uint32_t version = V;

        template<class Data >
        APIError connect(Data data) {
        }

        APIError disconnect() {
        }

    };

}

#endif
