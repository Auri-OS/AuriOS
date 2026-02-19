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

