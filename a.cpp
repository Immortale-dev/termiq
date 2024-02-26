#include <unistd.h>

#include <stdio.h>
#include <term.h>
#include <err.h>

int main(void){
        char *cup;
        int err;
        if(setupterm(0, 1, &err))
                errx(1, "setupterm failed");
        if(!(cup = tigetstr("cup")))
                errx(1, "no cursor addressing in terminal");
        tputs(tigetstr("smcup"), 1, putchar);


        tputs(tiparm(cup, 13, 13), 1, putchar);
        printf("SZ: %i", tigetnum("lines"));
        printf("HELLO");        /* or whatever you like */
        getchar();

        tputs(tigetstr("rmcup"), 1, putchar);
}
