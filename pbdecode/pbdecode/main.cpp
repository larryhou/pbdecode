//
//  main.cpp
//  pbdecode
//
//  Created by larryhou on 2019/12/7.
//  Copyright © 2019 larryhou. All rights reserved.
//

#include <assert.h>
#include <iostream>
#include <fstream>

void decode(const char *input, char *output, size_t length, bool newline = true)
{
    auto wCursor = output;
    
    auto rCursor = input;
    auto end = rCursor + length;
    while (rCursor < end)
    {
        *wCursor = *rCursor;
        if (*rCursor == '\\')
        {
            auto ptr = rCursor + 1;
            if (*ptr >= '0' && *ptr <= '9')
            {
                auto byte = 0;
                for (auto n = 0; n < 3; n++)
                {
                    byte = (byte << 3) | *ptr - '0';
                    ++ptr;
                }
                
                assert(byte <= 0xFF);
                *wCursor = static_cast<char>(byte);
                rCursor += 3;
            }
        }
        
        ++rCursor;
        ++wCursor;
    }
    
    *wCursor = 0;
    std::cout << output;
    if (newline) { std::cout << std::endl; }
    std::cout << std::flush;
}

void decode(const char *path)
{
    std::ifstream fs;
    fs.open(path, std::ifstream::binary);
    fs.seekg(0, std::ios_base::end);
    auto length = static_cast<size_t>(fs.tellg());
    fs.seekg(0);
    
    char buffer[length];
    fs.read(buffer, length);
    fs.close();
    
    decode(buffer, buffer, length, false);
}

int main(int argc, const char * argv[])
{
    if (argc > 1)
    {
        for (auto i = 1; i < argc; i++)
        {
            decode(argv[i]);
        }
    }
    else
    {
        std::string pipe;
        while (std::getline(std::cin, pipe))
        {
            auto size = pipe.size();
            
            char buffer[size];
            decode(pipe.c_str(), buffer, size);
        }
    }
    
    return 0;
}
