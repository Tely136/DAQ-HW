#include <fcntl.h>
#include <linux/gpio.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define LED_OFFSET 2 /* Channel 2, bit 0; channel 1 has two lines. */

int main(void)
{
    /* 1. Open the GPIO controller. */
    int chip_fd = open(GPIO_CHIP, O_RDONLY);
    if (chip_fd < 0) {
        perror("open GPIO chip");
        return 1;
    }

    /* 2. Request one output. Zero-initialize all unused fields.
     * The initial output value defaults to low.
     */
    struct gpio_v2_line_request request = {0};
    request.offsets[0] = LED_OFFSET;
    request.num_lines = 1;
    request.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;

    if (ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &request) < 0) {
        perror("request LED output");
        close(chip_fd);
        return 1;
    }
    close(chip_fd); /* Further operations use the returned request.fd. */

    /* 3. Blink ten times. Mask bit 0 selects request.offsets[0]: line 2. */
    struct gpio_v2_line_values value = {0};
    value.mask = 1;

    for (int blink = 0; blink < 10; ++blink) {
        value.bits = 1; /* LED high */
        if (ioctl(request.fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &value) < 0) {
            perror("set LED high");
            close(request.fd);
            return 1;
        }
        sleep(1);

        value.bits = 0; /* LED low */
        if (ioctl(request.fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &value) < 0) {
            perror("set LED low");
            close(request.fd);
            return 1;
        }
        sleep(1);
    }

    /* 4. Release the line. Its state after release is not guaranteed.
     * Ctrl+C also releases it, but may leave the LED high.
     */
    close(request.fd);
    return 0;
}
