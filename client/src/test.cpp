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
        std::this_thread::sleep_for(std::chrono::seconds(10));
        delete server1;
    }catch(const boost::system::system_error& e){
        std::cerr << "Message: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.code().value() << "\n";
        std::cerr << "Category: " << e.code().category().name() << "\n";
    }
}



