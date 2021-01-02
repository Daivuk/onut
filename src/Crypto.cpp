// Onut
#include <onut/Crypto.h>

// STL
#include <string.h>

static bool isCharacter(const char Character)
{
    return ((Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
    //Checks if a Character is a Valid A-Z, a-z Character, based on the ascii value
}

static bool isNumber(const char Character)
{
    return (Character >= '0' && Character <= '9');
    //Checks if a Character is a Valid 0-9 Number, based on the ascii value
}

static bool isValidEmailAddress(const char * EmailAddress)
{
    if (!EmailAddress) // If cannot read the Email Address...
        return 0;
    if (!isCharacter(EmailAddress[0])) // If the First character is not A-Z, a-z
        return 0;
    int AtOffset = -1;
    int DotOffset = -1;
    size_t Length = strlen(EmailAddress); // Length = StringLength (strlen) of EmailAddress
    for (size_t i = 0; i < Length; i++)
    {
        if (EmailAddress[i] == '@') // If one of the characters is @, store it's position in AtOffset
            AtOffset = (int)i;
        else if (EmailAddress[i] == '.') // Same, but with the dot
            DotOffset = (int)i;
    }
    if (AtOffset == -1 || DotOffset == -1) // If cannot find a Dot or a @
        return 0;
    if (AtOffset > DotOffset) // If the @ is after the Dot
        return 0;
    return !(DotOffset >= ((int)Length - 1)); //Chech there is some other letters after the Dot
}


#ifndef __MD5_H__
#define __MD5_H__

/*
 * Size of a standard MD5 signature in bytes.  This definition is for
 * external programs only.  The MD5 routines themselves reference the
 * signature as 4 unsigned 32-bit integers.
 */
const unsigned int MD5_SIZE = (4 * sizeof(unsigned int));   /* 16 */
const unsigned int MD5_STRING_SIZE = 2 * MD5_SIZE + 1;      /* 33 */

 namespace md5 {
    /*
     * The MD5 algorithm works on blocks of characters of 64 bytes.  This
     * is an internal value only and is not necessary for external use.
     */
    const unsigned int BLOCK_SIZE = 64;

    class md5_t {
        public:
            /*
             * md5_t
             *
             * DESCRIPTION:
             *
             * Initialize structure containing state of MD5 computation. (RFC 1321,
             * 3.3: Step 3).  This is for progressive MD5 calculations only.  If
             * you have the complete string available, call it as below.
             * process should be called for each bunch of bytes and after the last
             * process call, finish should be called to get the signature.
             *
             * RETURNS:
             *
             * None.
             *
             * ARGUMENTS:
             *
             * None.
             */
            md5_t();

            /*
             * md5_t
             *
             * DESCRIPTION:
             *
             * This function is used to calculate a MD5 signature for a buffer of
             * bytes.  If you only have part of a buffer that you want to process
             * then md5_t, process, and finish should be used.
             *
             * RETURNS:
             *
             * None.
             *
             * ARGUMENTS:
             *
             * input - A buffer of bytes whose MD5 signature we are calculating.
             *
             * input_length - The length of the buffer.
             *
             * signature_ - A 16 byte buffer that will contain the MD5 signature.
             */
            md5_t(const void* input, const unsigned int input_length, void* signature_ = NULL);

            /*
             * process
             *
             * DESCRIPTION:
             *
             * This function is used to progressively calculate an MD5 signature some
             * number of bytes at a time.
             *
             * RETURNS:
             *
             * None.
             *
             * ARGUMENTS:
             *
             * input - A buffer of bytes whose MD5 signature we are calculating.
             *
             * input_length - The length of the buffer.
             */
            void process(const void* input, const unsigned int input_length);

            /*
             * finish
             *
             * DESCRIPTION:
             *
             * Finish a progressing MD5 calculation and copy the resulting MD5
             * signature into the result buffer which should be 16 bytes
             * (MD5_SIZE).  After this call, the MD5 structure cannot be used
             * to calculate a new md5, it can only return its signature.
             *
             * RETURNS:
             *
             * None.
             *
             * ARGUMENTS:
             *
             * signature_ - A 16 byte buffer that will contain the MD5 signature.
             */
            void finish(void* signature_ = NULL);

            /*
             * get_sig
             *
             * DESCRIPTION:
             *
             * Retrieves the previously calculated signature from the MD5 object.
             *
             * RETURNS:
             *
             * None.
             *
             * ARGUMENTS:
             *
             * signature_ - A 16 byte buffer that will contain the MD5 signature.
             */
            void get_sig(void* signature_);

            /*
             * get_string
             *
             * DESCRIPTION:
             *
             * Retrieves the previously calculated signature from the MD5 object in
             * printable format.
             *
             * RETURNS:
             *
             * None.
             *
             * ARGUMENTS:
             *
             * str_ - a string of characters which should be at least 33 bytes long
             * (2 characters per MD5 byte and 1 for the \0).
             */
            void get_string(void* str_);

        private:
            /* internal functions */
            void initialise();
            void process_block(const unsigned char*);
            void get_result(void*);

            unsigned int A;                             /* accumulator 1 */
            unsigned int B;                             /* accumulator 2 */
            unsigned int C;                             /* accumulator 3 */
            unsigned int D;                             /* accumulator 4 */

            unsigned int message_length[2];             /* length of data */
            unsigned int stored_size;                   /* length of stored bytes */
            unsigned char stored[md5::BLOCK_SIZE * 2];  /* stored bytes */

            bool finished;                              /* object state */

            char signature[MD5_SIZE];                   /* stored signature */
            char str[MD5_STRING_SIZE];                  /* stored plain text hash */
    };

    /*
     * sig_to_string
     *
     * DESCRIPTION:
     *
     * Convert a MD5 signature in a 16 byte buffer into a hexadecimal string
     * representation.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * signature - a 16 byte buffer that contains the MD5 signature.
     *
     * str - a string of characters which should be at least 33 bytes long (2
     * characters per MD5 byte and 1 for the \0).
     *
     * str_len - the length of the string.
     */
    extern void sig_to_string(const void* signature, char* str, const int str_len);

    /*
     * sig_from_string
     *
     * DESCRIPTION:
     *
     * Convert a MD5 signature from a hexadecimal string representation into
     * a 16 byte buffer.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * signature - A 16 byte buffer that will contain the MD5 signature.
     *
     * str - A string of charactes which _must_ be at least 32 bytes long (2
     * characters per MD5 byte).
     */
    extern void sig_from_string(void* signature, const char* str);
} // namespace md5

#endif /* ! __MD5_H__ */

/*
 * Local defines for the md5 functions.
 *
 * $Id: md5_loc.h,v 1.5 2010-05-07 13:58:18 gray Exp $
 */

#ifndef __MD5_LOC_H__
#define __MD5_LOC_H__

/*
 * We don't include "conf.h" here because it gets included before this file in md5.cpp so the defines
 * are correctly determing before they are checked.
 */
 #if MD5_DEBUG
    #include <iostream>
#endif // MD5_DEBUG

/// For now we are assuming everything is in little endian byte-order

namespace md5 {
    /*
     * T denotes the integer part of the i-th element of the function:
     * T[i] = 4294967296 * abs(sin(i)), where i is in radians.
     */
    const unsigned int T[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    /*
     * Constants for the MD5 Transform routine as defined in RFC 1321
     */
    const unsigned int S1[4] = {7, 12, 17, 22};
    const unsigned int S2[4] = {5, 9,  14, 20};
    const unsigned int S3[4] = {4, 11, 16, 23};
    const unsigned int S4[4] = {6, 10, 15, 21};

    /*
     * Function to perform the cyclic left rotation of blocks of data
     */
    inline unsigned int cyclic_left_rotate(unsigned int data, unsigned int shift_bits) {
        return (data << shift_bits) | (data >> (32 - shift_bits));
    }

    inline unsigned int F(unsigned int x, unsigned int y, unsigned int z) {return (x & y) | (~x & z);};
    inline unsigned int G(unsigned int x, unsigned int y, unsigned int z) {return (x & z) | (y & ~z);};
    inline unsigned int H(unsigned int x, unsigned int y, unsigned int z) {return x ^ y ^ z;};
    inline unsigned int I(unsigned int x, unsigned int y, unsigned int z) {return y ^ (x | ~z);};

    inline void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Xk, unsigned int s, unsigned int i) {
        #if MD5_DEBUG
            std::cout << "\nA: " << a << "\nB: " << b << "\nC: " << c << "\nD: " << d << "\nX[" << i << "]: " << Xk << "\ns: " << S1[s] << "\nT: " << T[i] << "\n";
        #endif

        a += F(b,c,d) + Xk + T[i];
        a = cyclic_left_rotate(a, S1[s]);
        a += b;

        #if MD5_DEBUG
            std::cout << "A = " << a << "\n";
        #endif
    };

    inline void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Xk, unsigned int s, unsigned int i) {
        #if MD5_DEBUG
            std::cout << "\nA: " << a << "\nB: " << b << "\nC: " << c << "\nD: " << d << "\nX[" << i - 16 << "]: " << Xk << "\ns: " << S2[s] << "\nT: " << T[i] << "\n";
        #endif // MD5_DEBUG

        a += G(b,c,d) + Xk + T[i];
        a = cyclic_left_rotate(a, S2[s]);
        a += b;

        #if MD5_DEBUG
            std::cout << "A = " << a << "\n";
        #endif // MD5_DEBUG
    };

    inline void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Xk, unsigned int s, unsigned int i) {
        #if MD5_DEBUG
            std::cout << "\nA: " << a << "\nB: " << b << "\nC: " << c << "\nD: " << d << "\nX[" << i - 32 << "]: " << Xk << "\ns: " << S3[s] << "\nT: " << T[i] << "\n";
        #endif // MD5_DEBUG

        a += H(b,c,d) + Xk + T[i];
        a = cyclic_left_rotate(a, S3[s]);
        a += b;

        #if MD5_DEBUG
            std::cout << "A = " << a << "\n";
        #endif // MD5_DEBUG
    };
    inline void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Xk, unsigned int s, unsigned int i) {
        #if MD5_DEBUG
            std::cout << "\nA: " << a << "\nB: " << b << "\nC: " << c << "\nD: " << d << "\nX[" << i - 48 << "]: " << Xk << "\ns: " << S4[s] << "\nT: " << T[i] << "\n";
        #endif // MD5_DEBUG

        a += I(b,c,d) + Xk + T[i];
        a = cyclic_left_rotate(a, S4[s]);
        a += b;

        #if MD5_DEBUG
            std::cout << "A = " << a << "\n";
        #endif // MD5_DEBUG
    };

    /*
     * Define my endian-ness.  Could not do in a portable manner using the
     * include files -- grumble.
     */
    #if MD5_BIG_ENDIAN

    /*
     * big endian - big is better
     */
    #define MD5_SWAP(n) (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))

    #else

    /*
     * little endian
     */
    #define MD5_SWAP(n) (n)

    #endif // MD5_BIG_ENDIAN

    const char* HEX_STRING = "0123456789abcdef";    /* to convert to hex */
}

