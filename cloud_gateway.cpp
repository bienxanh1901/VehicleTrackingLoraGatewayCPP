#include "cloud_gateway.h"

#include "restclient-cpp/restclient.h"

CloudGateway::CloudGateway(std::string url)
{
    RestClient::init();
    
    m_conn = new RestClient::Connection(url);
    m_conn->SetTimeout(10);
    m_conn->SetUserAgent("team/promise");

    m_headers["Content-Type"] = "text/plain";
    
    m_conn->SetHeaders(m_headers);

}

CloudGateway::~CloudGateway()
{
    delete m_conn;
    RestClient::disable();
}

bool CloudGateway::send_data(std::string data)
{
    m_resp = m_conn->post("/api/v2/device_request", data);
    return true;
}