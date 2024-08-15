#include <array>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <iostream>

#include "../include/FastAES.hpp"

void print_hex(const uint8_t* data, std::size_t length)
{
    for (std::size_t i = 0; i < length; ++i)
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(data[i]);
    std::cout << std::dec << std::endl;
}

uint8_t* hex_string_to_binary(const char* hex_str, size_t& out_size)
{
    size_t hex_len = std::strlen(hex_str);
    if (hex_len % 2 != 0)
    {
        std::cerr << "Error: Invalid input message for decryption.\n";
        exit(EXIT_FAILURE);
    }

    out_size = hex_len / 2;
    uint8_t* binary_data = new(std::nothrow) uint8_t[out_size];
    if (binary_data == nullptr)
    {
        std::cerr << "Error: Memory allocation failed while converting input message for decyption. Ensure sufficient memory is available and try again.\n";
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < out_size; ++i)
    {
        char high_nibble = hex_str[2 * i];
        char low_nibble = hex_str[2 * i + 1];

        if (high_nibble >= '0' && high_nibble <= '9')
            binary_data[i] = (high_nibble - '0') << 4;
        else if (high_nibble >= 'A' && high_nibble <= 'F')
            binary_data[i] = (high_nibble - 'A' + 10) << 4;
        else if (high_nibble >= 'a' && high_nibble <= 'f')
            binary_data[i] = (high_nibble - 'a' + 10) << 4;
        else
        {
            std::cerr << "Error: Invalid input message for decryption.\n";
            exit(EXIT_FAILURE);
        }

        if (low_nibble >= '0' && low_nibble <= '9')
            binary_data[i] |= (low_nibble - '0');
        else if (low_nibble >= 'A' && low_nibble <= 'F')
            binary_data[i] |= (low_nibble - 'A' + 10);
        else if (low_nibble >= 'a' && low_nibble <= 'f')
            binary_data[i] |= (low_nibble - 'a' + 10);
        else
        {
            std::cerr << "Error: Invalid input message for decryption.\n";
            exit(EXIT_FAILURE);
        }
    }

    return binary_data;
}

void print_help() 
{
    std::cout << "Usage: sm-aes.exe [options]\n"
              << "Options:\n"
              << "  -enc                  Encrypt the input\n"
              << "  -dec                  Decrypt the input\n"
              << "  -key <key>            Specify the encryption/decryption key (keys larger than 16 bytes will be truncated and smaller will be space padded)\n"
              << "  -msg <text>           Specify the plaintext message to encrypt (output could contain spaced extra bytes corresponding to Spaced padding bytes)\n"
              << "  -in <file>            Specify the input file path\n"
              << "  -out <file>           Specify the output file path (output could contain extra bytes corresponding to PKCS5 padding bytes)\n"
              << "  -thd <thread number>  Specify the number of threads to use for encryption/decryption. Defaults to the number of available CPU cores.\n"
              << "  -h                    Display this help message and exit\n"

              << "\nExamples:\n"
              << "  Encrypt a plaintext message and and display it:\n"
              << "    sm-aes.exe -enc -key mysecretkey123456 -msg \"Hello, World!\"\n"
              << "  Decrypt a cypher message with the same key and display it:\n"
              << "    sm-aes.exe -dec -key mysecretkey123456 -msg c080664469a3770e8424cdd0e6bb9e21\n"
              << "  Encrypt a file with a key and 4 threads:\n"
              << "    sm-aes.exe -enc -key mysecretkey123456 -in input.txt -out encrypted.bin -thd 4\n"
              << "  Decrypt a file with the same key and save the output to a text file:\n"
              << "    sm-aes.exe -dec -key mysecretkey123456 -in encrypted.bin -out decrypted.txt\n";
}

