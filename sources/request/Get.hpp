/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:16:33 by fberthou          #+#    #+#             */
/*   Updated: 2025/01/14 13:21:12 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

#include "ARequest.hpp"

class Get : virtual public ARequest
{
	public:
		Get();
		~Get();

		void	execute();
	
};

#endif