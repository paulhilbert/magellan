#include <functional>
#include <iostream>
#include <vector>

#include <boost/lexical_cast.hpp>

#include <magellan/magellan.hpp>
#include <cereal/types/vector.hpp>
#include "stock.hpp"


class data_kernel {
    public:
        data_kernel(std::string company) {
            stock s;
            s.company = company;
            s.code = "ABC";
            s.name = "A Big Company";
            s.open_price = 4.56;
            s.high_price = 5.12;
            s.low_price = 4.33;
            s.last_price = 4.98;
            s.buy_price = 4.96;
            s.buy_quantity = 1000;
            s.sell_price = 4.99;
            s.sell_quantity = 2000;
            stocks_.push_back(s);
            s.code = "DEF";
            s.name = "Developer Entertainment Firm";
            s.open_price = 20.24;
            s.high_price = 22.88;
            s.low_price = 19.50;
            s.last_price = 19.76;
            s.buy_price = 19.72;
            s.buy_quantity = 34000;
            s.sell_price = 19.85;
            s.sell_quantity = 45000;
            stocks_.push_back(s);
        }

        void accept(const asio::error_code& e, magellan::connection::ptr_t conn) {
            conn->async_write(stocks_, std::bind(&data_kernel::handle_write, this, std::placeholders::_1, conn));
        }

        void handle_write(const asio::error_code& e, magellan::connection::ptr_t conn) {
        }

    protected:
        std::vector<stock> stocks_;
};


int main(int argc, char* argv[]) {
    try {
        // Check command line arguments.
        if (argc != 2) {
        std::cerr << "Usage: server <port>" << std::endl;
        return 1;
        }
        unsigned short port = boost::lexical_cast<unsigned short>(argv[1]);

        asio::io_service io_service;
        magellan::server<data_kernel> server(io_service, port, "My Company");
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
