#ifndef CONF_FILE_HPP
#define CONF_FILE_HPP

#include <string>
#include <fstream>

struct conf_file
{
	conf_file(const std::string& path)
	{
		std::ifstream stream(path);

		if (!stream)
			std::cerr << "configuration files not found at path: " << path << std::endl;

		stream >> source_dir;
		stream >> wallpaper_dir;
	}

	std::string source_dir;
	std::string wallpaper_dir;
};

#endif //!CONF_FILE_HPP