#include "../include/client.h"

bool send_(std::string message, std::string host, std::string port);

using boost::asio::ip::udp;
using boost::asio::ip::tcp;


int main(){
    try{
        boost::asio::io_context io;
        server* server1 = new server();
        
        std::this_thread::sleep_for(std::chrono::seconds(2));  
        send_("hi", "127.0.0.1", "8000");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        delete server1;
    }catch(const boost::system::system_error& e){
        std::cerr << "Message: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.code().value() << "\n";
        std::cerr << "Category: " << e.code().category().name() << "\n";
    }
}



bool send_(std::string message, std::string host, std::string port){

    std::cout << "sent";

    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint server_endpoint = *resolver.resolve(host, port).begin();

    udp::socket socket_(io_context);
    socket_.open(udp::v4());

    //(recieving_server->socket.value()).async_send_to(boost::asio::buffer(message), server_endpoint);
    socket_.send_to(boost::asio::buffer(message), server_endpoint);

    return true;
}