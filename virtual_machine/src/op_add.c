/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 13:44:22 by bpierce           #+#    #+#             */
/*   Updated: 2018/06/02 14:56:15 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

/*
**	0x04 - ADD
**	- - - - - - - - - -
**	Takes 3 registers. Adds content of first two, and stores it in reg 3
*/

void		add_(t_corewar *core, t_process *process)
{
	uint32_t	additive_result;

	(void)core;
	if (!parse_encoding_byte(process) ||
		EB0 != REGISTER || EB1 != REGISTER || EB2 != REGISTER)
	{
		move_pc_by_encoding_byte(process, 0);
		return ;
	}
	if (!parse_arguments(process, 0))
		return ;
	additive_result = smash_bytes(REG[ARG00]);
	additive_result += smash_bytes(REG[ARG10]);
	REG[ARG20][0] = (uint8_t)(additive_result >> 24);
	REG[ARG20][1] = (uint8_t)(additive_result >> 16);
	REG[ARG20][2] = (uint8_t)(additive_result >> 8);
	REG[ARG20][3] = (uint8_t)(additive_result >> 0);
	process->carry = !smash_bytes(REG[ARG20]);
}