#endif /* ! __MD5_LOC_H__ */

#include <cassert>
#include <cstring>
#include <iostream>

namespace md5 {
    /****************************** Public Functions ******************************/

    /*
     * md5_t
     *
     * DESCRIPTION:
     *
     * Initialize structure containing state of MD5 computation. (RFC 1321,
     * 3.3: Step 3).  This is for progressive MD5 calculations only.  If
     * you have the complete string available, call it as below.
     * process should be called for each bunch of bytes and after the
     * last process call, finish should be called to get the signature.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * None.
     */
    md5_t::md5_t() {
        initialise();
    }

    /*
     * md5_t
     *
     * DESCRIPTION:
     *
     * This function is used to calculate a MD5 signature for a buffer of
     * bytes.  If you only have part of a buffer that you want to process
     * then md5_t, process, and finish should be used.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * buffer - A buffer of bytes whose MD5 signature we are calculating.
     *
     * input_length - The length of the buffer.
     *
     * signature - A 16 byte buffer that will contain the MD5 signature.
     */
    md5_t::md5_t(const void* input, const unsigned int input_length, void* signature) {
        /* initialize the computation context */
        initialise();

        /* process whole buffer but last input_length % MD5_BLOCK bytes */
        process(input, input_length);

        /* put result in desired memory area */
        finish(signature);
    }

