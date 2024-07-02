#pragma once

#include "restclient-cpp/connection.h"

using namespace RestClient;

class CloudGateWay
{
public:
    CloudGateWay(std::string url);
    ~CloudGateWay();
    bool sendData(std::string &data);

private:
    Connection *m_conn;
    HeaderFields m_headers;
    Response m_resp;
};