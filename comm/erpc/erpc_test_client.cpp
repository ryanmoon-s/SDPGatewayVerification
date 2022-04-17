#include "erpc_client.h"

int mini_client_ssl();

int main() {
    int ret = 0;
    erpc::FuncReverseReq req;
    erpc::FuncReverseRsp rsp;
    Header header;

    req.set_str("hello world");
    ret = Client().FuncReverseClient(req, rsp, header);
    iAssert(ret, ("FuncReverseClient"));
    
    TLOG_DBG(("FuncReverse Sucess: req.str:%s, rsp.str:%s", req.str().c_str(), rsp.str().c_str()));
    TLOG_DBG(("FuncReverse Sucess: header code:%d, msg:%s", header.ret_code, header.ret_msg.c_str()));

    return 0;

    // mini_client_ssl();
}

int mini_client_ssl()
{
    int fd = 0, ret = 0;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TCP_PORT_CONTROLLER);  
    addr.sin_addr.s_addr = inet_addr(IP_CONTROLLER); 

    TLOG_DBG(("1"));

    ret = connect(fd, (struct sockaddr*)&addr, (socklen_t)sizeof(addr));
    iAssert(ret, ("connect"));

    TLOG_DBG(("2 fd:%d", fd));

    SSLConnector connector(SSL_CRT_CLIENT, SSL_KEY_CLIENT, SSL_SELECT_CLIENT);

    // ERROR
    ret = connector.SSLConnect(fd);
    iAssert(ret, ("SSLConnect"));

    TLOG_DBG(("3"));

}