    /*
     * process
     *
     * DESCRIPTION:
     *
     * This function is used to progressively calculate a MD5 signature some
     * number of bytes at a time.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * buffer - A buffer of bytes whose MD5 signature we are calculating.
     *
     * input_length - The length of the buffer.
     */
    void md5_t::process(const void* input, const unsigned int input_length) {
        if (!finished) {
            unsigned int processed = 0;

            /*
             * If we have any data stored from a previous call to process then we use these
             * bytes first, and the new data is large enough to create a complete block then
             * we process these bytes first.
             */
            if (stored_size && input_length + stored_size >= md5::BLOCK_SIZE) {
                unsigned char block[md5::BLOCK_SIZE];
                memcpy(block, stored, stored_size);
                memcpy(block + stored_size, input, md5::BLOCK_SIZE - stored_size);
                processed = md5::BLOCK_SIZE - stored_size;
                stored_size = 0;
                process_block(block);
            }

            /*
             * While there is enough data to create a complete block, process it.
             */
            while (processed + md5::BLOCK_SIZE <= input_length) {
                process_block((unsigned char*)input + processed);
                processed += md5::BLOCK_SIZE;
            }

            /*
             * If there are any unprocessed bytes left over that do not create a complete block
             * then we store these bytes for processing next time.
             */
            if (processed != input_length) {
                memcpy(stored + stored_size, (char*)input + processed, input_length - processed);
                stored_size += input_length - processed;
            } else {
                stored_size = 0;
            }
        } else {
            // throw error when trying to process after completion?
        }
    }

