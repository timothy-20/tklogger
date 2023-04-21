#include <iostream>
#include <cstdarg>

struct logger_configuration {
public:
    enum log_level : uint16_t {
        DEBUG = 0,
        INFO,
        WARNING,
        ERROR
    };

public:
    explicit logger_configuration(log_level level);
    std::string get_level_string() const;

public:
	bool show_file_name;
	bool show_function_name;
	bool show_line_number;
	std::string file_path;

private:
    log_level _level;
};

class logger {
public:
	static void log(logger_configuration const& configuration,
                    char const* file_name,
                    char const* function_name,
                    int line_number,
                    char const* format, ...);

private:
	static std::string get_current_timestamp_string();
    static bool is_valid_file_path(std::string const& file_path);
    static bool is_enable_write_permission(std::string const& file_path);
};

#define LOG(conf, fmt, ...) logger.log(conf, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)