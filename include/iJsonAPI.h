#ifndef IJSONAPI_H
#define IJSONAPI_H
#include <tuple>
#include "API_1_0.h"



namespace ChronoAPI_1_0
{
    // ---------------------------------------------------------------------------
    class IJSON_Push : public PushCommand<IJSON_Push,APICommandEnum::ClientPush> {

    public:
        template <class... Data>
        static ChronoAPI::binary_data_t serialize_impl(Data... data) {
            std::tuple<Data...> args(data...);
            auto [id, buff] = args;
            std::cout << "json push: " << " " << id << buff.size() << std::endl;
            return ChronoAPI::binary_data_t();
        };

        static bool parse_impl(ChronoAPI::binary_data_t data) {
            //return T::parse_impl(data)
        };

    };
    // ---------------------------------------------------------------------------




    // ---------------------------------------------------------------------------
    class JsonCommands : public CommandPack<JsonCommands,
        IJSON_Push
    > {};
}

#endif