    /*
     * finish
     *
     * DESCRIPTION:
     *
     * Finish a progressing MD5 calculation and copy the resulting MD5
     * signature into the result buffer which should be 16 bytes
     * (MD5_SIZE).  After this call, the MD5 structure cannot process
	 * additional bytes.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * signature - A 16 byte buffer that will contain the MD5 signature.
     */
    void md5_t::finish(void* signature_) {
        if (!finished) {
            if (message_length[0] + stored_size < message_length[0])
                message_length[1]++;
            message_length[0] += stored_size;

            int pad = md5::BLOCK_SIZE - (sizeof(unsigned int) * 2) - stored_size;
            if (pad <= 0)
                pad += md5::BLOCK_SIZE;

            /*
             * Modified from a fixed array to this assignment and memset to be
             * more flexible with block-sizes -- Gray 10/97.
             */
            if (pad > 0) {
                stored[stored_size] = 0x80;
                if (pad > 1)
                    memset(stored + stored_size + 1, 0, pad - 1);
                stored_size += pad;
            }

            /*
             * Put the 64-bit file length in _bits_ (i.e. *8) at the end of the
             * buffer. appears to be in beg-endian format in the buffer?
             */
            unsigned int size_low = ((message_length[0] & 0x1FFFFFFF) << 3);
            memcpy(stored + stored_size, &size_low, sizeof(unsigned int));
            stored_size += sizeof(unsigned int);

            /* shift the high word over by 3 and add in the top 3 bits from the low */
            unsigned int size_high = (message_length[1] << 3) | ((message_length[0] & 0xE0000000) >> 29);
            memcpy(stored + stored_size, &size_high, sizeof(unsigned int));
            stored_size += sizeof(unsigned int);

            /*
             * process the last block of data.
             * if the length of the message was already exactly sized, then we have
             * 2 messages to process
             */
            process_block(stored);
            if (stored_size > md5::BLOCK_SIZE)
                process_block(stored + md5::BLOCK_SIZE);

            /* Arrange the results into a signature */
            get_result(static_cast<void*>(signature));

            /* store the signature into a readable sring */
            sig_to_string(signature, str, MD5_STRING_SIZE);

            if (signature_ != NULL) {
                memcpy(signature_, static_cast<void*>(signature), MD5_SIZE);
            }

            finished = true;
        } else {
            // add error?
        }
    }

    /*
     * get_sig
     *
     * DESCRIPTION:
     *
     * Retrieves the previously calculated signature from the MD5 object.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * signature_ - A 16 byte buffer that will contain the MD5 signature.
     */
    void md5_t::get_sig(void* signature_) {
        if (finished) {
            memcpy(signature_, signature, MD5_SIZE);
        } else {
            //error?
        }
    }

    /*
     * get_string
     *
     * DESCRIPTION:
     *
     * Retrieves the previously calculated signature from the MD5 object in
     * printable format.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * str_ - a string of characters which should be at least 33 bytes long
     * (2 characters per MD5 byte and 1 for the \0).
     */
    void md5_t::get_string(void* str_) {
        if (finished) {
            memcpy(str_, str, MD5_STRING_SIZE);
        } else {
            // error?
        }
    }

    /****************************** Private Functions ******************************/

    /*
     * initialise
     *
     * DESCRIPTION:
     *
     * Initialize structure containing state of MD5 computation. (RFC 1321,
     * 3.3: Step 3).
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * None.
     */
    void md5_t::initialise() {
        /*
         * ensures that unsigned int is 4 bytes on this platform, will need modifying
         * if we are to use on a different sized platform.
         */
        assert(MD5_SIZE == 16);

        A = 0x67452301;
        B = 0xefcdab89;
        C = 0x98badcfe;
        D = 0x10325476;

        message_length[0] = 0;
        message_length[1] = 0;
        stored_size = 0;

        finished = false;
    }

