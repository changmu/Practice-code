#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define KEY_LEN 256

static void print_hex(const void *p, size_t len)
{
    const unsigned char *str = p;
    int i;

    puts("");
    for (i = 0; i < len; ++i)
        printf("%02x ", str[i]);
    puts("");
}

int main(int argc, char **argv)
{
    if (3 != argc || (strcasecmp("enc", argv[1]) && strcasecmp("dec", argv[1]))) {
        fprintf(stderr, "Usage:\n\t");
        fprintf(stderr, "%s enc/dec filename\n", argv[0]);
        return 2;
    }

    int enc = !strcasecmp("enc", argv[1]);
    char *buf = (char *) malloc(strlen(argv[2]) + 5);
    strcpy(buf, argv[2]);
    if (enc)
        strcat(buf, "_enc");
    else 
        strcat(buf, "_dec");

    FILE *file_in  = fopen(argv[2], "r");
    if (NULL == file_in) {
        fprintf(stderr, "Couldn't open file %s\n", argv[2]);
        return 2;
    }
    FILE *file_out = fopen(buf, "w");
    if (NULL == file_out) {
        fprintf(stderr, "Couldn't open file %s\n", buf);
        return 2;
    }

    unsigned char aes_key[KEY_LEN >> 3];
    memset(aes_key, 0, KEY_LEN >> 3);

    size_t file_len;
    fseek(file_in, 0, SEEK_END);
    file_len = ftell(file_in);
    rewind(file_in);

    unsigned char aes_input[file_len];
    size_t encslength;

    if (enc) {
        fread(aes_input, 1, file_len, file_in);
        // print_hex(aes_input, file_len);
        if (!RAND_bytes(aes_key, KEY_LEN >> 3))
            return 2;
        encslength = ((file_len + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        unsigned char iv_enc[AES_BLOCK_SIZE]; 
        memset(iv_enc, 0, AES_BLOCK_SIZE);
        RAND_bytes(iv_enc, AES_BLOCK_SIZE);
        unsigned char enc_out[encslength + 10];
        memset(enc_out, 0, encslength + 10);
        AES_KEY enc_key;

        AES_set_encrypt_key(aes_key, KEY_LEN, &enc_key);
        AES_cbc_encrypt(aes_input, enc_out, file_len, &enc_key, iv_enc, AES_ENCRYPT);

        fwrite(&file_len, 1, sizeof(file_len), file_out);       // Source file's len
        fwrite(&encslength, 1, sizeof(encslength), file_out);   // Encrypted string's len
        fwrite(iv_enc, 1, AES_BLOCK_SIZE, file_out);            // iv_enc
        print_hex(iv_enc, AES_BLOCK_SIZE);
        fwrite(&enc_key, 1, sizeof(enc_key), file_out);
        fwrite(aes_key, 1, KEY_LEN >> 3, file_out);             // aes_key
        print_hex(aes_key, KEY_LEN >> 3);
        fwrite(enc_out, 1, encslength, file_out);               // Encrypted string
        // print_hex(enc_out, encslength);
    } else {
        fread(&file_len, 1, sizeof(file_len), file_in);
        fread(&encslength, 1, sizeof(encslength), file_in);
        unsigned char iv_dec[AES_BLOCK_SIZE];
        memset(iv_dec, 0, AES_BLOCK_SIZE);
        fread(iv_dec, 1, AES_BLOCK_SIZE, file_in);              // iv_dec == iv_enc
        print_hex(iv_dec, AES_BLOCK_SIZE);
        AES_KEY dec_key;
        unsigned char dec_out[file_len + 10];
        fread(&dec_key, 1, sizeof(dec_key), file_in);
        fread(aes_key, 1, KEY_LEN >> 3, file_in);
        print_hex(aes_key, KEY_LEN >> 3);
        fread(aes_input, 1, encslength, file_in);

        AES_set_decrypt_key(aes_key, KEY_LEN, &dec_key);
        AES_cbc_encrypt(aes_input, dec_out, encslength, &dec_key, iv_dec, AES_DECRYPT);

        fwrite(dec_out, 1, file_len, file_out);
    }

    free(buf);
    fclose(file_in);
    fclose(file_out);
    return 0;
}
