/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molesen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:59:43 by molesen           #+#    #+#             */
/*   Updated: 2022/03/15 16:59:56 by molesen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*	adds the needed zeros to the string	*/

static void	ft_itoa_add_zeros(unsigned long long nbr, char *str, int len, \
int neg)
{
	int	i;

	i = 0;
	if (nbr == 0 && len == 0)
		str[i++] = '0';
	while (nbr)
	{
		str[i++] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	while (i < len)
		str[i++] = '0';
	if (neg == -1)
		str[i++] = '-';
	ft_strrev_len(str, i);
	str[i] = '\0';
}

/*	takes number and stores it in a string	*/

static void	ft_fcalc(long long number, char *temp, int neg)
{
	long	nbr;

	if ((long long)number == 0)
	{
		if (neg == 1)
		{
			temp[0] = '-';
			temp[1] = '0';
		}
		else
			temp[0] = '0';
		return ;
	}
	nbr = (long long)number;
	if (neg == 1)
	{
		neg = -1;
		nbr *= neg;
	}
	ft_itoa_add_zeros((double)nbr, temp, 0, neg);
}

/*	calculates if we need to round up or not when deciding decimal length	*/

static int	ft_rounding(long double lnbr, int len, char *temp, int *i)
{
	int	nbr;

	if (len != 0)
	{
		lnbr = lnbr * ft_pow(10, len);
		ft_itoa_add_zeros((unsigned long long)lnbr, &temp[*i], len, 1);
		nbr = temp[ft_strlen(temp) - 1] - '0';
	}
	else
		nbr = temp[*i - 2] - '0';
	lnbr = lnbr - (unsigned long long)lnbr;
	if (lnbr > 0.5)
		return (TRUE);
	else if (lnbr < 0.5)
		return (FALSE);
	if (nbr % 2 != 0)
		return (TRUE);
	return (FALSE);
}

/*	calculates digits	*/

static void	*ft_calc_decimals(long double lnbr, int *i, char *temp, int len)
{
	int	up;

	if (lnbr < 0)
		lnbr *= -1;
	temp[(*i)++] = '.';
	up = ft_rounding(lnbr, len, temp, i);
	ft_check_correct_end(temp, up);
	return (temp);
}

/*	converts float(double) to str	*/

char	*ft_ftoa(long double number, int len, t_flag *flag)
{
	long double	lnbr;
	int			i;
	int			neg;
	char		*temp;
	char		*str;

	lnbr = number;
	neg = 0;
	if (1 / number < 0 && (long long)number == 0)
		neg = 1;
	temp = ft_strnew(ft_long_len((long long)number) + len + 1 + neg);
	if (1 / number < 0)
		neg = 1;
	ft_fcalc(number, temp, neg);
	lnbr = number - (long long)number;
	i = ft_strlen(temp);
	if (len != 0 || (len == 0 && flag->hash == TRUE))
		ft_calc_decimals(lnbr, &i, temp, len);
	str = ft_strnew(i + len);
	ft_strncpy(str, temp, i + len);
	ft_strdel(&temp);
	return (str);
}
