/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_byte.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 20:47:54 by bpierce           #+#    #+#             */
/*   Updated: 2018/06/04 20:48:18 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

/*
**	Utility functions for helping control the start of the game
*/

uint8_t		parse_arguments(t_process *process, uint8_t read_two_bytes)
{
	uint8_t i;
	uint8_t	success;

	i = -1;
	success = 1;
	while (++i < 3)
	{
		if (process->encoding_byte[i] == REGISTER)
		{
			process->args[i][0] = process->curr_pc->value;
			process->curr_pc = process->curr_pc->next;
			if (process->args[i][0] > REG_NUMBER || process->args[i][0] < 1)
				success = 0;
		}
		else if (process->encoding_byte[i] == DIRECT)
		{
			process->args[i][0] = process->curr_pc->value;
			process->args[i][1] = process->curr_pc->next->value;
			process->args[i][2] = process->curr_pc->next->next->value;
			process->args[i][3] = process->curr_pc->next->next->next->value;
			if (read_two_bytes)
				process->curr_pc = process->curr_pc->next->next;
			else
				process->curr_pc = process->curr_pc->next->next->next->next;
		}
		else if (process->encoding_byte[i] == INDIRECT)
		{
			process->args[i][0] = process->curr_pc->value;
			process->args[i][1] = process->curr_pc->next->value;
			process->args[i][2] = 0;
			process->args[i][3] = 0;
			process->curr_pc = process->curr_pc->next->next;
		}
	}
	return (success);
}

/*
**	Processes the encoding byte and moves the curr_pc to the next node
**	Returns 1 = success, 0 = FAILURE
*/

// uint8_t		parse_encoding_byte(t_process *process)
// {
// 	uint8_t encoding_byte;

// 	encoding_byte = process->curr_pc->next->value;
// 	process->curr_pc = process->curr_pc->next->next;
// 	if (encoding_byte > 0xF8 || (encoding_byte & 0b11) > 0)
// 		return (0);
// 	process->encoding_byte[0] = (encoding_byte >> 6) & 0b11;
// 	process->encoding_byte[1] = (encoding_byte >> 4) & 0b11;
// 	process->encoding_byte[2] = (encoding_byte >> 2) & 0b11;
// 	return (1);
// }

uint8_t		parse_encoding_byte(t_process *process)
{
	uint8_t encoding_byte;

	encoding_byte = process->curr_pc->next->value;
	process->curr_pc = process->curr_pc->next->next;
	process->encoding_byte[0] = (encoding_byte >> 6) & 0b11;
	process->encoding_byte[1] = (encoding_byte >> 4) & 0b11;
	process->encoding_byte[2] = (encoding_byte >> 2) & 0b11;
	return (!(encoding_byte > 0xF8 || (encoding_byte & 0b11) > 0));
}

/*
**	Processes an encoding byte and moves the pc
*/

void		move_pc_by_encoding_byte(t_process *process, uint8_t read_two_bytes)
{
	uint8_t		i;

	i = -1;
	while (++i < 3)
	{
		if (process->encoding_byte[i] == REGISTER)
			process->curr_pc = process->curr_pc->next;
		else if (process->encoding_byte[i] == DIRECT)
		{
			if (read_two_bytes)
				process->curr_pc = process->curr_pc->next->next;
			else
				process->curr_pc = process->curr_pc->next->next->next->next;
		}
		else if (process->encoding_byte[i] == INDIRECT)
			process->curr_pc = process->curr_pc->next->next;
	}
}

/*
**	Returns a uint32_t of the number stored in an array 0, 1, 2, 3
*/

uint32_t	smash_bytes(uint8_t *reg)
{
	uint32_t	result;

	result = 0;
	result |= ((uint32_t)reg[0]) << 24;
	result |= ((uint32_t)reg[1]) << 16;
	result |= ((uint32_t)reg[2]) << 8;
	result |= ((uint32_t)reg[3]) << 0;
	return (result);
}

uint8_t		*unsmash_bytes(uint32_t nbr)
{
	static uint8_t	unsmashme[4];

	unsmashme[0] = (nbr >> 24);
	unsmashme[1] = (nbr >> 16) & 0xFF;
	unsmashme[2] = (nbr >> 8) & 0xFF;
	unsmashme[3] = (nbr >> 0) & 0xFF;
	return (unsmashme);
}
