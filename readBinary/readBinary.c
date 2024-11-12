#include <stdio.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Please add a file name\n");
        return 1;
    }

    FILE* binaryFile;
    binaryFile = fopen(argv[1], "rb");
    if(binaryFile == NULL) {
        fprintf(stderr, "Unable to open file %s\n", argv[1]);
        return 1;
    }

    int count = 0;
    unsigned char buffer = 0;
    while(fread(&buffer, sizeof(buffer), 1, binaryFile) != 0) {
        for(int i = 7; i >= 0; i--) {
            if(buffer & (1<<i)) {
                printf("1");   
            } else {
                printf("0");
            }
        }
        printf(" ");
        if(count == 3) {
            printf("\n");
            count = 0;
        } else {
            count++;
        }
    }
    printf("\n");
}