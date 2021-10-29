#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    FILE* df_out = popen("df -h", "r");

    char grep_cmd[256] = "grep ";
    assert(argc > 1);
    strncat(grep_cmd, argv[1], sizeof(grep_cmd) - strlen(grep_cmd) - 1);
    FILE* grep_in = popen(grep_cmd, "w");

    int ch;
    while (!feof(df_out) && (ch = fgetc(df_out)) != EOF) {
        fputc(ch, grep_in);
    }
    pclose(df_out);
    pclose(grep_in);
    return 0;
}