    /*
     * process_block
     *
     * DESCRIPTION:
     *
     * Process a block of bytes into a MD5 state structure.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * buffer - A buffer of bytes whose MD5 signature we are calculating.
     *
     * input_length - The length of the buffer.
     */
    void md5_t::process_block(const unsigned char* block) {
    /* Process each 16-word block. */

        /*
         * we check for when the lower word rolls over, and increment the
         * higher word. we do not need to worry if the higher word rolls over
         * as only the two words we maintain are needed in the function later
         */
        if (message_length[0] + md5::BLOCK_SIZE < message_length[0])
            message_length[1]++;
        message_length[0] += BLOCK_SIZE;

        // Copy the block into X. */
        unsigned int X[16];
        for (unsigned int i = 0; i < 16; i++) {
            memcpy(X + i, block + 4 * i, 4);
        }

        /* Save A as AA, B as BB, C as CC, and D as DD. */
        unsigned int AA = A, BB = B, CC = C, DD = D;

        /* Round 1
         * Let [abcd k s i] denote the operation
         * a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s)
         * Do the following 16 operations
         * [ABCD  0  7  1]  [DABC  1 12  2]  [CDAB  2 17  3]  [BCDA  3 22  4]
         * [ABCD  4  7  5]  [DABC  5 12  6]  [CDAB  6 17  7]  [BCDA  7 22  8]
         * [ABCD  8  7  9]  [DABC  9 12 10]  [CDAB 10 17 11]  [BCDA 11 22 12]
         * [ABCD 12  7 13]  [DABC 13 12 14]  [CDAB 14 17 15]  [BCDA 15 22 16]
         */
        md5::FF(A, B, C, D, X[0 ], 0, 0 );
        md5::FF(D, A, B, C, X[1 ], 1, 1 );
        md5::FF(C, D, A, B, X[2 ], 2, 2 );
        md5::FF(B, C, D, A, X[3 ], 3, 3 );
        md5::FF(A, B, C, D, X[4 ], 0, 4 );
        md5::FF(D, A, B, C, X[5 ], 1, 5 );
        md5::FF(C, D, A, B, X[6 ], 2, 6 );
        md5::FF(B, C, D, A, X[7 ], 3, 7 );
        md5::FF(A, B, C, D, X[8 ], 0, 8 );
        md5::FF(D, A, B, C, X[9 ], 1, 9 );
        md5::FF(C, D, A, B, X[10], 2, 10);
        md5::FF(B, C, D, A, X[11], 3, 11);
        md5::FF(A, B, C, D, X[12], 0, 12);
        md5::FF(D, A, B, C, X[13], 1, 13);
        md5::FF(C, D, A, B, X[14], 2, 14);
        md5::FF(B, C, D, A, X[15], 3, 15);

        /* Round 2
         * Let [abcd k s i] denote the operation
         * a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s)
         * Do the following 16 operations
         * [ABCD  1  5 17]  [DABC  6  9 18]  [CDAB 11 14 19]  [BCDA  0 20 20]
         * [ABCD  5  5 21]  [DABC 10  9 22]  [CDAB 15 14 23]  [BCDA  4 20 24]
         * [ABCD  9  5 25]  [DABC 14  9 26]  [CDAB  3 14 27]  [BCDA  8 20 28]
         * [ABCD 13  5 29]  [DABC  2  9 30]  [CDAB  7 14 31]  [BCDA 12 20 32]
         */
        md5::GG(A, B, C, D, X[1 ], 0, 16);
        md5::GG(D, A, B, C, X[6 ], 1, 17);
        md5::GG(C, D, A, B, X[11], 2, 18);
        md5::GG(B, C, D, A, X[0 ], 3, 19);
        md5::GG(A, B, C, D, X[5 ], 0, 20);
        md5::GG(D, A, B, C, X[10], 1, 21);
        md5::GG(C, D, A, B, X[15], 2, 22);
        md5::GG(B, C, D, A, X[4 ], 3, 23);
        md5::GG(A, B, C, D, X[9 ], 0, 24);
        md5::GG(D, A, B, C, X[14], 1, 25);
        md5::GG(C, D, A, B, X[3 ], 2, 26);
        md5::GG(B, C, D, A, X[8 ], 3, 27);
        md5::GG(A, B, C, D, X[13], 0, 28);
        md5::GG(D, A, B, C, X[2 ], 1, 29);
        md5::GG(C, D, A, B, X[7 ], 2, 30);
        md5::GG(B, C, D, A, X[12], 3, 31);

        /* Round 3
         * Let [abcd k s i] denote the operation
         * a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s)
         * Do the following 16 operations
         * [ABCD  5  4 33]  [DABC  8 11 34]  [CDAB 11 16 35]  [BCDA 14 23 36]
         * [ABCD  1  4 37]  [DABC  4 11 38]  [CDAB  7 16 39]  [BCDA 10 23 40]
         * [ABCD 13  4 41]  [DABC  0 11 42]  [CDAB  3 16 43]  [BCDA  6 23 44]
         * [ABCD  9  4 45]  [DABC 12 11 46]  [CDAB 15 16 47]  [BCDA  2 23 48]
         */
        md5::HH(A, B, C, D, X[5 ], 0, 32);
        md5::HH(D, A, B, C, X[8 ], 1, 33);
        md5::HH(C, D, A, B, X[11], 2, 34);
        md5::HH(B, C, D, A, X[14], 3, 35);
        md5::HH(A, B, C, D, X[1 ], 0, 36);
        md5::HH(D, A, B, C, X[4 ], 1, 37);
        md5::HH(C, D, A, B, X[7 ], 2, 38);
        md5::HH(B, C, D, A, X[10], 3, 39);
        md5::HH(A, B, C, D, X[13], 0, 40);
        md5::HH(D, A, B, C, X[0 ], 1, 41);
        md5::HH(C, D, A, B, X[3 ], 2, 42);
        md5::HH(B, C, D, A, X[6 ], 3, 43);
        md5::HH(A, B, C, D, X[9 ], 0, 44);
        md5::HH(D, A, B, C, X[12], 1, 45);
        md5::HH(C, D, A, B, X[15], 2, 46);
        md5::HH(B, C, D, A, X[2 ], 3, 47);

        /* Round 4
         * Let [abcd k s i] denote the operation
         * a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s)
         * Do the following 16 operations
         * [ABCD  0  6 49]  [DABC  7 10 50]  [CDAB 14 15 51]  [BCDA  5 21 52]
         * [ABCD 12  6 53]  [DABC  3 10 54]  [CDAB 10 15 55]  [BCDA  1 21 56]
         * [ABCD  8  6 57]  [DABC 15 10 58]  [CDAB  6 15 59]  [BCDA 13 21 60]
         * [ABCD  4  6 61]  [DABC 11 10 62]  [CDAB  2 15 63]  [BCDA  9 21 64]
         */
        md5::II(A, B, C, D, X[0 ], 0, 48);
        md5::II(D, A, B, C, X[7 ], 1, 49);
        md5::II(C, D, A, B, X[14], 2, 50);
        md5::II(B, C, D, A, X[5 ], 3, 51);
        md5::II(A, B, C, D, X[12], 0, 52);
        md5::II(D, A, B, C, X[3 ], 1, 53);
        md5::II(C, D, A, B, X[10], 2, 54);
        md5::II(B, C, D, A, X[1 ], 3, 55);
        md5::II(A, B, C, D, X[8 ], 0, 56);
        md5::II(D, A, B, C, X[15], 1, 57);
        md5::II(C, D, A, B, X[6 ], 2, 58);
        md5::II(B, C, D, A, X[13], 3, 59);
        md5::II(A, B, C, D, X[4 ], 0, 60);
        md5::II(D, A, B, C, X[11], 1, 61);
        md5::II(C, D, A, B, X[2 ], 2, 62);
        md5::II(B, C, D, A, X[9 ], 3, 63);

        /* Then perform the following additions. (That is increment each
        of the four registers by the value it had before this block
        was started.) */
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    /*
     * get_result
     *
     * DESCRIPTION:
     *
     * Copy the resulting MD5 signature into the first 16 bytes (MD5_SIZE)
     * of the result buffer.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * result - A 16 byte buffer that will contain the MD5 signature.
     */
    void md5_t::get_result(void *result) {
        memcpy((char*)result, &A, sizeof(unsigned int));
        memcpy((char*)result + sizeof(unsigned int), &B, sizeof(unsigned int));
        memcpy((char*)result + 2 * sizeof(unsigned int), &C, sizeof(unsigned int));
        memcpy((char*)result + 3 * sizeof(unsigned int), &D, sizeof(unsigned int));
    }

    /****************************** Exported Functions ******************************/

    /*
     * sig_to_string
     *
     * DESCRIPTION:
     *
     * Convert a MD5 signature in a 16 byte buffer into a hexadecimal string
     * representation.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * signature_ - a 16 byte buffer that contains the MD5 signature.
     *
     * str_ - a string of charactes which should be at least 33 bytes long (2
     * characters per MD5 byte and 1 for the \0).
     *
     * str_len - the length of the string.
     */
    void sig_to_string(const void* signature_, char* str_, const int str_len) {
        unsigned char* sig_p;
        char* str_p;
        char* max_p;
        unsigned int high, low;

        str_p = str_;
        max_p = str_ + str_len;

        for (sig_p = (unsigned char*)signature_; sig_p < (unsigned char*)signature_ + MD5_SIZE; sig_p++) {
            high = *sig_p / 16;
            low = *sig_p % 16;
            /* account for 2 chars */
            if (str_p + 1 >= max_p) {
                break;
            }
            *str_p++ = md5::HEX_STRING[high];
            *str_p++ = md5::HEX_STRING[low];
        }
        /* account for 2 chars */
        if (str_p < max_p) {
            *str_p++ = '\0';
        }
    }

    /*
     * sig_from_string
     *
     * DESCRIPTION:
     *
     * Convert a MD5 signature from a hexadecimal string representation into
     * a 16 byte buffer.
     *
     * RETURNS:
     *
     * None.
     *
     * ARGUMENTS:
     *
     * signature_ - A 16 byte buffer that will contain the MD5 signature.
     *
     * str_ - A string of charactes which _must_ be at least 32 bytes long (2
     * characters per MD5 byte).
     */
    void sig_from_string(void* signature_, const char* str_) {
        unsigned char *sig_p;
        const char *str_p;
        char* hex;
        unsigned int high, low, val;

        hex = (char*)md5::HEX_STRING;
        sig_p = static_cast<unsigned char*>(signature_);

        for (str_p = str_; str_p < str_ + MD5_SIZE * 2; str_p += 2) {
            high = strchr(hex, *str_p) - hex;
            low = strchr(hex, *(str_p + 1)) - hex;
            val = high * 16 + low;
            *sig_p++ = val;
        }
    }
} // namespace md5


/*
Copyright (c) 2011, Micael Hildenborg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of Micael Hildenborg nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Micael Hildenborg ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Micael Hildenborg BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SHA1_DEFINED
#define SHA1_DEFINED

namespace sha1
{

    /**
    @param src points to any kind of data to be hashed.
    @param bytelength the number of bytes to hash from the src pointer.
    @param hash should point to a buffer of at least 20 bytes of size for storing the sha1 result in.
    */
    static void calc(const void* src, const int bytelength, unsigned char* hash);

