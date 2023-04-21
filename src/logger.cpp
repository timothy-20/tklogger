#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>

#include "logger.h"

#define DEBUG_MESSAGE_MAX_SIZE 8192

logger_configuration::logger_configuration(logger_configuration::log_level level) :
_level(level),
show_file_name(false),
show_function_name(false),
show_line_number(false) {

}

std::string logger_configuration::get_level_string() const {
    switch (this->_level) {
        case log_level::DEBUG: return "DEBUG";
        case log_level::INFO: return "INFO";
        case log_level::WARNING: return "WARNING";
        case log_level::ERROR: return "ERROR";
    }
}

void logger::log(logger_configuration const& configuration,
                 char const* file_name,
                 char const* function_name,
                 int line_number,
                 char const* format, ...) {
    std::stringstream ss;

    ss << logger::get_current_timestamp_string() << ' ';
    ss << '[' << configuration.get_level_string() << "] ";

    if (configuration.show_file_name) {
        ss << file_name << ' ';
    }

    if (configuration.show_function_name) {
        ss << function_name << ' ';
    }

    if (configuration.show_line_number) {
        ss << line_number << ' ';
    }

    std::va_list args;
    char debug_message[DEBUG_MESSAGE_MAX_SIZE];

    va_start(args, format);
    snprintf(debug_message, sizeof(debug_message), format, args);
    ss << std::string(debug_message);

    std::string log_message(ss.str());

    std::cout << log_message << '\n';

    if (logger::is_valid_file_path(configuration.file_path)) {
        if (logger::is_enable_write_permission(configuration.file_path)) {
            std::ofstream file_stream(configuration.file_path, std::ios_base::app);

            if (file_stream.is_open()) {
                file_stream << log_message << '\n';
                file_stream.close();

            } else {
                std::cerr << "Fail to open '" << configuration.file_path << "' file for write log.\n";
            }

        } else {
            std::cerr << "Don't have access to this file write.\n";
        }

    } else {
        std::cerr << "File path is invalid. " << configuration.file_path << '\n';
    }
}

std::string logger::get_current_timestamp_string() {
    using system_clock = std::chrono::system_clock;

    auto now_point(system_clock::now());
    auto now_time_t(system_clock::to_time_t(now_point));
    auto now_time_format(std::localtime(&now_time_t));
    std::ostringstream oss;

    oss << std::put_time(now_time_format, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

bool logger::is_valid_file_path(const std::string &file_path) {
    return (std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path));
}

bool logger::is_enable_write_permission(const std::string &file_path) {
    std::error_code code;
    std::filesystem::perms permissions(std::filesystem::status(file_path, code).permissions());

    if (code) {
        std::cerr << "Fail to get file write permission. " << code.message() << '\n';

        return false;
    }

    return (permissions & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
}