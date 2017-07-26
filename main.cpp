
#include <string>
#include <vector>
#include <iostream>
#include <cstddef>
#include <cassert>
#include "conf_file.hpp"
#include "../xtsslib/afilesystem.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //source : https://github.com/nothings/stb/blob/master/stb_image.h

struct image_local
{
	unsigned char * data;
	int x;
	int y;
	int image_component;

	image_local(const astd::filesystem::path& path, int desired_channel = 0)
	{
		data = stbi_load(path.string().data(), &x, &y, &image_component, desired_channel);
	}

	bool valid() const { return data != nullptr; }

	~image_local()
	{
		if (data)
		{
			stbi_image_free(data);
		}
	}
};


std::vector<astd::filesystem::path> fetch_wallpaper_list(const std::string& path, bool filter = true)
{
	std::vector<astd::filesystem::path> result;
	astd::filesystem::directory_iterator it(path);
	astd::filesystem::directory_iterator ite;
	astd::filesystem::path file_path;

	while (it != ite)
	{
		file_path = it->path();
		if (!filter) {
			result.push_back(file_path);
		}
		else {
			image_local img(file_path);

			if (img.valid() && img.x >= 1920 && img.y >= 1080 && img.x > img.y)
			{
				result.push_back(file_path);
			}
		}
		++it;
	}
	return result;
}

void copy_file(const std::vector<astd::filesystem::path>& file_to_copy, const std::string& to)
{
	astd::filesystem::path destination(to);

	for (const auto& f : file_to_copy)
	{
		astd::filesystem::path new_path = destination / f.filename();
		new_path.replace_extension(".jpg");
		astd::filesystem::copy_file(f, new_path);
	}
}

std::vector<astd::filesystem::path> remove_intersection(const std::vector<astd::filesystem::path>& current_wallpaper, const std::vector<astd::filesystem::path>& spotlight_wallpaper)
{
	std::vector<astd::filesystem::path> result;

	for (const auto& new_wall : spotlight_wallpaper)
	{
		auto new_wall_filename = new_wall.filename().replace_extension(".jpg");
		auto found_it = std::find_if(current_wallpaper.begin(), current_wallpaper.end(), [&new_wall_filename](const auto& cur_wall) {
			return new_wall_filename == cur_wall.filename();
		});
		if (found_it == current_wallpaper.end())
			result.push_back(new_wall);
	}
	return result;
}

int main(int argc, char** argv)
{
	if (argc < 1)
	{
		std::cout << "usage : retrieve_spotlight <config_file_path>" << std::endl;
	}

	conf_file conf(argv[1]);

	if (!astd::filesystem::exists(conf.wallpaper_dir)
		|| !astd::filesystem::exists(conf.source_dir))
	{
		std::cerr << "configuration file invalid !" << std::endl;
		return 1;
	}

	std::vector<astd::filesystem::path> current_wallpaper = fetch_wallpaper_list(conf.wallpaper_dir, false);
	std::vector<astd::filesystem::path> spotlight_wallpaper = fetch_wallpaper_list(conf.source_dir);

	std::sort(current_wallpaper.begin(), current_wallpaper.end());
	std::sort(spotlight_wallpaper.begin(), spotlight_wallpaper.end());

	copy_file(remove_intersection(current_wallpaper, spotlight_wallpaper), conf.wallpaper_dir);
	return 0;
}