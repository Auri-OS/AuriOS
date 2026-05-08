#include "../include/terminal.h"
#include "../include/string.h"

// Convert a String to an Integer 
int	atoi(char *str)
{
	int		i;
	int		nbr;
	int		negative;

	nbr = 0;
	negative = 0;
	i = 0;
	while ((str[i] == '\n') || (str[i] == '\t') || (str[i] == '\v') \
|| (str[i] == ' ') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if (str[i] == '-')
		negative = 1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] && (str[i] >= '0') && (str[i] <= '9'))
	{
		nbr *= 10;
		nbr += (int)str[i] - '0';
		i++;
	}
	if (negative == 1)
		return (-nbr);
	else
		return (nbr);
}

// Convert an Integer to a String
char *itoa(int nb, char *buffer)
{
    int			i;
    int			negative;

    i = 0;
    negative = 0;
    if (nb < 0)
    {
        negative = 1;
        nb = -nb;
    }
    if (nb == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return (buffer);
    }
    while (nb > 0)
    {
        buffer[i++] = (nb % 10) + '0';
        nb /= 10;
    }
    if (negative)
        buffer[i++] = '-';
    buffer[i] = '\0';
    i = 0;
    while (i < (int)strlen(buffer) / 2)
    {
        char tmp = buffer[i];
        buffer[i] = buffer[strlen(buffer) - 1 - i];
        buffer[strlen(buffer) - 1 - i] = tmp;
        i++;
    }
    return (buffer);
}

uint32_t htoi(const char *hex_str) {
    uint32_t result = 0;

    while (*hex_str == ' ') {
        hex_str++;
    }

    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        hex_str += 2;
    }

    while (*hex_str != '\0') {
        char c = *hex_str;
        uint32_t value = 0;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            value = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;
        } else {
            break;
        }

        result = (result << 4) | value;
        
        hex_str++;
    }

    return result;
}
