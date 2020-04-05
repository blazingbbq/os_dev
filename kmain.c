#include "framebuffer.h"

/* C entrypoint */
void kmain() {
    char buf[] = "Hello, Framebuffer!";

    fb_write(buf, 20);
}
