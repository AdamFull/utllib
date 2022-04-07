#include "zipextractor.hpp"
#include <zip.h>
#include <fstream>

void zipextractor::extract(const fs::path &from, const fs::path &to)
{
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100];
    int64_t sum;
    int len, err;

    if ((za = zip_open(from.string().c_str(), 0, &err)) == NULL)
    {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        throw std::runtime_error(buf);
    }

    for (auto i = 0; i < zip_get_num_entries(za, 0); i++)
    {
        if (zip_stat_index(za, i, 0, &sb) == 0)
        {
            len = strlen(sb.name);
            if (sb.name[len - 1] == '/')
            {
                auto dir_path = to / sb.name;
                if (!fs::exists(dir_path))
                    fs::create_directories(dir_path);
            }
            else
            {
                auto out_path = to / sb.name;
                zf = zip_fopen_index(za, i, 0);
                if (!zf)
                {
                    throw std::runtime_error("Cannot process file.");
                }

                std::fstream outfile(out_path, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
                if (!outfile.is_open())
                {
                    throw std::runtime_error("Connot create file");
                }

                sum = 0;
                while (sum != sb.size)
                {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0)
                    {
                        throw std::runtime_error("Bad file lenght.");
                    }
                    outfile.write(buf, len);
                    outfile.flush();
                    sum += len;
                }
                outfile.close();
                zip_fclose(zf);
            }
        }
        else
        {
        }
    }

    if (zip_close(za) == -1)
    {
        throw std::runtime_error("Unable to close file.");
    }
}