    /**
    @param hash is 20 bytes of sha1 hash. This is the same data that is the result from the calc function.
    @param hexstring should point to a buffer of at least 41 bytes of size for storing the hexadecimal representation of the hash. A zero will be written at position 40, so the buffer will be a valid zero ended string.
    */
    static void toHexString(const unsigned char* hash, char* hexstring);

} // namespace sha1

#endif // SHA1_DEFINED

/*
Copyright (c) 2011, Micael Hildenborg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of Micael Hildenborg nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Micael Hildenborg ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Micael Hildenborg BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
Contributors:
Gustav
Several members in the gamedev.se forum.
Gregory Petrosyan
*/

namespace sha1
{
    namespace // local
    {
        // Rotate an integer value to left.
        inline const unsigned int rol(const unsigned int value,
                                      const unsigned int steps)
        {
            return ((value << steps) | (value >> (32 - steps)));
        }

        // Sets the first 16 integers in the buffert to zero.
        // Used for clearing the W buffert.
        inline void clearWBuffert(unsigned int* buffert)
        {
            for (int pos = 16; --pos >= 0;)
            {
                buffert[pos] = 0;
            }
        }

        void innerHash(unsigned int* result, unsigned int* w)
        {
            unsigned int a = result[0];
            unsigned int b = result[1];
            unsigned int c = result[2];
            unsigned int d = result[3];
            unsigned int e = result[4];

            int round = 0;

#define sha1macro(func,val) \
                                    { \
                const unsigned int t = rol(a, 5) + (func) + e + val + w[round]; \
                                e = d; \
                                d = c; \
                                c = rol(b, 30); \
                                b = a; \
                                a = t; \
                                    }

            while (round < 16)
            {
                sha1macro((b & c) | (~b & d), 0x5a827999)
                    ++round;
            }
            while (round < 20)
            {
                w[round] = rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
                sha1macro((b & c) | (~b & d), 0x5a827999)
                    ++round;
            }
            while (round < 40)
            {
                w[round] = rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
                sha1macro(b ^ c ^ d, 0x6ed9eba1)
                    ++round;
            }
            while (round < 60)
            {
                w[round] = rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
                sha1macro((b & c) | (b & d) | (c & d), 0x8f1bbcdc)
                    ++round;
            }
            while (round < 80)
            {
                w[round] = rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
                sha1macro(b ^ c ^ d, 0xca62c1d6)
                    ++round;
            }

#undef sha1macro

            result[0] += a;
            result[1] += b;
            result[2] += c;
            result[3] += d;
            result[4] += e;
        }
    } // namespace

