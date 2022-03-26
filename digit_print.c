/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digit_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molesen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 20:20:19 by molesen           #+#    #+#             */
/*   Updated: 2022/03/24 20:20:21 by molesen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
**	when placing furthest left we check flags to know index to place digit
**	this function checks for float specific rules
*/

static int	ft_check_float_flags(char *str, t_flag *flag, int total, int len)
{
	if (flag->prec < len && flag->width == -1)
	{
		if ((flag->plus == '+' || flag->space == ' ') && str[0] != '-')
			total++;
	}
	else if (flag->width <= len)
	{
		if (str[0] == '-' && (flag->width > -1 || flag->prec > -1))
			total++;
		else if ((flag->plus == '+' || flag->space == ' ') && str[0] != '-')
			total++;
	}
	else if (flag->minus == '-' && flag->plus == '+')
		total++;
	return (total);
}

/*	when placing furthest left we check flags to know index to place digit	*/

int	ft_check_flags_digit(char *str, t_flag *flag, int total, int len)
{
	if (flag->spec == 'f')
		return (ft_check_float_flags(str, flag, total, len));
	else
	{
		if ((flag->width <= len && flag->width <= flag->prec) || \
		(flag->prec >= len && flag->prec > flag->width))
		{
			if (str[0] == '-' && (flag->width > -1 || flag->prec > -1))
				return (++total);
			else if ((flag->plus == '+' || flag->space == ' ') && str[0] != '-')
				return (++total);
		}
		if (flag->width > len && flag->prec == -1 && flag->minus == '-' && \
			str[0] == '-')
			total++;
		else if (flag->plus == '+' && flag->width > flag->prec && \
			flag->width > len)
			total++;
		else if (flag->plus == '+' && flag->minus == '1' && flag->width < len \
			&& flag->prec < len)
			total++;
	}
	return (total);
}

/*	base function for printing digits d,u,f,o,x,X	*/

static void	ft_digit_to_str(char **temp, char *str, t_flag *flag, int i)
{
	int	len;

	len = ft_strlen(str);
	if (str[0] == '-')
		len--;
	if (flag->minus != '-')
		i -= len;
	if (flag->prec == 0 && ft_strcmp(str, "0") == 0)
	{
		if (spec_check(flag, 'o', 'x', 'X') == TRUE && flag->hash == TRUE)
			(*temp)[i] = '0';
		else
			(*temp)[i] = ' ';
	}
	else if (str[0] == '-')
		ft_strcpy(&(*temp)[i], &str[1]);
	else
		ft_strcpy(&(*temp)[i], str);
}

/*
**	calculate the amount of space we need to consider when printing digits
**	narrows down what extra needs to be printed to have correct width
*/

static void	ft_minus_flag_strcpy(char *str, t_flag *flag, int total)
{
	int	i;
	int	new_total;

	i = 0;
	new_total = ft_check_flags_digit(str, flag, total, flag->len);
	if (spec_check(flag, 'n', 'x', 'X') == TRUE && flag->hash == TRUE)
		i += 2;
	else if (flag->spec == 'o' && flag->hash == TRUE)
		i++;
	if (flag->prec > flag->len && flag->hash == TRUE && flag->spec == 'o')
		i += flag->prec - flag->len - 1;
	else if (flag->prec > flag->len)
		i += flag->prec - flag->len;
	if (total < new_total)
		i++;
	ft_digit_to_str(&flag->str, str, flag, i);
	while (i < total)
	{
		if (flag->str[i] == '\0')
			flag->str[i] = ' ';
		i++;
	}
}

/*	base function for printing digits d,u,f,o,x,X	*/

void	ft_digit_print(char *str, t_flag *flag, int total)
{
	if (spec_check(flag, 'd', 'u', 'f') == TRUE || \
		spec_check(flag, 'n', 'x', 'X') == TRUE)
		if (flag->prec == 0 && ft_strcmp(str, "0") == 0 && flag->width == -1)
			return ;
	flag->str = ft_strnew(total);
	if (!flag->str)
	{
		ft_strdel(&str);
		return (ft_putstr_fd("error\n", 2));
	}
	ft_set_base_str(str, flag, total);
	if (!(flag->spec != 'u' && ft_strcmp(str, "0") == 0 && \
		ft_strlen(str) == 0 && flag->prec == 0))
	{
		if (flag->minus == '1')
			ft_digit_to_str(&flag->str, str, flag, total);
		else
			ft_minus_flag_strcpy(str, flag, total);
	}
	flag->ret += write(1, flag->str, total);
	ft_strdel(&str);
	ft_strdel(&flag->str);
}
