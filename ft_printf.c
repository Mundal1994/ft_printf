/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molesen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 11:28:12 by molesen           #+#    #+#             */
/*   Updated: 2022/02/21 13:28:27 by molesen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_initialize_flag(t_flag *flag, int first)
{
	flag->flags = FALSE;
	flag->hash = FALSE;
	flag->space = FALSE;
	flag->minus = FALSE;
	flag->plus = FALSE;
	flag->zero = FALSE;
	flag->width = FALSE;
	flag->prec = FALSE;
	flag->hh = FALSE;
	flag->h = FALSE;
	flag->l = FALSE;
	flag->ll = FALSE;
	flag->s_l = FALSE;
	flag->b_l = FALSE;
	if (first == TRUE)
	{
		flag->i = 0;
		flag->len = 0;
	}
}

static void	ft_convert_checker(const char *format, t_flag *flag, va_list *arg)
{
	int	i;

	i = 0;
	if (ft_convert_symbol(&format[i], flag, arg) == FALSE)
	{
		while (ft_convert_symbol(&format[i], flag, arg) == FALSE)
		{
			flag->flags = TRUE;
			ft_flag_check(&format[i], flag, TRUE);
			i++;
		}
	}
	//ft_str_convert(str, flag);
	//return the new string length added - changes i's number.
}

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	t_flag	*flag;

	flag = (t_flag *) malloc(sizeof(t_flag));
	if (!flag)
		ft_putstr("error");
	ft_initialize_flag(flag, TRUE);
	va_start(arg, format);
	if (format)
	{
		while (format[flag->i] != '\0')
		{
			if (format[flag->i] != '%')
				ft_putchar(format[flag->i]);
			else
			{
				flag->i++;
				if (format[flag->i] == '%')
					ft_putchar('%');
				else
				{
					ft_convert_checker(&format[flag->i], flag, &arg);
					ft_initialize_flag(flag, FALSE);
				}
			}
			/*ft_putchar('\n');
			ft_putnbr(flag->i);
			ft_putchar('\n');
			ft_putnbr(flag->len);
			ft_putchar('\n');
			*/
			flag->i++;
			flag->len++;
		}
	}
	va_end(arg);
	return (flag->len); 
}