int main(int argc, char* argv[]) 
{
    if (argc == 1)
        print_help();

    // args parsing
    int enc{0}, dec{0}, msg{0}, in{0}, out{0}, key{0}, thd{0}, num_threads{0};
    for (int i = 1; i < argc; ++i)
    {
        if (!std::strcmp(argv[i], "-h")) 
        {
            if (argc > 2 or i != 1) 
            {
                std::cerr << "Error: The -h (help) option cannot be used in combination with other arguments.\n";
                return EXIT_FAILURE;
            }

            print_help();
            return EXIT_SUCCESS;
        }
        else if (!std::strcmp(argv[i], "-enc"))
            enc = 1;
        else if (!std::strcmp(argv[i], "-dec"))
            dec = 1;
        else if (!std::strcmp(argv[i], "-key"))
            key = ++i; // key position in arg-array
        else if (!std::strcmp(argv[i], "-msg"))
            msg = ++i; // message position
        else if (!std::strcmp(argv[i], "-in"))
            in = ++i; // input file path position in arg-array
        else if (!std::strcmp(argv[i], "-out"))
            out = ++i; // output file path position in arg-array
        else if (!std::strcmp(argv[i], "-thd"))
            thd = ++i; // thread number position in arg-array
        else
        {
            std::cerr << "Error: Unknow option " << argv[i] << "\n";
            return EXIT_FAILURE;
        }
    }

    if (enc == dec)
    {
        std::cerr << "Error: You must specify either -enc or -dec, and not both.\n";
        return EXIT_FAILURE;
    }
    if (not key or key >= argc)
    {
        std::cerr << "Error: You must specify the -key option followed by a valid key value.\n";
        return EXIT_FAILURE;
    }
    if (msg and (in or out))
    {
        std::cerr << "Error: The -msg option cannot be used in combination with the -in or -out options.\n";
        return EXIT_SUCCESS;
    }
    if (msg and msg >= argc)
    {
        std::cerr << "Error: You must specify the -msg option followed by a valid plaintext value.\n";
        return EXIT_FAILURE;
    }
    if (not(msg or in))
    {
        std::cerr << "Error: You must specify an input using either the -msg or -in option.\n";
        return EXIT_FAILURE;
    }
    if (in and not out)
    {
        std::cerr << "Error: An output file path must be specified using the -out option when -in is used.\n";
        return EXIT_FAILURE;
    }
    if (in and in >= argc)
    {
        std::cerr << "Error: You must specify the -in option followed by a valid input file path.\n";
        return EXIT_FAILURE;
    }
    if (out and not in)
    {
        std::cerr << "Error: An input file path must be specified using the -in option when -out is used.\n";
        return EXIT_FAILURE;
    }
    if (out and out >= argc)
    {
        std::cerr << "Error: You must specify the -out option followed by a valid output file path.\n";
        return EXIT_FAILURE;
    }
    if (thd and thd >= argc)
    {
        std::cerr << "Error: You must specify the -thd option followed by a valid thread number.\n";
        return EXIT_FAILURE;
    }
    if (thd)
    {
        try { 
            num_threads = std::stoi(argv[thd]); 
            if (num_threads < 0)
            {
                std::cerr << "Error: You must specify a valid number thread.\n";
                return EXIT_FAILURE;
            }

        }
        catch(const std::invalid_argument& e) { 
            std::cerr << "Error: You must specify a valid number thread.\n";
            return EXIT_FAILURE;
        }
    }

    if (num_threads == 0)
        num_threads = std::thread::hardware_concurrency();
    alignas(16) uint8_t key_buff[16];
    std::memset(key_buff, ' ', 16);
    std::memcpy(key_buff, argv[key], std::strlen(argv[key]));
    FastAES f_aes(key_buff);
    if (msg)
    {
        if (enc)
        {
            std::size_t msg_len = std::strlen(argv[msg]);
            std::size_t size = ((msg_len / 16) + 1) * 16;
            std::size_t remainder = size - msg_len;
            uint8_t* msg_buffer = new(std::nothrow) uint8_t[size];
            if (msg_buffer == nullptr)
            {
                std::cerr << "Error: Memory allocation failed while padding the input message. Ensure sufficient memory is available and try again.\n";
                return EXIT_FAILURE;
            }
            std::memcpy(msg_buffer, argv[msg], msg_len);
            std::memset(msg_buffer + msg_len, 0, remainder);

            uint8_t* cyphertext = new(std::nothrow) uint8_t[size];
            if (cyphertext == nullptr)
            {
                std::cerr << "Error: Memory allocation failed for the ciphertext buffer. Ensure sufficient memory is available and try again.\n";
                return EXIT_FAILURE;
            }
            
            f_aes.encrypt(msg_buffer, cyphertext, size, num_threads);
            print_hex(cyphertext, size);
            delete[] cyphertext;
            delete[] msg_buffer;
        }

        if (dec)
        {
            std::size_t size{0};
            uint8_t* msg_buffer = hex_string_to_binary(argv[msg], size);
            uint8_t* decrypted_text = new(std::nothrow) uint8_t[size];
            if (decrypted_text == nullptr)
            {
                std::cerr << "Error: Memory allocation failed for the decrypted text buffer. Ensure sufficient memory is available and try again.\n";
                return EXIT_FAILURE;
            }

            f_aes.decrypt(msg_buffer, decrypted_text, size);
            for (int i = 0; i < size; ++i)
                std::cout << static_cast<char>(decrypted_text[i]);
            delete[] decrypted_text;
            delete[] msg_buffer;
        }

        return EXIT_SUCCESS;
    }

    if (in)
    {
        std::ofstream ofs(argv[out], std::ios::binary);
        std::ifstream ifs(argv[in], std::ios::binary);

        if (not ifs.is_open())
        {
            std::cerr << "Error: Cannot open input file at path : " << argv[in] << "\n";
            return EXIT_FAILURE;
        }

        if (not ofs.is_open())
        {
            std::cerr << "Error: Cannot create output file at path : " << argv[out] << "\n";
            return EXIT_FAILURE;
        }

        ifs.seekg(0, std::ios::end);
        std::streamsize fsize = ifs.tellg();
        std::size_t size = ((fsize / 16) + (enc ? 1 : 0)) * 16;
        std::size_t remainder = size - fsize;

        uint8_t* input_buffer = new(std::nothrow) uint8_t[size];
        if (input_buffer == nullptr)
        {
            std::cerr << "Error: Memory allocation failed for the input buffer. Ensure sufficient memory is available and try again.\n";
            return EXIT_FAILURE;
        }

        ifs.seekg(0, std::ios::beg);
        if (not ifs.read(reinterpret_cast<char*>(input_buffer), fsize))
        {
            std::cerr << "Error: Cannot read input file.\n";
            delete[] input_buffer;
            return EXIT_FAILURE;
        }
        std::memset(input_buffer + fsize, remainder, remainder); // PCKS5 padding

        if (enc)
        {
            uint8_t* cyphertext = new(std::nothrow) uint8_t[size];
            if (cyphertext == nullptr)
            {
                std::cerr << "Error: Memory allocation failed for the cyphertext buffer. Ensure sufficient memory is available and try again.\n";
                delete[] input_buffer;
                return EXIT_FAILURE; 
            }

            f_aes.encrypt(input_buffer, cyphertext, size, num_threads);
            if (not ofs.write(reinterpret_cast<char*>(cyphertext), size))
            {
                std::cerr << "Error: Cannot write encrypted buffer to output file.\n";
                delete[] input_buffer;
                delete[] cyphertext;
                return EXIT_FAILURE;   
            }

            delete[] cyphertext;   
        }

        if (dec)
        {
            uint8_t* decrypted_text = new(std::nothrow) uint8_t[size];
            if (decrypted_text == nullptr)
            {
                std::cerr << "Error: Memory allocation failed for the decrypted buffer. Ensure sufficient memory is available and try again.\n";
                delete[] input_buffer;
                return EXIT_FAILURE; 
            }

            f_aes.decrypt(input_buffer, decrypted_text, size);
            int last_byte = decrypted_text[size - 1];
            for (int i = size - last_byte; i >= size; ++i)
            {
                if (decrypted_text[i] != last_byte)
                {
                    std::cerr << "Error : Bad Key provided for decryption.\n";
                    delete[] input_buffer;
                    delete[] decrypted_text;
                    return EXIT_FAILURE;
                }
            }

            if (not ofs.write(reinterpret_cast<char*>(decrypted_text), size - last_byte))
            {
                std::cerr << "Error: Cannot write encrypted buffer to output file.\n";
                delete[] input_buffer;
                delete[] decrypted_text;
                return EXIT_FAILURE;   
            }

            delete[] decrypted_text;
        }

        delete[] input_buffer;
        return EXIT_SUCCESS;
    }
}