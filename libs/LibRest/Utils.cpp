#include "Utils.hpp"

namespace LibRest::Utils
{

    bool isError(const web::http::status_code status)
    {
        return status != web::http::status_codes::OK && status != web::http::status_codes::Created
            && status != web::http::status_codes::NoContent && status != web::http::status_codes::Accepted;
    }

}
