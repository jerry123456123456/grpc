#include <iostream>
#include <string>
// grpc头文件
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
// 包含我们自己proto文件生成的.h
#include "IM.Login.pb.h"
#include "IM.Login.grpc.pb.h"
// 命名空间
// grcp
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
// 自己proto文件的命名空间
using IM::Login::ImLogin;
using IM::Login::IMRegistReq;
using IM::Login::IMRegistRes;
using IM::Login::IMLoginReq;
using IM::Login::IMLoginRes; 

//继承类
class IMLoginServiceImpl : public ImLogin::Service{
    //重写注册方法
    virtual Status Regist(ServerContext* context, const IMRegistReq* request,IMRegistRes* response) override {
        std::cout<< "Regist " << request->user_name() << std::endl;
        //设置用户名
        response->set_user_name(request->user_name());
        response->set_user_id(10);
        response->set_result_code(0);
        return Status::OK;
    }
    //重写登录方法
    virtual Status Login(ServerContext* context, const IMLoginReq* request,IMLoginRes* response) override {
        std::cout<< "Login " << request->user_name() << std::endl;
        response->set_user_id(10);
        response->set_result_code(0);
        return Status::OK;
    }
};


void RunServer(){
    //服务器对应的端口号
    std::string server_addr("0.0.0.0:50051");
    //创建一个服务类
    IMLoginServiceImpl service;

    ServerBuilder builder;
    //通过build服务和地址关联起来
    builder.AddListeningPort(server_addr,grpc::InsecureServerCredentials());
    //设置时间和超时
    builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIME_MS,5000);
    builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIMEOUT_MS,10000);
    //因此，这行代码的作用是向 gRPC 通道参数中添加一个参数，该参数指定了在没有调用的情况下是否允许保持连接。在这种情况下，设置为 `1` 表示允许在没有调用的情况下保持连接。
    builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);
    
    //把服务做进来
    builder.RegisterService(&service);
    
    //启动
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout<< "Server listening on" << server_addr << std::endl;
    //进入服务循环，等待退出
    server->Wait();
}


int main(){
    RunServer();
    return 0;
}
