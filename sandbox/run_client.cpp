#include <functional>
#include <memory>
#include <iostream>
#include <vector>
#include <thread>

#include <magellan/magellan.hpp>
#include <cereal/types/vector.hpp>
#include "stock.hpp"

namespace magellan {

class client {
    public:
        client(asio::io_service& io_service, const std::string& host, const std::string& service) : connection_(io_service) {
            connection_.connect(io_service, host, service, std::bind(&client::handle_connect, this, std::placeholders::_1));
        }

        void handle_connect(const asio::error_code& e) {
            if (!e) {
                //connection_.async_read(stocks_, std::bind(&client::handle_read, this, std::placeholders::_1));
                stocks_ = connection_.async_read<std::vector<stock>>().get();
                handle_read();
            } else {
                std::cerr << e.message() << std::endl;
            }
            connection_.finish();
        }

        /// Handle completion of a read operation.
        //void handle_read(const asio::error_code& e) {
        void handle_read() {
            // Print out the data that was received.
            for (std::size_t i = 0; i < stocks_.size(); ++i) {
                std::cout << "Stock number " << i << "\n";
                std::cout << "  company: " << stocks_[i].company << "\n";
                std::cout << "  code: " << stocks_[i].code << "\n";
                std::cout << "  name: " << stocks_[i].name << "\n";
                std::cout << "  open_price: " << stocks_[i].open_price << "\n";
                std::cout << "  high_price: " << stocks_[i].high_price << "\n";
                std::cout << "  low_price: " << stocks_[i].low_price << "\n";
                std::cout << "  last_price: " << stocks_[i].last_price << "\n";
                std::cout << "  buy_price: " << stocks_[i].buy_price << "\n";
                std::cout << "  buy_quantity: " << stocks_[i].buy_quantity << "\n";
                std::cout << "  sell_price: " << stocks_[i].sell_price << "\n";
                std::cout << "  sell_quantity: " << stocks_[i].sell_quantity << "\n";
            }
            //} else {
                //// An error occurred.
                //std::cerr << e.message() << std::endl;
            //}
        }

    private:
        connection connection_;
        std::shared_ptr<asio::io_service::work> work_;
        std::vector<stock> stocks_;
};

} // magellan

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        asio::io_service io_service;
        //auto work = std::make_shared<asio::io_service::work>(io_service);
        magellan::client client(io_service, argv[1], argv[2]);
        std::thread t([&io_service] () { io_service.run(); });
        t.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
