#include <cstdlib>
#include <vector>
#include <string>
#include <openssl/evp.h>
#include <iostream>
#include <stdexcept>
std::vector<uint8_t> load_key(){
    auto *hmc_key = std::getenv("HMAC_KEY");
    if (!hmc_key) {
        throw std::runtime_error("HMAC-KEY no esta definida en el entorno, porfavor mira el archivo .env");
    }
    std::string key_str(hmc_key);


    int len_key = EVP_DECODE_LENGTH(key_str.size());

    std::vector<uint8_t> key_array(len_key);

    int decodes_len = EVP_DecodeBlock(key_array.data(), reinterpret_cast<const unsigned char*> (key_str.data()),key_str.size());
    if (decodes_len < 0) {
        throw std::runtime_error("Error al decodificar la clave HMAC");
    }
    key_array.resize(decodes_len); // Ajustar el tamaño del vector al tamaño real de la clave decodificada
    return key_array;
}
int main() {
    try {
        std::vector<uint8_t> key = load_key();
        std::cout << "Clave HMAC cargada correctamente, tamaño: " << key.size() << " bytes." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
// Este código carga una clave HMAC desde una variable de entorno llamada "HMAC-KEY