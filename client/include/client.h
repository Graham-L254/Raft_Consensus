#include "main.h"


auto startTime = std::chrono::steady_clock::now();


//auto endTime = std::chrono::steady_clock::now();
//auto duration = std::chrono::duration<double, std::milli>(endTime - startTime);
//std::cout << "It took " << duration.count() << " milliseconds.\n";

using boost::asio::ip::udp;
using boost::asio::ip::tcp; 

enum message_type_udp : char{
    dataSend,            //message type, term (num), value, leader
    announceCandidacy,   //message type, address
    vote,                //message type, address
    requestTask,         //message type, address, potentially other speed related stuff here
};

enum message_type_tcp : char{
    taskSucced,          //message type, Succesful Value, name / address
    newConnection,       //message type, address
};

struct UDP_receive {
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
    //boost::asio::io_context& context;

    UDP_receive(boost::asio::io_context& io_context, udp::endpoint endpoint) : socket_(io_context, endpoint) {
        std::cout << "UDP server listening on port " << endpoint << "\n";
        //context = io_context;
        start_receive();
    }

    bool start_receive() {
        std::cout << "started receive";
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,     
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                recieved_data(ec, bytes_recvd);
            });
        
        
        //std::cout << recv_buffer_.data() << "\n";
        std::cout << "got past asyncRecieve";
    }

    void recieved_data(boost::system::error_code ec, std::size_t bytes_recvd){
        if (!ec && bytes_recvd > 0) {
            // Bounds-checked: bytes_recvd <= recv_buffer_.size(), guaranteed by ASIO
            std::cout << "Received " << bytes_recvd << " bytes from "
                      << remote_endpoint_.address().to_string() << ":"
                      << remote_endpoint_.port() << "\n";
            std::cout << recv_buffer_.data();


        } else if (ec) {
            std::cerr << "Receive error: " << ec.message() << "\n";
        }else{
            std::cout << "ran recieved data no error, no received";
        }
        start_receive();
    }


    //
    
};

struct server {
    char *name;
    std::string host;
    std::string port;
    int sockfd;
    int term {};
    bool isLeader {false};
    
    boost::asio::io_context io_context;

    std::optional<udp::socket> socket;
    UDP_receive* input;

    std::thread repeatedly_check;


    std::vector<char> message_buffer;

    std::vector<int> dataHeld;

    server(std::string host = "127.0.0.1", std::string port = "8000"){

        udp::resolver resolver(io_context);
        udp::endpoint server_endpoint =
            *resolver.resolve(host, port).begin();

        socket.emplace(io_context);
        socket.value().open(udp::v4());

        repeatedly_check = std::thread(&server::start_loop, this);

        input = new UDP_receive(io_context, server_endpoint);
    }

    void start_loop(){
        while (true){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            io_context.run(); 
        }
    }


    bool check(){
        if (isLeader){

        }else{
            
        }
        return true;
    }

    void await_message(){
        if (sockfd < 0) {
            std::cerr << "Socket not initialized." << std::endl;
            return;
        }
        else if (name == nullptr || host == "") {
            std::cerr << "Server name or IP is not set." << std::endl;
            return;
        }
        /*else if (port <= 0) {
            std::cerr << "Invalid port number." << std::endl;
            return;
        }*/
        else {
            char buffer[1024];
            int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received < 0) {
                std::cerr << "Error receiving message." << std::endl;
            } else if (bytes_received == 0) {
                std::cout << "Connection closed by server." << std::endl;
            } else {
                buffer[bytes_received] = '\0'; // Null-terminate the received data
                std::cout << "received message: " << buffer << std::endl;
            }
        }

        if (message_buffer.size() > 0) {
            
            if (message_buffer[0] == dataSend) {
                // Handle dataSend message
                data_received();

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

    void data_received(){
        if (message_buffer[0] == 0){

        }
    }

    //leader only function
    void send_heartbeat(){

    }

    void start_candidacy(){

    }

    bool mine(){
        if (check()){
            return true;
        }
        return false;
    }

};