    static void calc(const void* src, const int bytelength, unsigned char* hash)
    {
        // Init the result array.
        unsigned int result[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};

        // Cast the void src pointer to be the byte array we can work with.
        const unsigned char* sarray = (const unsigned char*)src;

        // The reusable round buffer
        unsigned int w[80];

        // Loop through all complete 64byte blocks.
        const int endOfFullBlocks = bytelength - 64;
        int endCurrentBlock;
        int currentBlock = 0;

        while (currentBlock <= endOfFullBlocks)
        {
            endCurrentBlock = currentBlock + 64;

            // Init the round buffer with the 64 byte block data.
            for (int roundPos = 0; currentBlock < endCurrentBlock; currentBlock += 4)
            {
                // This line will swap endian on big endian and keep endian on little endian.
                w[roundPos++] = (unsigned int)sarray[currentBlock + 3]
                    | (((unsigned int)sarray[currentBlock + 2]) << 8)
                    | (((unsigned int)sarray[currentBlock + 1]) << 16)
                    | (((unsigned int)sarray[currentBlock]) << 24);
            }
            innerHash(result, w);
        }

        // Handle the last and not full 64 byte block if existing.
        endCurrentBlock = bytelength - currentBlock;
        clearWBuffert(w);
        int lastBlockBytes = 0;
        for (; lastBlockBytes < endCurrentBlock; ++lastBlockBytes)
        {
            w[lastBlockBytes >> 2] |= (unsigned int)sarray[lastBlockBytes + currentBlock] << ((3 - (lastBlockBytes & 3)) << 3);
        }
        w[lastBlockBytes >> 2] |= 0x80 << ((3 - (lastBlockBytes & 3)) << 3);
        if (endCurrentBlock >= 56)
        {
            innerHash(result, w);
            clearWBuffert(w);
        }
        w[15] = bytelength << 3;
        innerHash(result, w);

        // Store hash in result pointer, and make sure we get in in the correct order on both endian models.
        for (int hashByte = 20; --hashByte >= 0;)
        {
            hash[hashByte] = (result[hashByte >> 2] >> (((3 - hashByte) & 0x3) << 3)) & 0xff;
        }
    }

