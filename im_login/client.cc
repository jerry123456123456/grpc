#include <iostream>
#include <memory>
#include <string>
// /usr/local/include/grpcpp/grpcpp.h
#include <grpcpp/grpcpp.h>
// 包含我们自己proto文件生成的.h
#include "IM.Login.pb.h"
#include "IM.Login.grpc.pb.h"
// 命名空间
// grcp
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
// 自己proto文件的命名空间
using IM::Login::ImLogin;
using IM::Login::IMRegistReq;
using IM::Login::IMRegistRes;
using IM::Login::IMLoginReq;
using IM::Login::IMLoginRes;

class ImLoginClient{
public:
    ImLoginClient(std::shared_ptr<Channel> channel)
        :stub_(ImLogin::NewStub(channel)) {
    }
    void Regist(const std::string &user_name,const std::string &password){
        IMRegistReq request;
        request.set_user_name(user_name);
        request.set_password(password);

        IMRegistRes response;
        ClientContext context;
        std::cout << "-> Regist req" << std::endl;
        Status status = stub_->Regist(&context,request,&response);
        if(status.ok()){
            std::cout << "user_name:" << response.user_name() << ",user_id:" << response.user_id() << std::endl;
        }else{
            std::cout << status.error_code() << ": " << status.error_message()<<std::endl;
        }
    }

    void Login(const std::string &user_name, const std::string &password) {
        IMLoginReq request;
        request.set_user_name(user_name);
        request.set_password(password);
        IMLoginRes response;
        ClientContext context;
        std::cout << "-> Login req" << std::endl;
        Status status = stub_->Login(&context, request, &response);
        if(status.ok()) {
        std::cout << "user_id:" << response.user_id() << " login ok" <<
        std::endl;
        } else {
        std::cout << "user_name:" << request.user_name() << "Login failed:" << response.result_code()<< std::endl;
        }
    }

private:
    std::unique_ptr<ImLogin::Stub> stub_; 
};

//照葫芦画瓢
int main(){
    //服务器的地址
    std::string server_addr = "localhost:50051";
    //创建请求通道
    ImLoginClient im_login_client{
        grpc::CreateChannel(server_addr,grpc::InsecureChannelCredentials())  
    };
    std::string user_name = "jerry";
    std::string password = "123456";
    //发送请求
    //注册
    im_login_client.Regist(user_name, password);
    //登录
    im_login_client.Login(user_name, password);

    return 0;
}

