/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:20:41 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:20:43 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <map>
# include <vector>
# include <string>
# include <algorithm>
# include <stdexcept>
# include <algorithm>
# include <cctype>
# include "../Utils/Utils.hpp"

class Message {
private:
    std::string                 _prefix;
    std::string                 _command;
    std::vector<std::string>    _params;

    void    _tokenize(std::string message); //parser
public:
    Message(std::string message);
    Message(const Message &other);
    ~Message(void);
    Message &operator=(const Message &other);

    std::string                 get_prefix(void) const;
    std::string                 get_command(void) const;
    std::vector<std::string>    get_params(void) const;
};
