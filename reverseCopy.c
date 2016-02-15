#include <stdio.h>
#include <errno.h>

long file_character_count(FILE *);

int main(int argc, char *argv[])
{
    FILE *read_file, *write_file;

    // open arg1 for reading
    read_file = fopen(argv[1], "r");
    // open arg2 for writing
    write_file = fopen(argv[2], "w");
    
    long read_file_count = file_character_count(read_file);

    // read_file ptr -> end of file
    fseek(read_file, -1L, SEEK_END);
    
    while(read_file_count > 0)
    {
        char ch = fgetc(read_file);
        fputc(ch, write_file);
        
        // read_file ptr -> 1 previous character
        fseek(read_file, -2L, SEEK_CUR);
        read_file_count--;
    }
    
    // because im nice
    fclose(read_file);
    fclose(write_file);
}

long file_character_count(FILE *f)
{
    fseek(f, -1L, SEEK_END);
    return (ftell(f)) + 1;
}