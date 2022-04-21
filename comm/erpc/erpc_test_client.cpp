#include "erpc_client.h"

int mini_client_ssl();

int main() 
{
    int ret = 0;
    erpc::TestFuncReverseReq req;
    erpc::TestFuncReverseRsp rsp;
    Header header;

    req.set_str("hello world");
    ret = ErpcClient().TestFuncReverseRequest(req, rsp, header);
    iAssert(ret, ("TestFuncReverseRequest"));
    
    TLOG_MSG(("TestFuncReverse Sucess: req.str:%s, rsp.str:%s", req.str().c_str(), rsp.str().c_str()));
    TLOG_MSG(("TestFuncReverse Sucess: header code:%d, msg:%s", header.ret_code, header.ret_msg.c_str()));

    return 0;
}

int mini_client_ssl()
{
    int fd = 0, ret = 0;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TCP_PORT_CONTROLLER);  
    addr.sin_addr.s_addr = inet_addr(IP_CONTROLLER_PB); 

    ret = connect(fd, (struct sockaddr*)&addr, (socklen_t)sizeof(addr));
    iAssert(ret, ("connect"));

    SSLConnector connector(SSL_CRT_CONTROLLER, SSL_KEY_CONTROLLER, SSL_SELECT_CLIENT);

    // ERROR
    ret = connector.SSLConnect(fd);
    iAssert(ret, ("SSLConnect"));
}
