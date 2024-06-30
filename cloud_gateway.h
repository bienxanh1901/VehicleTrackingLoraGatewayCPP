#pragma once

#include "restclient-cpp/connection.h"

using namespace RestClient;

class CloudGateway
{
public:
    CloudGateway(std::string url);
    ~CloudGateway();
    bool send_data(std::string data);

private:
    Connection *m_conn;
    HeaderFields m_headers;
    Response m_resp;
};