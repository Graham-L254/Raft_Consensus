#include "main.h"
#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <string>
#include <chrono>

auto startTime = std::chrono::steady_clock::now();


auto endTime = std::chrono::steady_clock::now();
auto duration = std::chrono::duration<double, std::milli>(endTime - startTime);
std::cout << "It took " << duration.count() << " milliseconds.\n";

using boost::asio::ip::udp;

enum message_type : char{
    dataSend,            //message type, term (num), value, leader
    announceCandidacy,   //message type, address
    vote,                //message type, address
    join,                //message type, address
    newCon,              //message type, address
}


struct server {
    char *name;
    char *ip;
    int port;
    int sockfd;
    int term {};
    bool isLeader {false};
    
    auto lastHeartbeat = std::chrono::steady_clock::now();

    std::string host = "127.0.0.1";
    std::string port = "8000";

    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint server_endpoint =
        *resolver.resolve(udp::v4(), host, port).begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    UDP_recieve input = new UDP_recieve(io_context, socket);

    std::vector<char> message_buffer;

    std::vector<int> dataHeld;

    void check(){
        if (isLeader){

        }else{
            
        }
    }


    void await_message();
    void send_heartbeat();

    void await_message() {
        if (sockfd < 0) {
            std::cerr << "Socket not initialized." << std::endl;
            return;
        }
        else if (name == nullptr || ip == nullptr) {
            std::cerr << "Server name or IP is not set." << std::endl;
            return;
        }
        else if (port <= 0) {
            std::cerr << "Invalid port number." << std::endl;
            return;
        }
        else {
            char buffer[1024];
            int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received < 0) {
                std::cerr << "Error receiving message." << std::endl;
            } else if (bytes_received == 0) {
                std::cout << "Connection closed by server." << std::endl;
            } else {
                buffer[bytes_received] = '\0'; // Null-terminate the received data
                std::cout << "Received message: " << buffer << std::endl;
            }
        }

        if (message_buffer.size() > 0) {
            
            if message_buffer[0] == dataSend {
                // Handle dataSend message
                std::cout << "Handling dataSend message." << std::endl;

            } else if (message_buffer[0] == announceCandidacy) {
                // Handle announceCandidacy message
                std::cout << "Handling announceCandidacy message." << std::endl;
            } else if (message_buffer[0] == vote) {
                // Handle vote message
                std::cout << "Handling vote message." << std::endl;
            } else {
                std::cerr << "Unknown message type received." << std::endl;
            }

            message_buffer.clear();
        }

    }

    void data_recieved(){
        if (message_buffer[])
    }

    //leader only function
    void send_heartbeat(){

    }

    void start_candidacy(){

    }

    mine(){
        check();
    }

};



struct UDP_recieve {
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;

    UdpServer(boost::asio::io_context& io_context, unsigned short port) : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        std::cout << "UDP server listening on port " << port << "\n";
        start_receive();
    }

    bool start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    std::string msg(recv_buffer_.data(), bytes_recvd);
                    return true;
                }
                return start_receive();
            });
    }
    
}



int main(int argc, char* argv[]) {
    try {
        unsigned short port = 8888;
        if (argc >= 2) {
            port = static_cast<unsigned short>(std::stoi(argv[1]));
        }

        boost::asio::io_context io_context;
        UdpServer server(io_context, port);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

// udp_client.cpp
// Simple Boost.Asio UDP client that sends messages and prints the echo reply
// Build: g++ -std=c++17 udp_client.cpp -o udp_client -lboost_system -lpthread






int client(){
    std::string host = "127.0.0.1";
    std::string port = "8000";

    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint server_endpoint =
        *resolver.resolve(udp::v4(), host, port).begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::string line;
    std::array<char, 1024> recv_buffer;

    while (std::getline(std::cin, line)) {
        if (line == "quit"){
            break;
        }

        socket.send_to(boost::asio::buffer(line), server_endpoint);

        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(
            boost::asio::buffer(recv_buffer), sender_endpoint);

        std::cout << "Reply: "<< std::string(recv_buffer.data(), len) << "\n";
    }

}