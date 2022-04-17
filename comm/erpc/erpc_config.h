#ifndef ERPC_CONFIG_H
#define ERPC_CONFIG_H

#include <map>
#include <string>

enum IP_WHITE_TABLE_OP {
    IP_WHITE_TABLE_ADD = 1,
    IP_WHITE_TABLE_DEL = 2,
};

class ErpcConfig {
public:
    void IpWhiteTableOp(int op, std::string ip);
    bool IsIpInWhiteTable(std::string ip);
    
private:
    std::map<std::string, int> ip_white_table_;

// 单例
public:
    static ErpcConfig* GetInstance() 
    {
        return instance_;
    }

private:
    ErpcConfig(){};
    ErpcConfig(const ErpcConfig&){};

    static ErpcConfig* instance_;
};




#endif