#include <string>
#include <stack>
#include <filesystem>
#define MODE_COMPRESS 0
inline std::string __zip_get_rel_dir(const std::string& root, const std::string& current) {
    return std::filesystem::path(current).lexically_relative(root).string();
}
bool add_dir(const std::string& root, const std::string& relpath) {
    if (0 != MODE_COMPRESS) return false;
    if (!std::filesystem::exists(root)) return false;

    std::stack<std::pair<std::string, std::string>> dirs;
    dirs.push({ root, relpath });

    while (!dirs.empty()) {
        auto [dir_root, dir_relpath] = dirs.top();
        dirs.pop();

        std::filesystem::directory_iterator dir;
        if (dir_relpath.size() != 0)
            dir = std::filesystem::directory_iterator(dir_root + "\\" + dir_relpath);
        else
            dir = std::filesystem::directory_iterator(dir_root);

        for (auto& entry : dir) {
            if (entry.is_regular_file()) {
                std::string __path = entry.path().string();
                //add_file(__path.c_str(), entry.path().filename().string(), dir_relpath);
            }
            else {
                dirs.push({ dir_root, __zip_get_rel_dir(dir_root, entry.path().string()) });
            }
        }
    }
    return true;
}