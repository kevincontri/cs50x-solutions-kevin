#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// A program to recover JPEGs from a memory card.
int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover <raw_file>\n");
        return 1;
    }

    // Open the memory card and checks for errors
    FILE *input = fopen(argv[1], "rb");
    if (!input)
    {
        fprintf(stderr,
                "Could not open file "
                "%s"
                "\n",
                argv[1]);
        return 1;
    }

    uint8_t buffer[512];  // Holds one 512-byte block
    int file_count = 0;   // JPEG file counter
    FILE *output = NULL;  // No JPEG opened yet
    size_t bytesRead = 0; // Initializing the return value of fread()

    while ((bytesRead = fread(buffer, 1, 512, input)) > 0) // Check until end of memory card
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (output != NULL) // If i am already writing a JPEG
            {
                fclose(output); // Close that one first
            }
            // If i'm not writing another JPEG file, just continue
            char file_name[8];
            sprintf(file_name, "%03d.jpg", file_count);

            output = fopen(file_name, "wb"); // Open a new JPEG file
            if (!output)                     // Check for any errors
            {
                fprintf(stderr, "Error\n");
                return 1;
            }

            file_count++; // Prepare the next file name for the next time
        }
        if (output != NULL) // If currently writing a JPEG
        {
            fwrite(buffer, bytesRead, 1, output); // Write all the read blocks
        }
    } // Loop will continue until a new header is found

    if (output != NULL)
    {
        fclose(output); // Close the last JPEG
    }
    fclose(input); // Close the memory card file
    return 0;
}
