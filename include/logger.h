#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>

#define MESSAGE_MAX_SIZE 8192

struct logger_configuration {
	bool show_file_name;
	bool show_function_name;
	bool show_line_number;
	std::string file_path;
};

class logger {
public:
	enum log_level : uint16_t {
		DEBUG = 0,
		INFO,
		WARNING,
		ERROR
	};

	using log_level_string = std::string;

public:
	static logger& default_logger() {
		static logger LOGGER;

		return LOGGER;
	}

	void set_configuration(logger_configuration const& configuration) {
		this->_configuration = configuration;
	}

	void log(log_level level, std::string& file_name, std::string& function_name, std::string& line_number, char const* format, char const* ...) {
		std::stringstream ss;

		ss << this->get_current_timestamp_string() << ' ';
		ss << '[' << logger::__LEVELS[level] << "] ";
		
		// 파일명 표시 옵션이 활성화되어 있는 경우
		if (this->_configuration.show_file_name) {
			ss << file_name << ' ';
		}

		// 함수명 표시 옵션이 활성화되어 있는 경우
		if (this->_configuration.show_function_name) {
			ss << function_name << ' ';
		}

		// 실행 라인 표시 옵션이 활성화되어 있는 경우
		if (this->_configuration.show_line_number) {
			ss << line_number << ' ';
		}

		va_list args;

	}

	bool is_create_file() const {
		return this->_enable_create_file;
	}

private:
	std::string get_current_timestamp_string() const {
		using system_clock = std::chrono::system_clock;

		auto now_point(system_clock::now());
		auto now_time_t(system_clock::to_time_t(now_point));
		auto now_time_format(std::localtime(now_time_t));
		std::ostringstream oss;

		oss << std::put_time(now_time_format, "%Y-%m-%d %H:%M:%S");

		return oss.str();
	}

private:
	logger_configuration _configuration;
	bool _enable_create_file;
	static log_level_string const __LEVELS[4] = {
		"DEBUG",
		"INFO",
		"WARNING",
		"ERROR"
	};
};