#pragma once

#include <fstream>
#include <string>

static std::string file_read(std::string path) {
    /// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
    if (path == "") return "";
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (f) {
        std::string contents;
        f.seekg(0, std::ios::end);  // read to the end of the file
        contents.resize(f.tellg()); // get position of the last character (size of file)

        f.seekg(0, std::ios::beg); // go to beginning
        f.read(&contents[0], contents.size());
        f.close(); // read whole file and close

        return contents;
    }
    return "";
}

template <typename II>
static std::string join(II b, II e, std::string delim) {
    std::stringstream s;
    bool first = true;
    for (auto it = b; it != e; ++it) {
        if (first) { first = false; }
        else { s << delim; }
        s << *it;
    }
    return s.str();
}