    static void toHexString(const unsigned char* hash, char* hexstring)
    {
        const char hexDigits[] = {"0123456789abcdef"};

        for (int hashByte = 20; --hashByte >= 0;)
        {
            hexstring[hashByte << 1] = hexDigits[(hash[hashByte] >> 4) & 0xf];
            hexstring[(hashByte << 1) + 1] = hexDigits[hash[hashByte] & 0xf];
        }
        hexstring[40] = 0;
    }
} // namespace sha1

/*
base64.cpp and base64.h

Copyright (C) 2004-2008 Ren� Nyffenegger

This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.

3. This notice may not be removed or altered from any source distribution.

Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#include <iostream>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(uint8_t c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

namespace onut
{
    namespace crypto
    {
        std::string base64_encode(uint8_t const* buf, unsigned int bufLen)
        {
            std::string ret;
            int i = 0;
            int j = 0;
            uint8_t char_array_3[3];
            uint8_t char_array_4[4];

            while (bufLen--)
            {
                char_array_3[i++] = *(buf++);
                if (i == 3)
                {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for (i = 0; (i < 4); i++)
                        ret += base64_chars[char_array_4[i]];
                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 3; j++)
                    char_array_3[j] = '\0';

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (j = 0; (j < i + 1); j++)
                    ret += base64_chars[char_array_4[j]];

                while ((i++ < 3))
                    ret += '=';
            }

            return ret;
        }

        std::vector<uint8_t> base64_decode(const std::string& encoded_string)
        {
            int in_len = (int)encoded_string.size();
            int i = 0;
            int j = 0;
            int in_ = 0;
            uint8_t char_array_4[4], char_array_3[3];
            std::vector<uint8_t> ret;

            while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
            {
                char_array_4[i++] = encoded_string[in_]; in_++;
                if (i == 4)
                {
                    for (i = 0; i < 4; i++)
                        char_array_4[i] = (uint8_t)base64_chars.find(char_array_4[i]);

                    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                    for (i = 0; (i < 3); i++)
                        ret.push_back(char_array_3[i]);
                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 4; j++)
                    char_array_4[j] = 0;

                for (j = 0; j < 4; j++)
                    char_array_4[j] = (uint8_t)base64_chars.find(char_array_4[j]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
            }

            return std::move(ret);
        }
    }
}

namespace onut
{
    namespace crypto
    {
        uint32_t hash(const std::string &str, unsigned int seed)
        {
            unsigned hash = seed;
            const char *s = str.c_str();
            while (*s) hash = hash * 101 + *s++;
            return hash;
        }

        std::string sha1(const std::string& str)
        {
            unsigned char resultHash[20] = {0};
            char hexstring[41];

            sha1::calc(str.c_str(), (int)str.size(), resultHash);
            sha1::toHexString(resultHash, hexstring);

            return hexstring;
        }

        std::string md5(const std::string& str)
        {
            char hash[33];
            md5::md5_t md5(str.c_str(), str.size());
            md5.get_string(hash);
            return hash;
        }

        bool validateEmail(const std::string& email)
        {
            return isValidEmailAddress(email.c_str());
        }
    }
};
