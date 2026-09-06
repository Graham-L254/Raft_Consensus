#include "main.h"

auto startTime = std::chrono::steady_clock::now();

using boost::asio::ip::udp;
using boost::asio::ip::tcp; 

enum message_type_udp : char{
    dataSend,            //message type, term (num), value, leader
    announceCandidacy,   //message type, address
    vote,                //message type, address (just sends message to candidate)
    catchUpRequest,      //message type, address, 
    requestTask,         //message type, address, potentially other speed related stuff here
    taskSucced,          //message type, Succesful Value, name / address
    newConnection,       //message type, address
    catchUpRequest,      //message type, last term (num), last term value
};


struct UDP_receive {
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
    int buffer_size;
    //boost::asio::io_context& context;

    UDP_receive(boost::asio::io_context& io_context, udp::endpoint endpoint) : socket_(io_context, endpoint) {
        std::cout << "UDP server listening on port " << endpoint << "\n";
        socket.non_blocking(true);
    }

    void start_receive() {
        buffer_size = socket_.receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_);
    }

    void received_data(boost::system::error_code ec, std::size_t bytes_recvd) {
        if (!ec) {
            std::cout.write(recv_buffer_.data(), bytes_recvd);
            start_receive();
        }else if (ec == boost::asio::error::would_block) {
            if (!mine()){
                start_receive();
            }
        }else if (ec != boost::asio::error::operation_aborted) {
            std::cerr << "receive error: " << ec.message() << "\n";
            start_receive();
        }
    }
};

struct time_manager{
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    int millesecondsToWait {};
    
    time_manager(int time) : millesecondsToWait{time}{
        resetTimer();
    }

    time_manager() : millesecondsToWait{random_num_gen()}{
        resetTimer();
    }

    void resetTimer(){
        lastTime = std::chrono::steady_clock::now();
    }

    bool check_if_time_elapsed(){
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastTime).count();
        if (elapsed > millesecondsToWait){
            return true;
        }
        return false;
    }

    int random_num_gen(int lower = 100, int upper = 120){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(lower, upper); 
        return distribution(gen);
    }
};

struct data_storage(){
    int term_num {};
    std::vector<int> term_num {};

    data_storage(){}

    bool add_data(int value, int term){
        if (term != term_num + 1){
            return false;
        }else{
            term_num.push_back(value);
            return true;
        }
    }
}

struct server {
    char *name;
    std::string host;
    std::string port;
    int sockfd;
    int term {};
    bool isLeader {false};
    int time_to_wait {};
    data_storage _data {};
    
    boost::asio::io_context io_context;

    std::optional<udp::socket> recieving_socket;
    std::optional<udp::socket> sending_socket;
    UDP_receive* input;
    timer* = new time_manager();


    std::thread repeatedly_check;
    std::thread mining_thread;


    std::vector<char> message_buffer;

    std::vector<int> dataHeld;

    server(std::string host = "127.0.0.1", std::string port = "8000"){

        udp::resolver resolver(io_context);
        udp::endpoint server_endpoint =
            *resolver.resolve(host, port).begin();

        recieving_socket.emplace(io_context);
        recieving_socket.value().open(udp::v4());

        input = new UDP_receive(io_context, server_endpoint);

        std::cout << "about to check" << "\n";

        sending_socket.emplace(io_context);
        sending_socket.value().open(udp::v4());

        repeatedly_check = std::thread(&server::start_loop, this);

        std::cout << "here" << "\n";
    }

    void start_loop(){
        while (true){
            await_message();
            timer->check_if_time_elapsed();
        }
    }

/*
    dataSend,            //message type, term (num), value, leader
    announceCandidacy,   //message type, address
    vote,                //message type, address (just sends message to candidate)
    requestTask,         //message type, address, potentially other speed related stuff here
    taskSucced,          //message type, Succesful Value, name / address
    newConnection,       //message type, address
    catchUpRequest,      //message type, last term (num), last term value
*/

    void await_message(){

        input->start_receive();

        if (input->buffer_size() > 0) {
            
            if (input->recv_buffer_[0] == dataSend) {
                // Handle dataSend message
                data_received();
            } else if (input->recv_buffer_[0] == announceCandidacy) {
                // Handle announceCandidacy message
                std::cout << "Handling announceCandidacy message." << "\n";
            } else if (input->recv_buffer_[0] == vote) {
                // Handle vote message
                std::cout << "Handling vote message." << "\n";
            } else {
                std::cerr << "Unknown message type received." << "\n";
            }

            input->buffer_size = 0;
        }
    }

    void data_received(){
        _data.add_data(,);
    }

    //leader only function
    void send_heartbeat(){

    }

    void start_candidacy(){

    }

    bool send_(std::string message, std::string host, std::string port){

        std::cout << "sending" << "\n";



        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint server_endpoint = *resolver.resolve(host, port).begin();



        //(recieving_server->socket.value()).async_send_to(boost::asio::buffer(message), server_endpoint);
        (sending_socket.value()).send_to(boost::asio::buffer(message), server_endpoint);

        std::cout << "finished send" << "\n";

        return true;
    }

    bool mine(){
        if (true/*check()*/){
            return true;
        }
        return false;
    }

};






