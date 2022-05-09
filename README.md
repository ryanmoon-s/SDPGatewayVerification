# SDPGatewayVerification  
  
# 编译  

由于protobuf的不兼容 ————  
需要使用4G及以上机器，使用此命令编译：  
  
bazel build --incompatible_blacklisted_protos_requires_proto_info=false :target  
  

# 连接准备  
  
一：控制器上线 -> (应用网关上线 -> 应用上线, 同机部署)。  
    应用网关连接到控制器并由其进行身份验证。  
    但是，他们不会应答来自任何其他主机的通信，也不会响应非预分配的请求。  
  
二：应用网关 < --- TLS --- > 控制器  
    应用网关向控制器注册应用 < IP, UDP_PORT, TCP_PORT, APP > 列表  
    控制器管理着用户可以访问的应用列表  
  
#  开始连接  
  
三：Client -> 控制器  
    SDA敲门(认证凭证)  
  
四：控制器确认  
    Client可访问的应用列表，即 < IP, UDP_PORT, TCP_PORT, APP, -Ticket- > 列表，一个APP一张票据  
    < 应用主机IP, 网关UDP端口, 应用TCP端口，应用信息, 应用票据 >
  
五：控制器 -> Client  
    Client可访问的应用列表  
  
六：Client -> 应用网关  
    SPA敲门(授权票据) < IP, UDP_PORT >  
    双向TLS连接，访问具体应用 < IP, TCP_PORT, APP, -Ticket- >  
  
    访问app流程：  
    // 一个appgateway对应一个服务   
    // 认证：verify_client -> controller   
    // 认证: verify_client -> gateway   
    // 访问: access_client -> application  
   
七：控制器动态控制用户权限  
    可随时切断用户访问应用(让应用网关对Client加黑)  
  